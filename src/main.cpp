

#include "mainWindow.h"
#include "imgFileMgr.h"
#include <QApplication>
#include <QSettings>
#include <QDesktopWidget>

int main(int argc, char* argv[])
{

    QCoreApplication::setApplicationName("ImgRes");

   // QGuiApplication::setDesktopSettingsAware(false);

    QApplication app(argc, argv);

    QFont font;
    font.setPointSize(10);
    font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
    app.setFont(font);

    ImgFileMgr::init();

    QString configName ("default_cfg.ini");
    QString configFileFullPath = ImgFileMgr::findFile(configName, ImgFileMgr::Flags(ImgFileMgr::Writable|ImgFileMgr::File));
    if (configFileFullPath.isEmpty())
    {
        configFileFullPath = ImgFileMgr::findFile(configName, ImgFileMgr::New);
        if (configFileFullPath.isEmpty())
            qFatal("Could not create configuration file %s.", qPrintable(configName));
    }

    QSettings* confSettings = Q_NULLPTR;
    if (ImgFileMgr::exists(configFileFullPath))
    {

    }

    QDesktopWidget* desktopWidget = QApplication::desktop();
    QRect screenRect = desktopWidget->screenGeometry();
    int screenX = screenRect.width();
    int screenY = screenRect.height();

    int adjust = 400;
    int mainWW = screenX - adjust;
    int mainWH = screenY - (screenY * 1.0/ screenX) * adjust;

    ImgResMainWindow mainWin(confSettings,mainWW,mainWH);
    mainWin.setWindowIcon(QIcon(":/mainWindow/icons/128x128/spot.png"));

    if(!mainWin.init())
    {
        return -1;
    }

    mainWin.move(100,100);
    mainWin.show();

    app.exec();

   // delete confSettings;

    return 0;
}

