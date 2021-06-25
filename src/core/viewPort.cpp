
#include "viewPort.h"
#include "imgScene.h"
#include <QGraphicsItem>
#include <QRect>

QRect ViewPort::fullViewSceneRect()
{
	QRect geometry = self_->viewport()->rect();
	const QGraphicsScene * scene = self_->scene();
	const GraphicScene * scene_ = (const GraphicScene *)(scene);
	const QGraphicsItem * img = scene_->getImgItem();

	QPolygonF polygon = self_->mapToScene(geometry);

	QPolygonF itemPolygon = img->mapToScene(img->boundingRect());

	QPolygonF viewPolygon = polygon.intersected(itemPolygon);

	const QRectF result = viewPolygon.boundingRect();

	return result.toRect();
}

QRect ViewPort::sceneToView(const QRect sr)
{
	QPoint viewTopLeft = self_->mapFromScene(sr.topLeft());
	QPoint viewBR = self_->mapFromScene(sr.bottomRight());

	return QRect(viewTopLeft, viewBR);
}

QRect ViewPort::viewToScene(const QRect)
{
	QRect t;
	return t;
}

void  ViewPort::drag(const QRect&sceneRect)
{
	vTranslate(sceneRect.center());
	self_->update();  
}

void ViewPort::vTranslate(QPoint scenePos)
{
	QTransform vTransform(self_->transform());

	QPointF oldCenter = self_->mapToScene(self_->viewport()->rect().center());
	QPointF mouseDelta = scenePos - oldCenter;
	self_->setTransformationAnchor(QGraphicsView::NoAnchor);
	self_->setTransform(vTransform.translate(mouseDelta.x(), mouseDelta.y()));
	self_->setTransformationAnchor(QGraphicsView::AnchorViewCenter);

    self_->centerOn(oldCenter + mouseDelta);
}

