
#ifndef MATCHTHREAD_H
#define MATCHTHREAD_H

#include "feature.h"

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
 
class MatchThread : public QThread
{
	Q_OBJECT

public:
	MatchThread(QObject *parent = 0);

    virtual ~MatchThread();

    void match(int match_operator,int);

	void getMatcher(QList<qreal>&);

    void getMatcherOperator(QStringList&);
Q_SIGNALS:
	void overMatch();

protected:
	void run() Q_DECL_OVERRIDE;

private:
    QMutex      mutex_;
	Feature		feature_;
    int         tiles_num_;
}; 

#endif
