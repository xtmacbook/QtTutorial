#include "feature.h"
#include "imgMgr.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgproc.hpp>


#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace cv;
using namespace cv::xfeatures2d;

const int GOOD_PTS_MAX = 50;
const float GOOD_PORTION = 0.15f;

extern Mat LMS(const Mat&match1_xy, const Mat &match2_xy,  float &rmse);

Feature::Feature():type_(FEATURE_ALG::ISURF)
{
}

void Feature::setType(FEATURE_ALG type)
{
	type_ = type;
}

const char * Feature::types(FEATURE_ALG idx) const
{
    static const char *names[NONE] = {
		"SIFT",
		"SURF",
		"ORB"
	};
    return names[idx];
}

void Feature::match(int num_tile)
{
	//find feature and match
	Mat baseImg = ImgMgr::getInstance()->getBaseImg();
	Mat wrapImg = ImgMgr::getInstance()->getWrapImg();

	if (baseImg.empty() || wrapImg.empty()) return;

	Mat gray_base_img;
	Mat gray_wrap_img;
	cvtColor(baseImg, gray_base_img, CV_RGB2GRAY);
	cvtColor(wrapImg, gray_wrap_img, CV_RGB2GRAY);

	switch (type_)
	{
	/*case FEATURE_ALG::IGMS:
		gmsMatch(gray_base_img, gray_wrap_img, num_tile);
		break;*/
	case FEATURE_ALG::ISIFT:
		siftMatch(gray_base_img, gray_wrap_img, num_tile);
		break;
	case FEATURE_ALG::ISURF:
        surfMatch(gray_base_img, gray_wrap_img,num_tile);
		break;
	case FEATURE_ALG::IORB:
		orbMatch(gray_base_img, gray_wrap_img, num_tile);
	case FEATURE_ALG::NONE:
		break;
	default:
		break;
	}

    if(!k1_.empty() && !k2_.empty())
    {
        Mat match1_xy,match2_xy;
        float rmse;

        match1_xy.create(k1_.size(), 2, CV_32FC1);
        match2_xy.create(k2_.size(), 2, CV_32FC1);

        for(unsigned int i = 0;i < k1_.size();i++)
        {
            const cv::Point2f& point =  k1_.at(i);
            match1_xy.at<float>(i, 0) = point.x;
            match1_xy.at<float>(i, 1) = point.y;
        }
        for(unsigned int i = 0;i < k2_.size();i++)
        {
            const cv::Point2f& point =  k2_.at(i);
            match2_xy.at<float>(i, 0) = point.x;
            match2_xy.at<float>(i, 1) = point.y;
        }

        Mat mat =  LMS(match1_xy, match2_xy,rmse);
		const int N = mat.rows;//特征点个数
		for (unsigned i = 0; i < N; i++)
		{
			error_.push_back(std::sqrt(mat.at<float>(i, 0) + mat.at<float>(i, 1)));
		}
		
    }
}

Feature::~Feature()
{

}


static void getGoodMatches(
	const Mat& img1,
	const Mat& img2,
	const std::vector<KeyPoint>& keypoints1,
	const std::vector<KeyPoint>& keypoints2,
	std::vector<DMatch>& matches,
	std::vector< DMatch >& good_matches
)
{
	//-- Sort matches and preserve top 10% matches
	std::sort(matches.begin(), matches.end());
	
	double minDist = matches.front().distance;
	double maxDist = matches.back().distance;

	const int ptsPairs = std::min(GOOD_PTS_MAX, (int)(matches.size() * GOOD_PORTION));
	for (int i = 0; i < ptsPairs; i++)
	{
		good_matches.push_back(matches[i]);
	}

	std::cout << "\nMax distance: " << maxDist << std::endl;
	std::cout << "Min distance: " << minDist << std::endl;

	std::cout << "Calculating homography using " << ptsPairs << " point pairs." << std::endl;
}

