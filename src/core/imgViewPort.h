
#ifndef IMGVIEWPORT_H
#define IMGVIEWPORT_H

#include "imgType.h"
#include "selRect.h"
#include "viewPort.h"

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

#include <opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>

class GraphicsView : public QGraphicsView, public  ViewPort
{
	Q_OBJECT
public:
	GraphicsView(QWidget *parent = Q_NULLPTR);
protected:
	void adjustCenterFromMousePosition(QPoint &mousePos);
	virtual QRect viewToScene(const QRect);
protected:
	void wheelEvent(QWheelEvent *) Q_DECL_OVERRIDE;
private:


};

class ScrollView :public QGraphicsView, public  ViewPort
{
	Q_OBJECT
public:
	ScrollView(QWidget *parent = Q_NULLPTR);

	virtual ~ScrollView();
	virtual QRect viewToScene(const QRect);
protected:
	void wheelEvent(QWheelEvent *) Q_DECL_OVERRIDE;
private:
	QRect			selRect_;
};


class ZoomView :public QGraphicsView, public ViewPort
{
	Q_OBJECT
public:
	ZoomView(QWidget *parent = Q_NULLPTR);

	virtual QRect fullViewSceneRect();
	virtual void vTranslate(QPoint scenePos);

    QAction*       addAction();
    QPoint         contextPoint();
Q_SIGNALS:
    void        mouseMove(QPoint);
private Q_SLOTS:
   // void showListWidgetMenuSlot(QPoint pos);
protected:
	void wheelEvent(QWheelEvent *) Q_DECL_OVERRIDE;
    virtual void  mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

    QMenu *         contextMenu_;
    QAction*        addPoint_;

    QPoint          context_Cusor_Pos_;
};

#endif
