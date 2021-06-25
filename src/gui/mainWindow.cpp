

#include "mainWindow.h"
#include "ui_mainWindow.h"

#include "imgType.h"
#include "autoGenTileDialog.h"
#include "zoomDialog.h"
#include "scrollDialog.h"
#include "tileSelDialog.h"
#include "imgMgr.h"
#include "imgViewPort.h"
#include "imgConfig.h"
#include "imgScene.h"
#include "tilesListDialog.h"
#include "feature.h"
#include "labelItem.h" 
#include "treeView.h"
#include "treeitem.h"
#include "treemodel.h"
#include "imgScene.h"
#include <QGraphicsItem>

#include <QDrag>
#include <QFileDialog>
#include <QGraphicsItem>
#include <QDesktopWidget>
#include <QDebug>
#include <QMessageBox>


ImgResMainWindow::ImgResMainWindow(QSettings* settings,int w,int h,QWidget *parent):
    QMainWindow(parent),
    ui_(new Ui::MainWindow),
    genTileDialog_(nullptr),
    baseScene_(new GraphicScene()),
    wrapScene_(new GraphicScene()),
    baseRectSel_(nullptr),
    wrapRectSel_(nullptr)
{
    ui_->setupUi(this);

    //ui_->baseImgView->setBackgroundBrush(Qt::black);
    //ui_->wrapImgView->setBackgroundBrush(Qt::black);

    //window type
    Qt::WindowFlags flags = this->windowFlags();
    setWindowFlags(flags&~Qt::WindowContextHelpButtonHint);
    setWindowFlags(windowFlags()& ~Qt::WindowMinMaxButtonsHint);
    setFixedSize(w,h);
}

ImgResMainWindow::~ImgResMainWindow()
{
    DELETEPTR(baseScene_);
    DELETEPTR(wrapScene_);

    DELETEPTR(baseRectSel_);
    DELETEPTR(wrapRectSel_);

    DELETEPTR(genTileDialog_);
    DELETEPTR(tileSelDialog_);
    DELETEPTR(matchTilesDialog_);

    DELETEPTR(baseZoomDialog_);
    DELETEPTR(wrapZoomDialog_);
    DELETEPTR(baseScroolDialog_);
    DELETEPTR(wrapScroolDialog_);
}

void ImgResMainWindow::enableBaseScollWindow(bool flag)
{
    if (!flag && baseScroolDialog_&& !baseScroolDialog_->isHidden())
        baseScroolDialog_->hide();

    if (!flag) ui_->baseScrollWinBtn->setEnabled(false);

    if (flag) ui_->baseScrollWinBtn->setEnabled(true);
}

void ImgResMainWindow::enableWrapScollWindow(bool flag)
{
    if (!flag && wrapScroolDialog_&& !wrapScroolDialog_->isHidden())
        wrapScroolDialog_->hide();

    if (!flag) ui_->wrapScrollWinBtn->setEnabled(false);

    if(flag) ui_->wrapScrollWinBtn->setEnabled(true);
}

void ImgResMainWindow::enableBaseZoomWindow(bool flag)
{
    if (!flag && baseZoomDialog_ && !baseZoomDialog_->isHidden())
        baseZoomDialog_->hide();

    if (!flag) ui_->baseZoomWinBtn->setEnabled(false);

    if (flag) ui_->baseZoomWinBtn->setEnabled(true);

}

void ImgResMainWindow::enableWrapZoomWindow(bool flag)
{
    if (!flag && wrapZoomDialog_ && !wrapZoomDialog_->isHidden())
        wrapZoomDialog_->hide();

    if (!flag) ui_->wrapZoomWinBtn->setEnabled(false);

    if (flag) ui_->wrapZoomWinBtn->setEnabled(true);
}

void ImgResMainWindow::getCurrentSelImgPos(double *data, int flag)
{
    if(flag | 0x0001)
    {
        QPoint sc = baseRectSel_->sceneCenterPos();
        QPoint imgPos = scene2Img(sc,true);

        data[0] = imgPos.x();
        data[1] = imgPos.y();
    }
    if(flag | 0x0010)
    {
        QPoint sc = wrapRectSel_->sceneCenterPos();
        QPoint imgPos = scene2Img(sc,false);

        data[2] = imgPos.x();
        data[3] = imgPos.y();
    }
}

