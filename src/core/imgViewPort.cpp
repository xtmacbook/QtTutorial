#include "imgViewPort.h"
#include "imgScene.h"
#include "mainWindow.h"

#include <QMouseEvent>
#include <QPixmap> 
#include <QTimer>
#include <QMenu>
#include <QApplication>
#include <QDragMoveEvent>
GraphicsView::GraphicsView(QWidget *parent):
    QGraphicsView(parent)
{
	self_ = this;
   // setDragMode(QGraphicsView::NoDrag);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setCursor(Qt::CrossCursor);
    setRenderHint(QPainter::Antialiasing);

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(update()));
	timer->start(10);
}


void GraphicsView::adjustCenterFromMousePosition(QPoint &mousePos)
{
    QWidget *view = viewport();
    QPointF viewCenterScenePos = mapToScene(view->rect().center());
    QPointF mouseScenePos = mapToScene(view->mapFromGlobal(mousePos));
    QPointF diff = viewCenterScenePos - mouseScenePos;
    //centerOn(mLastMouseScenePos + diff);
}

QRect GraphicsView::viewToScene(const QRect sr)
{

	QPointF sceneTopLeft = mapToScene(sr.topLeft());
	QPointF sceneBR = mapToScene(sr.bottomRight());
	return QRectF(sceneTopLeft, sceneBR).toRect();
}

void GraphicsView::wheelEvent(QWheelEvent *)  
{
    return;
}

ScrollView::ScrollView(QWidget *parent):
    QGraphicsView(parent)
{
	self_ = this;
	/*setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);*/
	setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}

ScrollView::~ScrollView()
{
	 
}

void ScrollView::wheelEvent(QWheelEvent *) 
{
	return;
}
 
 QRect ScrollView::viewToScene(const QRect sr)
 {
	 QPointF sceneTopLeft = mapToScene(sr.topLeft());
	 QPointF sceneBR = mapToScene(sr.bottomRight());
	 return QRectF(sceneTopLeft, sceneBR).toRect();
 }

 
ZoomView::ZoomView(QWidget *parent):
    QGraphicsView(parent)
{
	self_ = this;

    contextMenu_ = new QMenu;
    addPoint_ = new QAction(QApplication::translate("zoomViwe", "\346\267\273\345\212\240\347\202\271", Q_NULLPTR), this);
    contextMenu_->addAction(addPoint_);
    //connect(this, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(showListWidgetMenuSlot(QPoint)));
}

QRect ZoomView::fullViewSceneRect()
{
	 
	if (true)
	{
		QRect geometry(0, 0, 202, 204);

		const QGraphicsScene * scene = self_->scene();
		const GraphicScene * scene_ = (const GraphicScene *)(scene);
		const QGraphicsItem * img = scene_->getImgItem();

		QPolygonF polygon = this->mapToScene(geometry);
		QPolygonF itemPolygon = img->mapToScene(img->boundingRect());
		QPolygonF viewPolygon = polygon.intersected(itemPolygon);
		const QRectF result = viewPolygon.boundingRect();

		return result.toRect();
	}
	else
	{
		return ViewPort::fullViewSceneRect();
	}
}

void ZoomView::vTranslate(QPoint scenePos)
{
    ViewPort::vTranslate(scenePos);
}

QAction *ZoomView::addAction()
{
    return addPoint_;
}

QPoint ZoomView::contextPoint()
{
    return context_Cusor_Pos_;
}

//void ZoomView::showListWidgetMenuSlot(QPoint pos)
//{
//    contextMenu_->exec(QCursor::pos());

//    QPoint scenePos = mapToScene(pos).toPoint();
//    ImgResMainWindow* mainw = (ImgResMainWindow*) (ImgResMainWindow::getMainWindow());
//    if(mainw)
//        context_Cusor_Pos_ = mainw->scene2Img(scenePos,this);
//}

void ZoomView::wheelEvent(QWheelEvent *)
{
    return;
}

void ZoomView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);

    QPoint local_pos = event->pos();
    QPoint scenePos = mapToScene(local_pos).toPoint();
    ImgResMainWindow* mainw = (ImgResMainWindow*) (ImgResMainWindow::getMainWindow());
    if(mainw)
    {
        QPoint imgPos = mainw->scene2Img(scenePos,this);

        emit mouseMove(imgPos);
    }
}

void ZoomView::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint cur_pos = cursor().pos();
    QPoint local_pos = mapFromGlobal(cur_pos);
    QPoint scenePos = mapToScene(local_pos).toPoint();
    ImgResMainWindow* mainw = (ImgResMainWindow*) (ImgResMainWindow::getMainWindow());
    if(mainw)
        context_Cusor_Pos_ = mainw->scene2Img(scenePos,this);

    contextMenu_->move(cursor().pos()); //让菜单显示的位置在鼠标的坐标上
    contextMenu_->show();

}

