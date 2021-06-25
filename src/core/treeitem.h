 
#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>
#include <QVector>
#include <iostream>

class LabelItem;

class TreeItem
{
public:
    explicit TreeItem(const QVector<QVariant> &data, TreeItem *parent = 0);
    virtual  ~TreeItem();

    TreeItem *  child(int number);
    int         childCount() const;
    int         columnCount() const;
    QVariant data(int column) const;
	bool insertChildren(int position, int count, int columns);
	TreeItem* insertChildren(int position, int columns, QVector<QVariant>&data);
    bool insertColumns(int position, int columns);
    TreeItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

	LabelItem *		getBaseLabelItem();
	LabelItem *		getWrapLabelItem();

	void			updateLabelText(const QString&t);
	QString			getLabelText();

    QVector<QVariant> itemData_;
private:
    QList<TreeItem*> childItems_;

    TreeItem *		parentItem_;

	LabelItem*		baseLabel_;
	LabelItem*		wrapLabel_;
};


#endif 
