#include "imgMgr.h"

#include <QDebug>

 ImgMgr *ImgMgr::local_instance = nullptr;
 

ImgMgr::~ImgMgr()
{

}

ImgMgr* ImgMgr::getInstance()
{
	if (local_instance == nullptr)
	{
		local_instance = new ImgMgr();
	}
	return local_instance;
}

bool ImgMgr::loadBaseImg(const QString &file)
{
    return loadImg(file);
}

bool ImgMgr::loadWrapImg(const QString &file)
{
    return loadImg(file,false);
}

cv::Mat ImgMgr::getBaseImg()
{
	return baseImgs_;
}

cv::Mat ImgMgr::getWrapImg()
{
	return wrapImgs_;
}

QImage ImgMgr::mat2Qimage(cv::Mat inMat)
{
    switch ( inMat.type() )
      {
         // 8-bit, 4 channel
         case CV_8UC4:
         {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

            return image;
         }

         // 8-bit, 3 channel
         case CV_8UC3:
         {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

            return image.rgbSwapped();
         }

         // 8-bit, 1 channel
         case CV_8UC1:
         {
            static QVector<QRgb>  sColorTable;

            // only create our color table once
            if ( sColorTable.isEmpty() )
            {
               for ( int i = 0; i < 256; ++i )
                  sColorTable.push_back( qRgb( i, i, i ) );
            }

            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );

            image.setColorTable( sColorTable );

            return image;
         }

         default:
            qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
      }

      return QImage();
}

bool ImgMgr::loadImg(const QString &file,bool base)
{
	if (base)
	{
		baseImgs_ = cv::imread(file.toStdString());

//        std::vector<cv::Rect> rects;
//        rects.push_back(cv::Rect(2549,724,523,1311));
//        rects.push_back(cv::Rect(2796,21,221,623));
//        rects.push_back(cv::Rect(2104,0,160,364));
//        rects.push_back(cv::Rect(1862,122,214,568));
//        rects.push_back(cv::Rect(2738,0,145,405));
//        rects.push_back(cv::Rect(2027,2,97,277));
//        rects.push_back(cv::Rect(1779,152,256,850));

//        for(unsigned int i = 0;i < rects.size();i++)
//        {
//            cv::rectangle(baseImgs_, rects.at(i), cv::Scalar(1.0,0.0,2.0), 1);
//        }

		return !(baseImgs_.empty());
	}
	else
	{
		wrapImgs_ = cv::imread(file.toStdString());
		return !(wrapImgs_.empty());
	}

}
