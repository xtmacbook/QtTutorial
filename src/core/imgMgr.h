
#ifndef IMGMGR_H
#define IMGMGR_H
#include <QString>
#include <map>
#include <QImage>

#include <opencv2/opencv.hpp>
class ImgMgr
{
private:
	static ImgMgr * local_instance;
	ImgMgr() {}
	virtual ~ImgMgr();
public:

	static  ImgMgr* getInstance();

	bool loadBaseImg(const QString&file);
	bool loadWrapImg(const QString&file);

	cv::Mat getBaseImg();
	cv::Mat getWrapImg();
	static QImage mat2Qimage(cv::Mat);

private:
	bool loadImg(const QString&file, bool base = true);

private:

	cv::Mat baseImgs_;
	cv::Mat wrapImgs_;
};





#endif
