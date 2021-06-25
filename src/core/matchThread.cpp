#include "matchThread.h"

#include <QtWidgets>
#include <cmath>

//! [0]
MatchThread::MatchThread(QObject *parent)
    : QThread(parent)
{

}

MatchThread::~MatchThread()
{
    wait();
}

void MatchThread::match(int match_operator,int tile_Num)
{
    QMutexLocker locker(&mutex_);
    tiles_num_ = tile_Num;

	feature_.setType((FEATURE_ALG)match_operator);

    if (!isRunning())
    {
        start(HighestPriority);
    }
}

void MatchThread::getMatcher(QList<qreal>&data)
{
    const std::vector<cv::Point2f>& k1 = feature_.k1_;
    const std::vector<cv::Point2f>& k2 = feature_.k2_;
	const std::vector<float>& error_ = feature_.error_;

    QStringList dataList;
    for (int i = 0; i < k1.size(); i++)
    {
        data.append(k1[i].x);
        data.append(k1[i].y);

        data.append(k2[i].x);
        data.append(k2[i].y);

		data.append(error_[i]);
    }
}

void MatchThread::getMatcherOperator(QStringList &operators)
{
    int sum = FEATURE_ALG::NONE;

    for(int i = 0;i < sum;i++)
        operators.append(feature_.types((FEATURE_ALG)i));
}

void MatchThread::run()
{
    feature_.match(tiles_num_);
    emit overMatch();
}
