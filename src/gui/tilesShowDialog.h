
#ifndef TILESHOWDIALOG_H
#define TILESHOWDIALOG_H

#include <QDialog>
#include <QTreeView>

namespace Ui
{
	class TileListsWin;
}

class TreeModel;
class TileShowDialog :public QDialog
{
	Q_OBJECT

public:
	TileShowDialog(QWidget*parent = Q_NULLPTR);

	virtual ~TileShowDialog();

	QTreeView*		getTreeView();

	private Q_SLOTS:

	void	loadTilesFile();
	void	saveTilesFile();

	void	addTiles(double*);
	void	delLastTiles();
	void    updateData(QList<qreal>);

	void	locationTile();
	void	onOff();
	void	update();
	void	hideTile();
	void	orderErr();

private:
	void	clearData();
	void	updateActions();
protected:
	Ui::TileListsWin* ui_;
	TreeModel *		model_;
};

#endif
