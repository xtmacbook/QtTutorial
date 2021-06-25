
#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QTreeView>
#include <QGraphicsItem>

class TreeItem;

//! [0]
class TreeView : public QTreeView
{
    Q_OBJECT

public:
	TreeView(QWidget *parent = Q_NULLPTR);

    virtual ~TreeView();

Q_SIGNALS:
	void	locationToImgPoint(const QGraphicsItem*,bool);
protected:
	void mouseDoubleClickEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // TREEMODEL_H