bool ImgResMainWindow::init()
{
    initmenuIcon();

    genTileDialog_	= new AutoGenTileDialog(this);

    baseZoomDialog_ = new ZoomDialog(this);
    baseZoomDialog_->setWindowTitle(QApplication::translate("ZoomDialog", "\345\237\272\345\207\206\345\233\276\345\203\217\347\274\251\346\224\276", Q_NULLPTR));
    baseScroolDialog_ = new ScrollDialog(this);

    wrapZoomDialog_ = new ZoomDialog(this);
    wrapZoomDialog_->setWindowTitle(QApplication::translate("ZoomDialog", "\345\276\205\351\205\215\345\207\206\345\233\276\345\203\217\347\274\251\346\224\276", Q_NULLPTR));
    wrapScroolDialog_ = new ScrollDialog(this);

    tileSelDialog_ = new TileSelDialog(this);
    matchTilesDialog_ = new MatchTileShowDialog(this);

    ui_->baseImgView->setScene(baseScene_);
    ui_->wrapImgView->setScene(wrapScene_);

    baseZoomDialog_->setScene(baseScene_);
    baseScroolDialog_->setScene(baseScene_);
    wrapZoomDialog_->setScene(wrapScene_);
    wrapScroolDialog_->setScene(wrapScene_);

    baseRectSel_ = new SelRect(ui_->baseImgView, ui_->base_frame);
    baseRectSel_->setSize(64, 64);
    ui_->gridLayout_4->addWidget(baseRectSel_, 0, 0, 1, 1);
    baseRectSel_->hide();
    baseScroolDialog_->mainSelRect(baseRectSel_);

    wrapRectSel_ = new SelRect(ui_->wrapImgView, ui_->wrap_frame);
    wrapRectSel_->setSize(64, 64);
    ui_->gridLayout_5->addWidget(wrapRectSel_, 0, 0, 1, 1);
    wrapRectSel_->hide();
    wrapScroolDialog_->mainSelRect(wrapRectSel_);

    baseZoomDialog_->mainSelRect(baseRectSel_);
    wrapZoomDialog_->mainSelRect(wrapRectSel_);
    baseZoomDialog_->resize(200, 200);
    wrapZoomDialog_->resize(200, 200);

    enableBaseScollWindow(false);
    enableWrapScollWindow(false);
    enableBaseZoomWindow(false);
    enableWrapZoomWindow(false);

    connectSS();

    return true;
}

void ImgResMainWindow::connectSS()
{
    connect(ui_->openBaseImageA,SIGNAL(triggered()), this, SLOT(openBaseImg()));
    connect(ui_->openWrapImageA,SIGNAL(triggered()), this, SLOT(openWrapImg()));
    connect(ui_->exitA,SIGNAL(triggered()),this,SLOT(exitApp()));
    connect(ui_->autoResA,SIGNAL(triggered()), this, SLOT(autoRegistration()));
    connect(ui_->manTileSelA,SIGNAL(triggered()), this, SLOT(manualRegistration()));
    connect(ui_->help_Action,SIGNAL(triggered()), this, SLOT(help()));

    connect(ui_->baseScrollWinBtn, SIGNAL(clicked()), this, SLOT(openBaseScrollWin()));
    connect(ui_->baseZoomWinBtn, SIGNAL(clicked()), this, SLOT(openBaseZoomWin()));
    connect(ui_->wrapScrollWinBtn, SIGNAL(clicked()), this, SLOT(openWrapScrollWin()));
    connect(ui_->wrapZoomWinBtn, SIGNAL(clicked()), this, SLOT(openWrapZoomWin()));

    connect(genTileDialog_, SIGNAL(dataBeGenerated(QList<qreal>)), matchTilesDialog_, SLOT(datasGenerated(QList<qreal>)));
    connect(genTileDialog_, SIGNAL(showTilesList()), matchTilesDialog_, SLOT(show()));

    connect(tileSelDialog_, SIGNAL(addTile(double*)), matchTilesDialog_, SLOT(addTiles(double*)));
    connect(tileSelDialog_, SIGNAL(deleteLastTile()), matchTilesDialog_, SLOT(delLastTiles()));
    connect(tileSelDialog_, SIGNAL(showTiles()), this, SLOT(showMatchedTilesListDialog()));

    connect(baseZoomDialog_->getView(), SIGNAL(mouseMove(QPoint)), tileSelDialog_, SLOT(real_update_base_point_pos(QPoint)));
    connect(wrapZoomDialog_->getView(), SIGNAL(mouseMove(QPoint)), tileSelDialog_, SLOT(real_update_base_point_pos(QPoint)));

    connect(baseZoomDialog_, SIGNAL(addSelPoint(QPoint)), tileSelDialog_, SLOT(addSelBasePoint(QPoint)));
    connect(wrapZoomDialog_, SIGNAL(addSelPoint(QPoint)), tileSelDialog_, SLOT(addSelWrapPoint(QPoint)));

    connect(matchTilesDialog_,SIGNAL(visiableAllTilesItem(bool)),this,SLOT(visiableAllMatchTiles(bool)));
    connect(matchTilesDialog_,SIGNAL(removeLabelItem(TreeItem*)),this,SLOT(deleteLabelItemByRemoveRow(TreeItem*)));
	connect(matchTilesDialog_, SIGNAL(addLabelItem(TreeItem*)), this, SLOT(addLabelItemByRowInsert(TreeItem*)));
	connect(matchTilesDialog_, SIGNAL(updateLabelItem(TreeItem*, bool, QPoint)), this, SLOT(updateLabelItem(TreeItem *, bool, QPoint)));


    TreeView* treeview =  (TreeView*)matchTilesDialog_->getTreeView();
    if (treeview)
    {
        connect(treeview, SIGNAL(locationToImgPoint(const QGraphicsItem*,bool)), this, SLOT(centerToImg(const QGraphicsItem*, bool)));
        connect(treeview->model(), SIGNAL(visiableSpecialOne(bool, TreeItem*)), this,
                SLOT(visiableSpecialOne(bool, TreeItem*)));
    }

#ifndef NDEBUG
    //test
    connect(ui_->actionTest_2, SIGNAL(triggered()), this, SLOT(test()));
    connect(this, SIGNAL(reposBaseSel()), this, SLOT(test()));
#else
#endif
}

