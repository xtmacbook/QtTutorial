#include "imgConfig.h"

Config * Config::local_instance = nullptr;
Config::~Config()
{

}

Config* Config::getInstance()
{
	if (local_instance == nullptr)
	{
		local_instance = new Config();
	}
	return local_instance;
}

void Config::setScollWindow(qreal x, qreal y)
{
	
}

void Config::getScollWindow(qreal&x, qreal&y) const
{
	x = 300;
	y = 400;
}