static Mat draw(const Mat& img1,
	const Mat& img2,
	const std::vector<KeyPoint>& keypoints1,
	const std::vector<KeyPoint>& keypoints2,
	std::vector<DMatch>& matches, std::vector< DMatch >& good_matches)
{
	Mat img_matches;

	drawMatches(img1, keypoints1, img2, keypoints2,
		good_matches, img_matches, Scalar::all(-1), Scalar::all(-1),
		std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

	//-- Localize the object
	std::vector<Point2f> obj;
	std::vector<Point2f> scene;

	for (size_t i = 0; i < good_matches.size(); i++)
	{
		//-- Get the keypoints from the good matches
		obj.push_back(keypoints1[good_matches[i].queryIdx].pt);
		scene.push_back(keypoints2[good_matches[i].trainIdx].pt);
	}
	//-- Get the corners from the image_1 ( the object to be "detected" )
	std::vector<Point2f> obj_corners(4);
	obj_corners[0] = Point(0, 0);
	obj_corners[1] = Point(img1.cols, 0);
	obj_corners[2] = Point(img1.cols, img1.rows);
	obj_corners[3] = Point(0, img1.rows);
	std::vector<Point2f> scene_corners(4);

	Mat H = findHomography(obj, scene, RANSAC);
	perspectiveTransform(obj_corners, scene_corners, H);

	//scene_corners_ = scene_corners;

	//-- Draw lines between the corners (the mapped object in the scene - image_2 )
	line(img_matches,
		scene_corners[0] + Point2f((float)img1.cols, 0), scene_corners[1] + Point2f((float)img1.cols, 0),
		Scalar(0, 255, 0), 2, LINE_AA);
	line(img_matches,
		scene_corners[1] + Point2f((float)img1.cols, 0), scene_corners[2] + Point2f((float)img1.cols, 0),
		Scalar(0, 255, 0), 2, LINE_AA);
	line(img_matches,
		scene_corners[2] + Point2f((float)img1.cols, 0), scene_corners[3] + Point2f((float)img1.cols, 0),
		Scalar(0, 255, 0), 2, LINE_AA);
	line(img_matches,
		scene_corners[3] + Point2f((float)img1.cols, 0), scene_corners[0] + Point2f((float)img1.cols, 0),
		Scalar(0, 255, 0), 2, LINE_AA);
	return img_matches;
}

bool Feature::siftMatch(cv::Mat base, cv::Mat wrap, int num_tile)
{
	
	k1_.clear();
	k2_.clear();
	error_.clear();

	int minHessian = 400;
	Ptr<SURF> detector = SURF::create(minHessian);
	std::vector<KeyPoint> keypoints1, keypoints2;
	Mat descriptors1, descriptors2;
	detector->detectAndCompute(base, noArray(), keypoints1, descriptors1);
	detector->detectAndCompute(wrap, noArray(), keypoints2, descriptors2);

	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::BRUTEFORCE);
	std::vector< DMatch > matches;
	matcher->match(descriptors1, descriptors2, matches);

	std::vector<DMatch> good_matches;
	getGoodMatches(base, wrap, keypoints1, keypoints2, matches, good_matches);

	int need_tilts_number = (num_tile < good_matches.size()) ? num_tile : good_matches.size();
	for (size_t i = 0; i < need_tilts_number; i++)
	{
		k1_.push_back(keypoints1[good_matches[i].queryIdx].pt);
		k2_.push_back(keypoints2[good_matches[i].trainIdx].pt);
	}

	return true;
}

