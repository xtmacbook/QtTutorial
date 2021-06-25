#include "imgFrame.h"



ImgFrame::ImgFrame(QWidget *parent):
	QFrame(parent)
{
	//sel_ = new MoveEnableRect(this);
	//sel_->setWindowFlags(Qt::FramelessWindowHint | Qt::Tool | Qt::CustomizeWindowHint);//нч╠ъ©Р
	//sel_->setStyleSheet(QString("background-color: rgb(0, 255, 0);"));

	//sel_->show();
}

ImgFrame::~ImgFrame()
{

}

void ImgFrame::showSel()
{
	/*QPoint p = pos();
	QSize sz = sel_->size();

	sel_->move(QPoint(p.x() + int(sz.width() / 2.0), p.y() + sz.height() / 2.0));*/
	//sel_->show();
}
