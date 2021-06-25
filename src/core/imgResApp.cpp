

#include "imgResApp.h"

ImgResApp* ImgResApp::singleton_ = Q_NULLPTR;

ImgResApp::ImgResApp():
    QObject(Q_NULLPTR),
    //mainWin_(Q_NULLPTR),
    initialized_(false)
{
    setObjectName("ImgResApp");

    Q_ASSERT(!singleton_);
	singleton_ = this;
}

ImgResApp::~ImgResApp()
{

}

void ImgResApp::init(QSettings* conf)
{
    confSettings_ = conf;
}

void ImgResApp::deinit()
{

}