bool Feature::gmsMatch(cv::Mat base, cv::Mat wrap, int num_tile)
{
	Ptr<Feature2D> orb = ORB::create();  
	orb.dynamicCast<cv::ORB>()->setFastThreshold(20);  
	Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create("BruteForce-Hamming");

	bool withRotation = true; 
	bool withScale = true;
	bool drawSimple = true;

	std::vector<KeyPoint> kpRef, kpCur;
	Mat descRef, descCur;
	orb->detectAndCompute(base, noArray(), kpRef, descRef);
	orb->detectAndCompute(wrap, noArray(), kpCur, descCur);

	std::vector<DMatch> matchesAll, matchesGMS;
	matcher->match(descCur, descRef, matchesAll);

	matchGMS(base.size(), wrap.size(), kpCur, kpRef, matchesAll, matchesGMS, withRotation, withScale);
		 
	std::vector<DMatch> good_matches;
	getGoodMatches(base, wrap, kpCur, kpRef, matchesGMS, good_matches);

	int need_tilts_number = (num_tile < good_matches.size()) ? num_tile : good_matches.size();
	for (size_t i = 0; i < need_tilts_number; i++)
	{
		k1_.push_back(kpCur[good_matches[i].queryIdx].pt);
		k2_.push_back(kpRef[good_matches[i].trainIdx].pt);
	}

	return true;

}

bool Feature::orbMatch(cv::Mat base, cv::Mat wrap, int num_tile)
{
	std::vector<double> desMethCmp;
	Ptr<Feature2D> b =  ORB::create();
	std::vector<String>::iterator itDesc;

	std::vector<DMatch> matches;
	std::vector<KeyPoint> keyImg1, keyImg2;
	Mat descImg1, descImg2;

	b->detect(base, keyImg1, Mat());
	// and compute their descriptors with method  compute
	b->compute(base, keyImg1, descImg1);
	// or detect and compute descriptors in one step
	b->detectAndCompute(wrap, Mat(), keyImg2, descImg2, false);

	Ptr<DescriptorMatcher> descriptorMatcher = DescriptorMatcher::create("BruteForce");
	descriptorMatcher->match(descImg1, descImg2, matches, Mat());

	Mat index;
	int nbMatch = int(matches.size());
	Mat tab(nbMatch, 1, CV_32F);
	for (int i = 0; i < nbMatch; i++)
	{
		tab.at<float>(i, 0) = matches[i].distance;
	}
	sortIdx(tab, index, SORT_EVERY_COLUMN + SORT_ASCENDING);
	std::vector<DMatch> bestMatches;

	int returnNum = (num_tile < nbMatch) ? num_tile : nbMatch;

	for (int i = 0; i < returnNum; i++)
	{
		bestMatches.push_back(matches[index.at<int>(i, 0)]);
	}

	return true;

}

bool Feature::surfMatch(cv::Mat base, cv::Mat wrap, int num_tile)
{
	k1_.clear();
	k2_.clear();
	error_.clear();

	const int LOOP_NUM = 10;

	double hessian = 800.0;
	Ptr<Feature2D> surf = SURF::create(hessian);
	
	std::vector<KeyPoint> keypoints1,keypoints2;

	UMat _descriptors1, _descriptors2;
	Mat descriptors1 = _descriptors1.getMat(ACCESS_RW),
		descriptors2 = _descriptors2.getMat(ACCESS_RW);

	BFMatcher matcher;
	std::vector<DMatch> matches;
	for (int i = 0; i <= LOOP_NUM; i++)
	{ 
		surf->detectAndCompute(base, Mat(), keypoints1, descriptors1);
		surf->detectAndCompute(wrap, Mat(), keypoints2, descriptors2);

		matcher.match(descriptors1, descriptors2, matches);
	}

	std::vector<DMatch> good_matches;
	getGoodMatches(base, wrap, keypoints1, keypoints2, matches, good_matches);

    int need_tilts_number = (num_tile < good_matches.size()) ?num_tile :good_matches.size();
    for (size_t i = 0; i < need_tilts_number; i++)
	{
		k1_.push_back(keypoints1[good_matches[i].queryIdx].pt);
		k2_.push_back(keypoints2[good_matches[i].trainIdx].pt);
	}

	return true;
}

