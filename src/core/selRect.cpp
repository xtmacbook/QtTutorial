#include "selRect.h"
#include "imgMgr.h"
#include "viewPort.h"

#include <QGraphicsView>
#include <QMouseEvent>

SelRect::SelRect(ViewPort* view, QWidget* parent):QLabel(parent)
    ,viewPort_(view)
{
	setFrameStyle(Box);
	setStyleSheet("border-width: 1px;border-style: solid;border-color: rgb(255, 20, 0);");
	setLineWidth(2);
}

void SelRect::setSize(qreal w, qreal h)
{
	setMaximumSize((int)w, (int)h);
}

void SelRect::repos()
{
    ViewPort * parent = parent_->getView();
    if (parent != nullptr)
	{
        QRect viewInSceneRect = parent->fullViewSceneRect();
		
        QRect viewRect = viewPort_->sceneToView(viewInSceneRect);
		 
		setSize(viewRect.width(), viewRect.height());
		
		move(viewRect.topLeft());
    }
}

void SelRect::setParent(SelRect * p)
{
    parent_ = p;
}

void SelRect::setChild(SelRect *c)
{
    child_ = c;
}

SelRect *SelRect::child()
{
    return child_;
}

ViewPort *SelRect::getView()
{
    return viewPort_;
}

QPoint SelRect::sceneCenterPos() const
{
    QPoint scneCenter =  viewPort_->viewToScene(geometry()).center();

    return scneCenter;
}

void SelRect::reLocationChild(SelRect *sel, const QRect viewRect)
{
    ViewPort * vp = sel->getView();
    QRect sr =  vp->viewToScene(viewRect);

    if(sel->child())
    {
        sel->child()->getView()->drag(sr);
        reLocationChild(sel->child(),sel->child()->geometry());
    }

}

void SelRect::mousePressEvent(QMouseEvent *event)
{
	QLabel::mousePressEvent(event);

	oldPos_ = event->pos();
}

void SelRect::mouseReleaseEvent(QMouseEvent *event)
{
	const QPoint delta = event->pos() - oldPos_;

	QPoint newPoint(pos() + delta);

	QRect rect(newPoint, size());

	clip(rect);

	move(rect.topLeft());

    if (child_ && child_->getView())
	{
        reLocationChild(this,rect);
//        QRect sr =  viewPort_->viewToScene(rect);
//        child_->getView()->drag(sr);
	}

	QLabel::mouseReleaseEvent(event);
}

void SelRect::clip(QRect& rect)
{
    if (viewPort_)
	{
        QRect viewRect = viewPort_->sceneToView(viewPort_->fullViewSceneRect());

		if (viewRect.contains(rect))
			return;

		if (rect.contains(viewRect))
		{
			rect = viewRect;
			return;
		}

		if (viewRect.intersects(rect))
		{
			if (rect.left() < viewRect.left())		rect.moveLeft(viewRect.left());
			if (rect.right() > viewRect.right())	rect.moveRight(viewRect.right());
			if (rect.top() < viewRect.top())		rect.moveTop(viewRect.top());
			if (rect.bottom() > viewRect.bottom())	rect.moveBottom(viewRect.bottom());
		}
		else
		{
			rect = viewRect;
		}
	}
}
