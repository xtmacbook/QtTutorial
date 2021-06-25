

#include "labelItem.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsAnchorLayout>
#include <QGraphicsWidget> 
#include <QGraphicsSimpleTextItem>
#include <math.h>


class TextWidget : public QGraphicsLayoutItem
{

public:
	TextWidget( QString text):
		 QGraphicsLayoutItem()
	{
		original_ = new QGraphicsSimpleTextItem();
		original_->setBrush(Qt::green);
		original_->setText(text);

		QFont font("Microsoft YaHei", 12);
		font.setBold(true);
		original_->setFont(font);
		setGraphicsItem(original_);
		original_->show();

		rect_ = QRectF(QPoint(0, 0), original_->boundingRect().size());
	}

	~TextWidget()
	{
		setGraphicsItem(0);
		delete original_;
	}

	void setZValue(qreal z)
	{
		original_->setZValue(z);
	}

	void updateLabelText(const QString& text)
	{
		original_->setText(text);
	}
	QString getLabelText()const
	{
		return original_->text();
	}
	/*void setGeometry(const QRectF &rect) Q_DECL_OVERRIDE
	{
		original_->setTransform(QTransform::fromScale(rect.width() / rect_.width(),
			rect.height() / rect_.height()), true);
		original_->setPos(rect.x(), rect.y());
		rect_ = rect;
	}*/

protected:
	QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const Q_DECL_OVERRIDE
	{
		Q_UNUSED(constraint);
		QSizeF sh;
		switch (which) {
		case Qt::MinimumSize:
			sh = QSizeF(2, 2);
			break;
		case Qt::PreferredSize:
			sh = QSizeF(6, 6);
			break;
		case Qt::MaximumSize:
			sh = QSizeF(32, 32);
			break;
		default:
			break;
		}
		return sh;
	}

private:
	QGraphicsSimpleTextItem *original_;
	QRectF rect_;
};

class CirclePicItem : public QGraphicsItem
{
public:
	CirclePicItem(int);

	QRectF boundingRect() const Q_DECL_OVERRIDE;
	QPainterPath shape() const Q_DECL_OVERRIDE;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
		QWidget *widget) Q_DECL_OVERRIDE;

private:
	int radio_;
};

CirclePicItem::CirclePicItem(int radio):
	radio_(radio)
{
}

QRectF CirclePicItem::boundingRect() const
{
	QPainterPath path;
	path.addEllipse(QPoint(0,0), radio_, radio_);

	return path.boundingRect();
}

QPainterPath CirclePicItem::shape() const
{
	QPainterPath path;
	path.addEllipse(QPoint(0, 0), radio_, radio_);
	return path;
}

void CirclePicItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
	QPainterPath OuterPath;
	OuterPath.addEllipse(QPoint(0, 0), radio_, radio_);
	QPainterPath InnerPath;
	InnerPath.addEllipse(QPoint(0, 0), radio_ * 0.8, radio_ * 0.8);
	QPainterPath FillPath = OuterPath.subtracted(InnerPath);
	painter->setRenderHint(QPainter::Antialiasing);
	painter->fillPath(FillPath, Qt::red);
	painter->strokePath(OuterPath.simplified(), QPen(Qt::red, 1));
	painter->strokePath(InnerPath, QPen(Qt::red, 1));
}


class CirclePicWidget : public QGraphicsLayoutItem
{

public:
	CirclePicWidget(int radio)
		: QGraphicsLayoutItem()
	{
		original_ = new CirclePicItem(radio);
		setGraphicsItem(original_);
		original_->show();
		rect_ = QRect(0,0, 2* radio, 2 * radio);
	}

	~CirclePicWidget()
	{
		setGraphicsItem(0);
		delete original_;
	}

	void setZValue(qreal z)
	{
		original_->setZValue(z);
	}

	void setGeometry(const QRectF &rect) Q_DECL_OVERRIDE
	{
		original_->setTransform(QTransform::fromScale(rect.width() / rect_.width(),
			rect.height() / rect_.height()), true);
		original_->setPos(rect.x(), rect.y());
		rect_ = rect.toRect();
	}

protected:
	QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const Q_DECL_OVERRIDE
	{
		Q_UNUSED(constraint);
		QSizeF sh;
		switch (which) {
		case Qt::MinimumSize:
			sh = QSizeF(8, 8);
			break;
		case Qt::PreferredSize:
			sh = QSizeF(rect_.width(), rect_.height());
			break;
		case Qt::MaximumSize:
			sh = QSizeF(64, 64);
			break;
		default:
			break;
		}
		return sh;
	}

private:
	CirclePicItem *original_;
	QRect rect_;
};
 
 
LabelItem::LabelItem(QGraphicsItem *parent,Qt::WindowFlags wFlags )
    :QGraphicsWidget(parent,wFlags)
{
	setMinimumSize(8, 8);
	setPreferredSize(32, 32);
}


LabelItem::LabelItem(QString tile, QGraphicsItem *parent, Qt::WindowFlags wFlags):
	QGraphicsWidget(parent, Qt::Widget)
{
	setMinimumSize(8, 8);
	setPreferredSize(64, 64);
	create(tile);
}

void LabelItem::create(const QString text)
{
	title_ = new TextWidget(text);
	CirclePicWidget *circle = new CirclePicWidget(8);
	QGraphicsAnchorLayout *layout = new QGraphicsAnchorLayout;
	layout->setSpacing(0);
	setLayout(layout);

	layout->addCornerAnchors(circle, Qt::TopLeftCorner, layout, Qt::TopLeftCorner);
	title_->setZValue(1);
	layout->addAnchor(title_, Qt::AnchorLeft, circle, Qt::AnchorRight);
 	layout->addAnchor(title_, Qt::AnchorHorizontalCenter, circle, Qt::AnchorHorizontalCenter);

}

void LabelItem::updateLabelText(const QString& text)
{
	title_->updateLabelText(text);
}

QString LabelItem::getLabelText() const
{
	return title_->getLabelText();
}

