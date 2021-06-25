
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QRect>
#include <QGraphicsView>

class ViewPort 
{
public:
    virtual QRect fullViewSceneRect();

	virtual QRect sceneToView(const QRect);
	
	virtual QRect viewToScene(const QRect);

	virtual void  drag(const QRect&scene);

    virtual void vTranslate(QPoint scenePos);

protected:
    QGraphicsView*		self_ = nullptr;
};

#endif
