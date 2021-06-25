
#ifndef FEATURE_H
#define FEATURE_H

#include <QObject>
#include <vector>
#include <opencv2/core.hpp>


enum FEATURE_ALG
{
	//IGMS = 0,
	ISIFT = 0,
	ISURF,
	IORB,
	NONE
};


class Feature {

public:
	Feature();
	
    virtual ~Feature();

    void            setType(FEATURE_ALG);
    const char *    types(FEATURE_ALG)const ;
    void            match(int);
private:

	bool			siftMatch(cv::Mat, cv::Mat, int);
	bool			gmsMatch(cv::Mat, cv::Mat, int);
	bool			orbMatch(cv::Mat, cv::Mat, int);
    bool            surfMatch(cv::Mat,cv::Mat,int);

    FEATURE_ALG             type_;
	std::vector<cv::Point2f> k1_;
	std::vector<cv::Point2f> k2_;
	std::vector<float>       error_;

	friend class MatchThread;

};

#endif
