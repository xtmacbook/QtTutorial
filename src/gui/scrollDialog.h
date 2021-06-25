
#ifndef IR_SCROLLDIALOG_H
#define IR_SCROLLDIALOG_H

#include <QDialog>
#include <QResizeEvent>
#include <QGraphicsScene>
#include <opencv2/opencv.hpp>

namespace Ui {
	class scrollDialogForm;
} // namespace Ui

class SelRect;
class ViewPort;

class ScrollDialog : public QDialog
{
	Q_OBJECT

public:
	ScrollDialog(QWidget* parent);

	virtual ~ScrollDialog();

	void      setScene(QGraphicsScene*);
	void	  resizeView();
	void	  reposSelRect();
    void      clearViewparams();

    ViewPort * getView();
    void        constraintSize(bool f);
    void        mainSelRect(SelRect*);
protected:
	void resizeEvent(QResizeEvent* size);
private:
	Ui::scrollDialogForm* ui_;

	QTransform transform_;

	SelRect*	selRect_;
};
#endif
