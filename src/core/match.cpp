#include <iostream>
#include <iomanip>

#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<algorithm>
#include<vector>
#include<cmath>

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>

using namespace std;
using namespace cv;


/********该函数根据正确的匹配点对，计算出图像之间的变换关系********/
/*match1_xy表示参考图像特征点坐标集合,[M x 2]矩阵，M表示特征的个数
 match2_xy表示待配准图像特征点集合，[M x 2]矩阵，M表示特征点集合
 model表示变换类型，“相似变换”,"仿射变换","透视变换"
 rmse表示均方根误差
 返回值为计算得到的3 x 3矩阵参数
 */
Mat LMS(const Mat&match1_xy, const Mat &match2_xy,  float &rmse)
{

	if (match1_xy.rows != match2_xy.rows)
		CV_Error(CV_StsBadArg, "LMS模块输入特征点对个数不一致！");


	const int N = match1_xy.rows;//特征点个数
	
	Mat match2_xy_trans, match1_xy_trans;//坐标转置
	transpose(match1_xy, match1_xy_trans);
	transpose(match2_xy, match2_xy_trans);

	Mat change = Mat::zeros(3, 3, CV_32FC1);

	//A*X=B,接下来部分仿射变换和透视变换一样,如果特征点个数是M，则A=[2*M,6]矩阵
	//A=[x1,y1,0,0,1,0;0,0,x1,y1,0,1;.....xn,yn,0,0,1,0;0,0,xn,yn,0,1]
	Mat A = Mat::zeros(2*N,6,CV_32FC1);
	for (int i = 0; i < N; ++i)
	{
		A.at<float>(2 * i, 0) = match2_xy.at<float>(i, 0);//x
		A.at<float>(2 * i, 1) = match2_xy.at<float>(i, 1);//y
		A.at<float>(2*i, 4) = 1.f;

		A.at<float>(2 * i + 1, 2) = match2_xy.at<float>(i, 0);
		A.at<float>(2 * i + 1, 3) = match2_xy.at<float>(i, 1);
		A.at<float>(2 * i+1, 5) = 1.f;
	}

	//如果特征点个数是M,那个B=[2*M,1]矩阵
	//B=[u1,v1,u2,v2,.....,un,vn]
	Mat B;
	B.create(2 * N, 1, CV_32FC1);
	for (int i = 0; i < N; ++i)
	{
		B.at<float>(2 * i, 0) = match1_xy.at<float>(i, 0);//x
		B.at<float>(2 * i + 1, 0) = match1_xy.at<float>(i, 1);//y
	}

	//仿射变换
	
	Vec6f values;
	solve(A, B, values, DECOMP_QR);
	change = (Mat_<float>(3,3)<<values(0), values(1), values(4),
		values(2), values(3), values(5),
		+0.0f, +0.0f, 1.0f);

	Mat temp_1 = change(Range(0, 2), Range(0, 2));//尺度和旋转量
	Mat temp_2 = change(Range(0, 2), Range(2, 3));//平移量
	
	Mat match2_xy_change = temp_1 * match2_xy_trans + repeat(temp_2, 1, N);
	Mat diff = match2_xy_change - match1_xy_trans;//求差
	
	pow(diff,2.f,diff);

	Mat diff_trans;
	transpose(diff, diff_trans);
	//rmse = (float)sqrt(sum(diff)(0)*1.0/N);//sum输出是各个通道的和
	//

	return diff_trans;
}






