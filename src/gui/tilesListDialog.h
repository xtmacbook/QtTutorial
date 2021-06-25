
#ifndef MATCHTILESHOWDIALOG_H
#define MATCHTILESHOWDIALOG_H
#include "treeitem.h"

#include <QMainWindow>
#include <QTreeView>
#include <QDataStream>

namespace Ui
{
	class TileListsWin;
}

class TreeModel;


class MatchTileShowDialog :public QMainWindow
{
	Q_OBJECT

public:
	MatchTileShowDialog(QWidget*parent = Q_NULLPTR);

	virtual ~MatchTileShowDialog();

	QTreeView*		getTreeView();

Q_SIGNALS:
    void            visiableAllTilesItem(bool);
    void            visiableSpecialLabelItem(bool,TreeItem*);
    void            addLabelItem(TreeItem*);
    void            removeLabelItem(TreeItem*);
	void			updateLabelItem(TreeItem*,bool, QPoint);
private Q_SLOTS:

    void            loadTilesFile();
    void            saveTilesFile();

    void            addTiles(double*);
    void            delLastTiles();
	void            delCurrentTiles();
    void            datasGenerated(QList<qreal>);
	void			tileItemChange(const QModelIndex&, const QModelIndex&, const QVector<int> &);

    void            locationTile();
    void            onOff();
    void            update();
    void            hideTile();
    void            orderErr();

    void            clearTree();
protected:
    Ui::TileListsWin*       ui_;
    bool                    visiable_item_ = true;
    TreeModel*              model_ = nullptr;
};

QDataStream &operator<<(QDataStream& os, const TreeItem& dt);

#endif
