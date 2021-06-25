#ifndef SELRECT_H
#define SELRECT_H
#include <QGraphicsView>
#include <QFrame>
#include <QLabel>
#include "viewPort.h"

class SelRect : public QLabel
{
    Q_OBJECT
public:
    SelRect(ViewPort* view, QWidget* parent = Q_NULLPTR);

    void        setSize(qreal w, qreal y);

    void        repos();
    void        setParent(SelRect*);
    void        setChild(SelRect*);

    SelRect*    child();

    ViewPort*   getView();

    QPoint      sceneCenterPos()const;

    static void reLocationChild(SelRect*,const QRect);
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
private:
    void			clip(QRect&);

private:
    QPoint			oldPos_;
    ViewPort*		viewPort_ = nullptr;

    SelRect*        parent_= nullptr;
    SelRect*        child_= nullptr;
};


#endif // !SELRECT_H
