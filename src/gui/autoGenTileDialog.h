
#ifndef AUTOGENTILEDIALOG_H
#define AUTOGENTILEDIALOG_H

#include "matchThread.h"

#include <QObject>
#include <QDialog>

namespace Ui
{
	class autoGenTileDialog;
}

class ProgressIndicator;

class AutoGenTileDialog :public QDialog
{
	Q_OBJECT

public:
	AutoGenTileDialog(QWidget*parent = Q_NULLPTR);

	virtual ~AutoGenTileDialog();

Q_SIGNALS:
	void	showTilesList();
    void	dataBeGenerated(QList<qreal>);

private Q_SLOTS:
    void    sure_btn();
    void    show_btn();
    void    sendNewData();
private:
    void initEnableOperator();
protected:
	void showEvent(QShowEvent *);
protected:

    Ui::autoGenTileDialog*  ui_;
	ProgressIndicator*		progress_ = nullptr;
	MatchThread				matcher_;
};

#endif
