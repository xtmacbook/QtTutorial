
#ifndef PERSON_H
#define PERSON_H

#include <QGraphicsItem>
#include <QGraphicsWidget>

class TextWidget;
class LabelItem : public QGraphicsWidget
{
	Q_OBJECT
public:
    LabelItem(QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());
	LabelItem(QString tile,QGraphicsItem *parent = Q_NULLPTR, Qt::WindowFlags wFlags = Qt::WindowFlags());
	void create(const QString title);

	void updateLabelText(const QString& text);
	QString getLabelText()const;
private:
	TextWidget *title_;
};
 

#endif
