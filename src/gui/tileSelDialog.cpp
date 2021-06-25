
#include "tileSelDialog.h"
#include "ui_tileSelDialog.h"
#include "mainWindow.h"

TileSelDialog::TileSelDialog(QWidget* parent):
       QDialog(parent),
       ui_(new Ui::TileSelDialogForm)
{
    ui_->setupUi(this);

    ui_->real_pos->setStyleSheet("color:red;");

    Qt::WindowFlags flags = this->windowFlags();
    setWindowFlags(flags&~Qt::WindowContextHelpButtonHint);
	connect(ui_->addTileBtn, SIGNAL(clicked()), this, SLOT(add_tile()));
	connect(ui_->delTileBtn, SIGNAL(clicked()), this, SLOT(del_tile()));
	connect(ui_->showTileListsBtn, SIGNAL(clicked()), this, SLOT(show_tiles()));
}

TileSelDialog::~TileSelDialog()
{

}


void TileSelDialog::show_tiles()
{
	emit showTiles();
}

void TileSelDialog::add_tile()
{
    double data[4];

    QObject * p =  this->parent();
    ImgResMainWindow* mainW = (ImgResMainWindow*)(p);

    data[0] = ui_->baseLocationXSpinBox->value();
    data[1] = ui_->baseLocationYSpinBox->value();
    data[2] = ui_->wrapLocationXSpinBox->value();
    data[3] = ui_->wrapLocationYSpinBox->value();

    emit addTile(data);
}

void TileSelDialog::del_tile()
{
    emit deleteLastTile();
}

void TileSelDialog::real_update_base_point_pos(QPoint point)
{
    QString text;
    text.append("X: ");
    text.append( QString::number(point.x(), 10, 4) );
    text.append( " ,Y: ");
    text.append( QString::number(point.y(), 10, 4));
    ui_->real_pos->setText(text);
}

void TileSelDialog::addSelBasePoint(QPoint point)
{
    ui_->baseLocationXSpinBox->setValue(point.x());
    ui_->baseLocationYSpinBox->setValue(point.y());
}

void TileSelDialog::addSelWrapPoint(QPoint point)
{
    ui_->wrapLocationXSpinBox->setValue(point.x());
    ui_->wrapLocationYSpinBox->setValue(point.y());
}


