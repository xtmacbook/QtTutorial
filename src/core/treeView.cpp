#include "treeView.h"
#include "treemodel.h"
#include "treeitem.h"
#include "labelItem.h"
#include <QMouseEvent>

TreeView::TreeView(QWidget *parent /*= Q_NULLPTR*/):QTreeView(parent)
{
	setSortingEnabled(true);
}

TreeView::~TreeView()
{

}

void TreeView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		QModelIndex index = indexAt(event->pos());
		if (index.isValid())                  
		{
			QAbstractItemModel * model = this->model();

			TreeModel * tmodel = (TreeModel*)(model);
			if (tmodel)
			{
                if ((index.column() == 0 )|| (index.column() == 1))
				{
					/*QList<qreal> data;
					for (int columnIndex = 1; columnIndex < m->columnCount(); columnIndex++)
					{
						QModelIndex x = m->index(index.row(), columnIndex);
						data.append(x.data().toDouble());

					}*/

					TreeItem * item = tmodel->getItem(index);
					emit locationToImgPoint(item->getBaseLabelItem(), true);
					emit locationToImgPoint(item->getWrapLabelItem(), false);
					return;
				}
				
			}
		}

	}

	QTreeView::mouseDoubleClickEvent(event);
}
