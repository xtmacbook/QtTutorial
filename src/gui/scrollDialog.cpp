
#include "scrollDialog.h"
#include "ui_scrollDialog.h"
#include "selRect.h"
#include "imgScene.h"
#include "imgMgr.h"

#include <QLabel>
#include <QDrag>
#include <QMimeData>
#include <QByteArray>

ScrollDialog::ScrollDialog(QWidget* parent):
       QDialog(parent),
       ui_(new Ui::scrollDialogForm),
	 selRect_(nullptr)
{
    ui_->setupUi(this);

	Qt::WindowFlags flags = this->windowFlags();
	setWindowFlags(flags&~Qt::WindowContextHelpButtonHint);
	setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint);
	setFixedSize(this->width(), this->height());

	selRect_ = new SelRect(ui_->scrollGraphicView,this);
	selRect_->setSize(64, 64);
	ui_->gridLayout->addWidget(selRect_, 0, 0, 1, 1);
}

ScrollDialog::~ScrollDialog()
{
	if (selRect_)
		delete selRect_;
}

void ScrollDialog::setScene(QGraphicsScene *scene)
{
    ui_->scrollGraphicView->setScene(scene);
}
 
void ScrollDialog::resizeView()
{
	QSize size = this->size();

	QGraphicsView * view = ui_->scrollGraphicView;
	const QGraphicsScene * scene = view->scene();
	const GraphicScene * s = (const GraphicScene*)(scene);
	const QGraphicsItem * imgItem = s->getImgItem();
	if (imgItem)
	{
		QRect rect = imgItem->boundingRect().toRect();
		qreal scale = (size.width() * 1.0) / rect.width();
		view->setSceneRect(-size.width() / (2 * 1.0), -size.height() / (2 * 1.0), size.width() - 2, size.height() - 2);
		view->resetMatrix();
		view->scale(scale, scale);
		view->centerOn(imgItem);
		view->update();
	}
	if(selRect_) selRect_->repos();
}

void ScrollDialog::reposSelRect()
{
    if (selRect_) selRect_->repos();
}

void ScrollDialog::clearViewparams()
{
   ui_->scrollGraphicView->resetMatrix();
   ui_->scrollGraphicView->setSceneRect(0, 0, 0, 0);
}

ViewPort * ScrollDialog::getView()
{
	return ui_->scrollGraphicView;
}

void ScrollDialog::constraintSize(bool f)
{
	if (f) setFixedSize(this->width(), this->height());
    else setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
}

void ScrollDialog::mainSelRect(SelRect * ms)
{
    selRect_->setParent(ms);
    selRect_->setChild(ms);
}

void ScrollDialog::resizeEvent(QResizeEvent* sz)
{
	QDialog::resizeEvent(sz);
	resizeView();
}
 
