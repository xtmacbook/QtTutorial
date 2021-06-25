
#ifndef IMCONFIG_H
#define IMCONFIG_H

#include <QtCore/QtCore>

class Config
{
private:
	static Config * local_instance;

	Config() {}
	virtual ~Config();
public:

	static  Config* getInstance();
	
	void	setScollWindow(qreal, qreal);
	//void	setZoomWindow(qreal, qreal);

	void   getScollWindow(qreal&,qreal&)const ;
private:
 
	qreal scollDW_;
	qreal scollDH_;
};

#endif