void ImgResMainWindow::initmenuIcon()
{
   /* QIcon icon_openBI(":/menu/icons/document-open.png");
    ui_->openBaseImageA->setIcon(icon_openBI);
    ui_->openWrapImageA->setIcon(icon_openBI);
    QIcon exit_icon(":/menu/icons/application-exit.png");
    ui_->exitA->setIcon(exit_icon);
    QIcon sel_icon(":/menu/icons/selection.png");
    ui_->manTileSelA->setIcon(sel_icon);
    QIcon set_icon(":/menu/icons/preferences-system.png");
    ui_->set_action->setIcon(set_icon);
    QIcon help_icon(":/menu/icons/help.png");
    ui_->help_Action->setIcon(help_icon);
    QIcon auto_icon(":/menu/icons/auto.png");
    ui_->autoResA->setIcon(auto_icon);*/
}

void ImgResMainWindow::initGrahicView(const QRect&itemRect,bool base)
{
    if (base)
    {
        QRect viewRect = ui_->baseImgView->geometry();
        baseScene_->setSceneShowRect(viewRect,itemRect);
    }
    else
    {
        QRect viewRect = ui_->wrapImgView->geometry();
        wrapScene_->setSceneShowRect(viewRect,itemRect);
    }
}

bool ImgResMainWindow::scrollWindViewSize(bool base)
{

    QRect viewRect = (base) ? ui_->baseImgView->viewport()->rect() : ui_->wrapImgView->viewport()->rect();

    qreal w, h;
    w = viewRect.width();
    h = viewRect.height();

    const QGraphicsItem * item =  (base) ? baseScene_->getImgItem() : wrapScene_->getImgItem();

    QRect test = item->boundingRect().toRect();

    bool hideScrollWindow = ((item->boundingRect().height() < h) && item->boundingRect().width() < w);

    if (hideScrollWindow)
        (base) ? enableBaseScollWindow(false) : enableWrapScollWindow(false);
    else
        (base) ? enableBaseScollWindow(true) : enableWrapScollWindow(true);


    if (!hideScrollWindow)
    {
        Config::getInstance()->getScollWindow(w, h);
        float ratio = item->boundingRect().height() * 1.0 / item->boundingRect().width();

        int newChildW, newChildH;
        if (ratio > 1.0)
        {
            newChildH = h;
            newChildW = h / ratio;
        }
        else
        {
            newChildW = w;
            newChildH = w * ratio;
        }

        (base) ? baseScroolDialog_->constraintSize(false) : wrapScroolDialog_->constraintSize(false);
        (base) ? baseScroolDialog_->resize(newChildW, newChildH) : wrapScroolDialog_->resize(newChildW, newChildH);
        (base) ? baseScroolDialog_->resizeView() : wrapScroolDialog_->resizeView();
        (base) ? baseScroolDialog_->constraintSize(true) : wrapScroolDialog_->constraintSize(true);

    }

    return !hideScrollWindow;
}

