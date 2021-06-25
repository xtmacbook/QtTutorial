
#include "autoGenDialog.h"
#include "ui_autoGenTilesDialog.h"
#include "progressIndicator.h"

#include "feature.h"

AutoGenTileDialog::AutoGenTileDialog(QWidget*parent):
    QDialog(parent)
{
    ui_ = new  Ui::autoGenTileDialog;
    ui_->setupUi(this);

    Qt::WindowFlags flags = this->windowFlags();
    setWindowFlags(flags&~Qt::WindowContextHelpButtonHint);

	progress_ = new ProgressIndicator();
	ui_->progress_grid->addWidget(progress_);

	connect(ui_->cancel, SIGNAL(clicked()), this, SLOT(hide()));
	connect(ui_->ok, SIGNAL(clicked()), this, SLOT(sure_btn()));
    //connect(ui_->showLists, SIGNAL(clicked()), this, SLOT(show_btn()));
	connect(&matcher_, SIGNAL(overMatch()), this, SLOT(sendNewData()));

    initEnableOperator();
}

AutoGenTileDialog::~AutoGenTileDialog()
{
}

void AutoGenTileDialog::sure_btn()
{
	ui_->ok->setDisabled(true);
	ui_->cancel->setDisabled(true);
    //ui_->showLists->setDisabled(true);

	progress_->startAnimation();

	matcher_.match();
}

void AutoGenTileDialog::show_btn()
{
	emit showTilesList();
}

void AutoGenTileDialog::sendNewData()
{
	QList<qreal> data;
	matcher_.getMatcher(data); 

	progress_->stopAnimation();

	ui_->ok->setDisabled(false);
	ui_->cancel->setDisabled(false);
    //ui_->showLists->setDisabled(false);
	hide();

    if (!data.isEmpty()) emit dataIsGenerate(data);
}

void AutoGenTileDialog::initEnableOperator()
{
    ui_->intersetOperator->clear();

    QStringList operators;
    matcher_.getMatcherOperator(operators);

    for(int i = 0;i < operators.size();i++)
    {
         ui_->intersetOperator->insertItems(i,  QStringList() <<operators.at(i) );
    }
}

void AutoGenTileDialog::showEvent(QShowEvent * event)
{
	if (true)
	{
        //show enable algr
	}
	QDialog::showEvent(event);
}



