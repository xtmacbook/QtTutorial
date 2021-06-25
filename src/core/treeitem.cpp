 
#include "treeitem.h"
#include "labelItem.h"

#include "imgType.h"
#include <QStringList>

//! [0]
TreeItem::TreeItem(const QVector<QVariant> &data, TreeItem *parent)
{
    parentItem_ = parent;
    itemData_ = data;
	
    int num = data.at(1).toInt();
	baseLabel_ = new LabelItem(QString::number(num));
	wrapLabel_ = new LabelItem(QString::number(num));
}
//! [0]

//! [1]
TreeItem::~TreeItem()
{
    DELETEPTR(baseLabel_);
    DELETEPTR(wrapLabel_);

    qDeleteAll(childItems_);
}
//! [1]

//! [2]
TreeItem *TreeItem::child(int number)
{
    return childItems_.value(number);
}
//! [2]

//! [3]
int TreeItem::childCount() const
{
    return childItems_.count();
}
//! [3]

//! [4]
int TreeItem::childNumber() const
{
    if (parentItem_)
        return parentItem_->childItems_.indexOf(const_cast<TreeItem*>(this));

    return 0;
}
//! [4]

//! [5]
int TreeItem::columnCount() const
{
    return itemData_.count();
}
//! [5]

//! [6]
QVariant TreeItem::data(int column) const
{
    return itemData_.value(column);
}
//! [6]

//! [7]
bool TreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > childItems_.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        TreeItem *item = new TreeItem(data, this);
        childItems_.insert(position, item);
    }

    return true;
}

TreeItem* TreeItem::insertChildren(int position, int columns, QVector<QVariant>&data)
{
	if (position < 0 || position > childItems_.size())
		return nullptr;

	TreeItem *item = new TreeItem(data, this);
	childItems_.insert(position, item);
	return item;
}

//! [7]

//! [8]
bool TreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > itemData_.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData_.insert(position, QVariant());

    foreach (TreeItem *child, childItems_)
        child->insertColumns(position, columns);

    return true;
}
//! [8]

//! [9]
TreeItem *TreeItem::parent()
{
    return parentItem_;
}
//! [9]

//! [10]
bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems_.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems_.takeAt(position);

    return true;
}
//! [10]

bool TreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > itemData_.size())
        return false;

    for (int column = 0; column < columns; ++column)
        itemData_.remove(position);

    foreach (TreeItem *child, childItems_)
        child->removeColumns(position, columns);

    return true;
}

//! [11]
bool TreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= itemData_.size())
        return false;

    itemData_[column] = value;

    return true;
}

LabelItem * TreeItem::getBaseLabelItem()
{
	return baseLabel_;
}

LabelItem * TreeItem::getWrapLabelItem()
{
    return wrapLabel_;
}

void TreeItem::updateLabelText(const QString&t)
{
	if (baseLabel_)baseLabel_->updateLabelText(t);
	if (wrapLabel_)wrapLabel_->updateLabelText(t);
}

QString TreeItem::getLabelText()
{
	 if (baseLabel_) return baseLabel_->getLabelText();
	 return QString("");
}