void ImgResMainWindow::zoomWindViewSize(bool base /*= true*/)
{
    (base) ? enableBaseZoomWindow(true) : enableWrapZoomWindow(true);
    (base) ? baseZoomDialog_->resetViewSize() : wrapZoomDialog_->resetViewSize();
    (base) ? baseZoomDialog_->resizeView() : wrapZoomDialog_->resizeView();
}

bool ImgResMainWindow::loadImg(QString fileName, bool base /*= true*/)
{
    ImgMgr * mang = ImgMgr::getInstance();
    if (mang)
    {
        bool loadResult = false;
        if(base) loadResult = mang->loadBaseImg(fileName);
        else loadResult = mang->loadWrapImg(fileName);
        return loadResult;
    }

    return false;
}

QGraphicsItem * ImgResMainWindow::creatImgItem(cv::Mat img)
{
    QImage qImage = ImgMgr::mat2Qimage(img);
    QPixmap pixMap;
    pixMap = pixMap.fromImage(qImage);
    QGraphicsItem * pixItem = new QGraphicsPixmapItem(pixMap);
    return pixItem;
}

void ImgResMainWindow::clearScene(bool base /*= true*/)
{
    if (base)
    {
        baseScene_->clearScene();
        baseScene_->setSceneRect(0, 0, 0, 0);
        ui_->baseImgView->resetMatrix();
        ui_->baseImgView->setSceneRect(0, 0, 0, 0);

        baseScroolDialog_->clearViewparams();
        //QGraphicsView * view = (QGraphicsView*)baseZoomDialog_->getView();
        //if (view)
        {
            /*view->resetMatrix();
            view->setSceneRect(0, 0, 0, 0);*/
        }
    }

    else
        wrapScene_->clearScene();
}

void ImgResMainWindow::openBaseImg()
{
	const static QString fn = QApplication::translate("ImgResMainWindow", "\351\200\211\346\213\251\345\237\272\345\207\206\345\233\276\345\203\217!", Q_NULLPTR);

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    fn,
                                                    QDir::currentPath(),
                                                    tr("(*png *jpg *.bmp);;"
                                                       "(*txt)"));
    if(QFile::exists(fileName))
    {
        if (loadImg(fileName))
        {
            enableBaseScollWindow(false);
            enableBaseZoomWindow(false);
            clearScene(true);

            cv::Mat img = ImgMgr::getInstance()->getBaseImg();
            QGraphicsItem * item = creatImgItem(img);
            QRect itemRect = item->boundingRect().toRect();
            initGrahicView(itemRect, true);
            item->setPos(-itemRect.width() / 2.0, -itemRect.height() / 2.0);
            baseScene_->addImgItem(item);
            ui_->baseImgView->centerOn(item);

            scrollWindViewSize();
            zoomWindViewSize();

            baseZoomDialog_->move(getZoomDialogShowPos(true));
            baseZoomDialog_->show();

            QTimer::singleShot(500, [this]() {
                QRect sceneRect = baseZoomDialog_->getViewRect();
                QRect viewRect = this->ui_->baseImgView->sceneToView(sceneRect);
                this->baseRectSel_->setSize(viewRect.width(), viewRect.height());
                this->baseRectSel_->show();
                this->baseRectSel_->move(viewRect.topLeft());
            });
        }
    }

}

