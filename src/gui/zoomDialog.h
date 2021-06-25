
#ifndef IR_ZOOMDIALOG_H
#define IR_ZOOMDIALOG_H

#include <QMainWindow>
#include <QLabel>
#include <QDialog>
#include <QGraphicsView>
#include <opencv2/opencv.hpp>

namespace Ui {
    class Dialog;
} // namespace Ui

class ZoomDialog;
class ViewPort;
class SelRect;
class ZoomView;

enum ZOOMTYPE
{
	MAX_ZOOM,
	MIN_ZOOM,
	DEFAULT
};

class ZoomRect : public QLabel
{
	Q_OBJECT
public:
	ZoomRect(QWidget*p, ZoomDialog*, ZOOMTYPE type = DEFAULT);

	virtual ~ZoomRect();

Q_SIGNALS:
	void	maxView();
	void	minView();
	void	defaultView();

protected:
	void paintEvent(QPaintEvent *)Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *ev) Q_DECL_OVERRIDE;
private:
	ZOOMTYPE			type_;

	ZoomDialog*			parent_;
};


class ZoomDialog : public QDialog
{
    Q_OBJECT

public:
    ZoomDialog(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~ZoomDialog();

    void		setScene(QGraphicsScene*);
	void		resizeView();
    ZoomView *	getView();
	void		resetViewSize();
	QRect		getViewRect()const;
    void        mainSelRect(SelRect*);

 Q_SIGNALS:
    void        addSelPoint(QPoint);
protected:
	virtual void showEvent(QShowEvent *);
	virtual void resizeEvent(QResizeEvent *event);

private slots:
	void maxScale();
	void minScale();
	void dMaxScale();
	void dMinScale();
    void defScale();
    void addPoint();
private:
	void update();
protected:

     Ui::Dialog*		ui_;
	 qreal				scale_;
     SelRect*           selRect_ = nullptr;

     ZoomRect*			maxRect_= nullptr;
     ZoomRect*			minRect_= nullptr;
     ZoomRect*			defRect_= nullptr;

};
#endif
