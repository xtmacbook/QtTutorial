#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QModelIndex>
#include <QGraphicsItem>
#include <opencv2/opencv.hpp>

namespace  Ui {
	class MainWindow;
}

class AutoGenTileDialog;
class ZoomDialog;
class ScrollDialog;
class TileSelDialog;
class ImgMgr;
class SelRect;
class GraphicScene;
class MatchTileShowDialog;
class TreeItem;
class LabelItem;
class QGraphicsView;

class ImgResMainWindow : public QMainWindow
{
	Q_OBJECT

public:
    ImgResMainWindow(QSettings*,int w,int h,QWidget *parent = Q_NULLPTR);

	virtual ~ImgResMainWindow();

public:
	void   enableBaseScollWindow(bool);
	void   enableWrapScollWindow(bool);

	void   enableBaseZoomWindow(bool);
	void   enableWrapZoomWindow(bool);

    void   getCurrentSelImgPos(double*,int);

    static QMainWindow* getMainWindow();
    QPoint img2Scene(const QPoint&,bool);
    QPoint scene2Img(const QPoint&,bool);
    QPoint scene2Img(const QPoint&,QGraphicsView*);
    QPoint scene2Img(const QPoint&,QGraphicsScene*);

    bool  init();

Q_SIGNALS:
	void	reposBaseSel();

private Q_SLOTS:
    //menu
	void openBaseImg();
	void openWrapImg();

	void autoRegistration();
	void manualRegistration();

    void exitApp();
    void help();
	void setting();

    //buttom
	void openBaseScrollWin();
	void openBaseZoomWin();
	void openWrapScrollWin();
	void openWrapZoomWin();

    //labelitem
    void centerToImg(const QGraphicsItem*,bool );
    void addLabelItemByRowInsert(TreeItem*);
	void updateLabelItem(TreeItem *, bool, QPoint);
    void visiableSpecialOne(bool,TreeItem*);
    void visiableAllMatchTiles(bool);
    void deleteLabelItemByRemoveRow(TreeItem*);
    void showMatchedTilesListDialog();


	void test();
private:

	void connectSS();
    void initmenuIcon();

    void initGrahicView(const QRect&, bool base = true);
	bool scrollWindViewSize(bool base = true);
	void zoomWindViewSize(bool base = true);

	bool loadImg(QString fileName, bool base = true);
	QGraphicsItem * creatImgItem(cv::Mat);

	void clearScene(bool base = true);

    QPoint getZoomDialogShowPos(bool);
private:
    Ui::MainWindow*                 ui_;

    AutoGenTileDialog*          genTileDialog_ = nullptr;
    TileSelDialog*              tileSelDialog_ = nullptr;
    MatchTileShowDialog*        matchTilesDialog_ = nullptr;

    ZoomDialog*                 baseZoomDialog_ = nullptr;
    ScrollDialog*               baseScroolDialog_ = nullptr;

    ZoomDialog*                 wrapZoomDialog_ = nullptr;
    ScrollDialog*               wrapScroolDialog_ = nullptr;

    GraphicScene*               baseScene_ = nullptr;
    GraphicScene*               wrapScene_ = nullptr;

    bool                        baseImg_ = false;
    bool                        wrapImg_ = false;

    float                       baseRatio_ = 1.0;
    float                       wratRatio_ = 1.0;

    SelRect *                   baseRectSel_;
    SelRect *                   wrapRectSel_;
};

#endif
