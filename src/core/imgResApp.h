
#ifndef IMGRESAPP_H
#define IMGRESAPP_H

//! Singleton main ImgRes application class.

#include <QString>
#include <QObject>
#include <QSettings>

class ImgResGuiBase;
//class ImgResMainView;

class ImgResApp : public QObject
{
    Q_OBJECT
public:
	ImgResApp(/*ImgResMainView* parent*/);

	virtual ~ImgResApp();

    void init(QSettings * conf);

    void deinit();

	static ImgResApp& getInstance() {Q_ASSERT(singleton_); return *singleton_;}

    void setGui(ImgResGuiBase*) {}

    QSettings* getSettings() const {return confSettings_;}

private:

    // The ImgResGuiBase singleton
	static ImgResApp* singleton_;

   // ImgResMainView*   mainWin_;

	QSettings* confSettings_;

	bool initialized_;

};

#endif
