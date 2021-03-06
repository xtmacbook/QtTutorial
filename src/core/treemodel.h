
#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class TreeItem;

//! [0]
class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    TreeModel(const QStringList &headers, const QString &data,
              QObject *parent = 0);
    virtual ~TreeModel();

    bool                    isBooleanColumn( const QModelIndex &index ) const;

    void                    resetData(const QStringList&data);

    QVariant                data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    QVariant                headerData(int section, Qt::Orientation orientation,
                                    int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;

    QModelIndex             index(int row, int column,
                                 const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    QModelIndex             parent(const QModelIndex &index) const Q_DECL_OVERRIDE;

    int                     rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    int                     columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

    Qt::ItemFlags           flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
    bool                    setData(const QModelIndex &index, const QVariant &value,
                                   int role = Qt::EditRole) Q_DECL_OVERRIDE;
    bool                    setHeaderData(int section, Qt::Orientation orientation,
                                const QVariant &value, int role = Qt::EditRole) Q_DECL_OVERRIDE;

    bool                    insertColumns(int position, int columns,
                                     const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool                    removeColumns(int position, int columns,
                                    const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool                    insertRows(int position, int rows,
                                const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;
    bool                    removeRows(int position, int rows,
                                 const QModelIndex &parent = QModelIndex()) Q_DECL_OVERRIDE;

    TreeItem *              getItem(const QModelIndex &index) const;

Q_SIGNALS:
    void                    visiableSpecialOne(bool,TreeItem*);
private Q_SLOTS:
	void					sortByRow(int, Qt::SortOrder);
private:
    void                    resetData(const QStringList &lines, TreeItem *parent);

    TreeItem *              rootItem_;
};


#endif
