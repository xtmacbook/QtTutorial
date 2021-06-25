

#include <QtWidgets>
#include <QAbstractItemModel>

#include "treeitem.h"
#include "treemodel.h"

TreeModel::TreeModel(const QStringList &headers, const QString &data, QObject *parent)
    : QAbstractItemModel(parent)
{
    QVector<QVariant> rootData;
    foreach (QString header, headers)
        rootData << header;

    rootItem_ = new TreeItem(rootData);
    resetData(data.split(QString("\n")), rootItem_);
}

bool TreeModel::isBooleanColumn( const QModelIndex &index ) const
{
    bool bRet = false;
    if ( index.isValid())
    {
        bRet = ( index.column() == 0 );
    }
    return bRet;
}

TreeModel::~TreeModel()
{
    delete rootItem_;
}


void TreeModel::sortByRow(int row, Qt::SortOrder order)
{
	sort(row, order);
}

void TreeModel::resetData(const QStringList&data)
{
    resetData(data, rootItem_);
}

int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return rootItem_->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    TreeItem *item = getItem(index);
    if ( role == Qt::CheckStateRole && isBooleanColumn( index ) )
    {
        Qt::CheckState eChkState = ( item->data( index.column() ).toBool() ) ? Qt::Checked : Qt::Unchecked;
        return eChkState;
    }
    else if((role == Qt::EditRole | role == Qt::DisplayRole) && !isBooleanColumn(index) )
    {
        return item->data(index.column());
    }

    return QVariant();
}

//! [3]
Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return QAbstractItemModel::flags(index);

    Qt::ItemFlags flag = QAbstractItemModel::flags(index);

    if ( isBooleanColumn( index ) )
    {
        return Qt::ItemIsUserCheckable | flag;
    }
    else
    {
        flag =  Qt::ItemIsEditable | flag;
        flag &=  ~Qt::ItemIsUserCheckable;
        return flag;
    }
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem_;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem_->data(section);

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();
    TreeItem *parentItem = getItem(parent);
    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem_->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem_->columnCount());
    endInsertRows();

    return success;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem_)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem_->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem_->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);
    return parentItem->childCount();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool result = false;
    if (role == Qt::EditRole || role == Qt::CheckStateRole )
    {
        TreeItem *item = getItem(index);
        if ( role == Qt::CheckStateRole && isBooleanColumn( index ) )
        {
            Qt::CheckState eChecked = static_cast< Qt::CheckState >( value.toInt() );
            bool bNewValue = (eChecked == Qt::Checked);
            result = item->setData( index.column(), bNewValue );

            TreeItem * checkItem = getItem(index);
            emit visiableSpecialOne(bNewValue,checkItem);
        }
        else
            result = item->setData(index.column(), value);

        if (result)
            emit dataChanged(index, index);
    }

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem_->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

void TreeModel::resetData(const QStringList &lines, TreeItem *parent)
{
    QList<TreeItem*> parents;
    QList<int> indentations;
    parents << parent;
    indentations << 0;

    int number = 0;

    while (number < lines.count())
    {
        int position = 0;
        while (position < lines[number].length()) {
            if (lines[number].at(position) != ' ')
                break;
            ++position;
        }

        QString lineData = lines[number].mid(position).trimmed();

        if (!lineData.isEmpty())
        {

            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
            QVector<QVariant> columnData;
            for (int column = 0; column < columnStrings.count(); ++column)
                columnData << columnStrings[column];

            if (position > indentations.last())
            {

                if (parents.last()->childCount() > 0)
                {
                    parents << parents.last()->child(parents.last()->childCount()-1);
                    indentations << position;
                }
            }
            else
            {
                while (position < indentations.last() && parents.count() > 0)
                {
                    parents.pop_back();
                    indentations.pop_back();
                }
            }

            TreeItem *parent = parents.last();
            TreeItem *newChild = parent->insertChildren(parent->childCount(), rootItem_->columnCount(), columnData);
            if (newChild)
            {
                /*for (int column = 0; column < columnData.size(); ++column)
                    parent->child(parent->childCount() - 1)->setData(column, columnData[column]);*/

                //emit customerRowInsert(newChild);
            }
        }

        ++number;
    }
}