void ImgResMainWindow::openWrapImg()
{
	const static QString fn = QApplication::translate("ImgResMainWindow", "\351\200\211\346\213\251\345\276\205\351\205\215\345\207\206\345\233\276\345\203\217!", Q_NULLPTR);
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    fn,
                                                    QDir::currentPath(),
                                                    tr("图片文件(*png *jpg *bmp);;"
                                                       "本本文件(*txt)"));
    if(QFile::exists(fileName))
    {
        if (loadImg(fileName,false))
        {
            enableWrapScollWindow(false);
            enableWrapZoomWindow(false);
            clearScene(false);

            cv::Mat img = ImgMgr::getInstance()->getWrapImg();
            QGraphicsItem * item = creatImgItem(img);
            QRect itemRect = item->boundingRect().toRect();
            initGrahicView(itemRect, false);
            item->setPos(-itemRect.width() / 2.0, -itemRect.height() / 2.0);
            wrapScene_->addImgItem(item);
            ui_->wrapImgView->centerOn(item);

            scrollWindViewSize(false);
            zoomWindViewSize(false);
            wrapZoomDialog_->move(getZoomDialogShowPos(false));
            wrapZoomDialog_->show();

            QTimer::singleShot(500, [this]() {
                QRect sceneRect = wrapZoomDialog_->getViewRect();
                QRect viewRect = this->ui_->wrapImgView->sceneToView(sceneRect);
                this->wrapRectSel_->setSize(viewRect.width(), viewRect.height());
                this->wrapRectSel_->show();
                this->wrapRectSel_->move(viewRect.topLeft());
            });
        }
    }
}

void ImgResMainWindow::autoRegistration()
{
    genTileDialog_->show();
}

void ImgResMainWindow::manualRegistration()
{
    tileSelDialog_->show();
}

void ImgResMainWindow::exitApp()
{
    QApplication::quit();
}

void ImgResMainWindow::help()
{
	QString title = QApplication::translate("MainWindow", "\345\233\276\345\203\217\351\205\215\345\207\206", Q_NULLPTR);
    QString text = QApplication::translate("MainWindow", "\350\257\245\350\275\257\344\273\266\347\224\250\344\272\216\344\270\244\345\271\205\345\233\276\345\203\217\347\232\204\351\205\215\345\207\206\357\274\214\344\275\240\345\217\257\344\273\245\347\224\250\346\235\245\347\274\226\350\276\221\351\205\215\345\207\206\345\220\216\347\232\204\345\220\214\345\220\215\347\202\271\343\200\202\345\220\214\346\227\266\344\275\240\345\217\257\344\273\245\346\211\213\345\212\250\351\200\211\346\213\251\345\220\214\345\220\215\347\202\271\343\200\202\345\234\250\346\226\207\344\273\266\344\270\255\344\277\235\345\255\230\345\220\214\345\220\215\347\202\271\343\200\202", Q_NULLPTR);
    QMessageBox::about(this,title,text);
}

void ImgResMainWindow::setting()
{
}


void ImgResMainWindow::openBaseScrollWin()
{
    baseScroolDialog_->show();
}

void ImgResMainWindow::openBaseZoomWin()
{
    baseZoomDialog_->show();
}

void ImgResMainWindow::openWrapScrollWin()
{
    wrapScroolDialog_->show();
}

void ImgResMainWindow::openWrapZoomWin()
{
    wrapZoomDialog_->show();
}

void ImgResMainWindow::centerToImg(const QGraphicsItem*item,bool base)
{
    (base)?ui_->baseImgView->centerOn(item) :ui_->wrapImgView->centerOn(item);
} 

void ImgResMainWindow::addLabelItemByRowInsert(TreeItem* item)
{
    QGraphicsItem * baseItem = item->getBaseLabelItem();
    QGraphicsItem * wrapItem = item->getWrapLabelItem();

    qreal b_x = item->data(2).toDouble();
    qreal b_y = item->data(3).toDouble();

    qreal w_x = item->data(4).toDouble();
    qreal w_y = item->data(5).toDouble();

	QPoint off(-8, -8);
    QPoint scenePoint = img2Scene(QPoint(b_x,b_y),true);
    baseItem->setPos(scenePoint + off);
    baseScene_->addLabelItem(baseItem);

    qDebug() << b_x << " " << b_y << " base scene  point is :" << scenePoint.x() << " " << scenePoint.y() << "\n";
    scenePoint = img2Scene(QPoint(w_x,w_y),false);
    wrapItem->setPos(scenePoint + off);
    wrapScene_->addLabelItem(wrapItem);
    qDebug() << w_x << " " << w_y << " wrap scene  point is :" << scenePoint.x() << " " << scenePoint.y() << "\n";
}

void ImgResMainWindow::updateLabelItem(TreeItem *item, bool base, QPoint point)
{
	QGraphicsItem * labelItem = (base) ? item->getBaseLabelItem() : item->getWrapLabelItem();

	qreal x = point.x();
	qreal y = point.y();

	QPoint off(-8, -8);
	QPoint scenePoint = img2Scene(QPoint(x, y), base);
	labelItem->setPos(scenePoint + off);
}

void ImgResMainWindow::visiableSpecialOne(bool visiable, TreeItem *item)
{
    QGraphicsItem * baseItem = item->getBaseLabelItem();
    QGraphicsItem * wrapItem = item->getWrapLabelItem();

    if(baseItem)
        (visiable) ? baseItem->show():baseItem->hide();
    if(wrapItem)
        (visiable) ? wrapItem->show():wrapItem->hide();
}

void ImgResMainWindow::visiableAllMatchTiles(bool show)
{
    baseScene_->showLabelItem(show);
    wrapScene_->showLabelItem(show);
}

void ImgResMainWindow::deleteLabelItemByRemoveRow(TreeItem *item)
{
    QGraphicsItem * baseItem = item->getBaseLabelItem();
    QGraphicsItem * wrapItem = item->getWrapLabelItem();

    if(baseItem)
        baseScene_->removeLabelItem(baseItem);
    if(wrapItem)
        wrapScene_->removeLabelItem(wrapItem);
}

void ImgResMainWindow::showMatchedTilesListDialog()
{
    matchTilesDialog_->show();
}

QPoint ImgResMainWindow::getZoomDialogShowPos(bool base)
{
    QObject * p = (base)? baseZoomDialog_->parent() : wrapZoomDialog_->parent();
    QWidget * parent =(QWidget*)p;
    QPoint point;
    if(parent)
    {
        QPoint pos = parent->pos();
        int pw = parent->width();
        int ph = parent->height();
        point.setX(pos.x() + pw + 10);
        (base)? point.setY(pos.y()):point.setY(pos.y() + 250);
        //point = mapFromGlobal(point);
    }

    return point;
}

QPoint ImgResMainWindow::img2Scene(const QPoint& imgP,bool base)
{
    QGraphicsItem *imgItem = (base) ? baseScene_->getImgItem():wrapScene_->getImgItem();

    if (imgItem)
    {
        qreal w = imgItem->boundingRect().width();
        qreal h = imgItem->boundingRect().height();

        return QPoint(imgP.x() - w / 2.0, imgP.y() - h / 2.0);
    }

    return imgP;
}

QPoint ImgResMainWindow::scene2Img(const QPoint& sceneP,bool base)
{
    return (base)?scene2Img(sceneP,baseScene_):scene2Img(sceneP,wrapScene_);
}

QPoint ImgResMainWindow::scene2Img(const QPoint &point, QGraphicsView *view)
{
    return scene2Img(point,view->scene());
}

QPoint ImgResMainWindow::scene2Img(const QPoint &sceneP, QGraphicsScene *s)
{
    GraphicScene * scene = (GraphicScene*)(s);

    QGraphicsItem *imgItem = scene->getImgItem();
    if (imgItem)
    {
        qreal w = imgItem->boundingRect().width();
        qreal h = imgItem->boundingRect().height();

        return QPoint(sceneP.x() + w / 2.0, sceneP.y() + h / 2.0);
    }

    return sceneP;
}

QMainWindow* ImgResMainWindow::getMainWindow()
{
    foreach (QWidget *w, qApp->topLevelWidgets())
        if (QMainWindow* mainWin = qobject_cast<QMainWindow*>(w))
            return mainWin;
    return nullptr;
}

void ImgResMainWindow::test()
{
    QPoint thisPos = mapToGlobal(this->pos());

	qreal b_x = 224.24881;
	qreal b_y = 273.11362;

	qreal w_x = 548.633544;
	qreal w_y = 245.77246;

	QGraphicsItem * baseItem = new LabelItem("1");
	QGraphicsItem * wrapItem = new LabelItem("2");

	QPoint scenePoint = img2Scene(QPoint(b_x, b_y), true);
	baseItem->setPos(-8,-8);
	baseScene_->addLabelItem(baseItem);

	qDebug() << b_x << " " << b_y << " base scene  point is :" << scenePoint.x() << " " << scenePoint.y() << "\n";
	scenePoint = img2Scene(QPoint(w_x, w_y), false);
	wrapItem->setPos(scenePoint);
	wrapScene_->addLabelItem(wrapItem);
	qDebug() << w_x << " " << w_y << " wrap scene  point is :" << scenePoint.x() << " " << scenePoint.y() << "\n";


    int a = 3;
} 
