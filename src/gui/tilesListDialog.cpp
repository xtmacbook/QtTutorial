
#include "tilesListDialog.h"
#include "ui_tileListDialog.h"
#include "treemodel.h"
#include "mainWindow.h"
#include "imgType.h"
#include "treeitem.h"

#include <QFileDialog>
#include <QMessageBox>

QDataStream & operator<<(QDataStream &os, const TreeItem &item)
{
	//check
	QVariant checkstate = item.itemData_[0];
	int check = checkstate.value<int>();

	QVariant idx = item.itemData_[1];
	int idx_i = idx.value<int>();

	float bx = item.itemData_[2].value<float>();
	float by = item.itemData_[3].value<float>();

	float wx = item.itemData_[4].value<float>();
	float wy = item.itemData_[5].value<float>();

	float error = item.itemData_[6].value<float>();

	os << check << '\t' << idx_i << '\t' << bx << "\t" << by << "\t" << wx << "\t" << wy << "\t" << error;

	return os;
}



MatchTileShowDialog::MatchTileShowDialog(QWidget*parent) :
	QMainWindow(parent)
{
	ui_ = new  Ui::TileListsWin;
	ui_->setupUi(this);

	setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint& ~Qt::WindowMinimizeButtonHint);

	connect(ui_->locationBtn, SIGNAL(clicked()), this, SLOT(locationTile()));
	connect(ui_->onAoffBtn, SIGNAL(clicked()), this, SLOT(onOff()));
	connect(ui_->delBtn, SIGNAL(clicked()), this, SLOT(delLastTiles()));
	connect(ui_->delBtn_c, SIGNAL(clicked()), this, SLOT(delCurrentTiles()));
	connect(ui_->updateBtn, SIGNAL(clicked()), this, SLOT(update()));
	connect(ui_->showHideBtn, SIGNAL(clicked()), this, SLOT(hideTile()));
	connect(ui_->loatTileA, SIGNAL(triggered()), this, SLOT(loadTilesFile()));
	connect(ui_->saveTileA, SIGNAL(triggered()), this, SLOT(saveTilesFile()));
	connect(ui_->emptyTiles, SIGNAL(triggered()), this, SLOT(clearTree()));

	QString show = "SO"; //QApplication::translate("forTranslate", "\345\272\217\345\217\267", Q_NULLPTR);
	QString num = QApplication::translate("forTranslate", "\345\272\217\345\217\267", Q_NULLPTR);
	QString baseX = QApplication::translate("forTranslate", "\345\237\272\345\207\206X", Q_NULLPTR);
	QString baseY = QApplication::translate("forTranslate", "\345\237\272\345\207\206Y", Q_NULLPTR);
	QString forWrapX = QApplication::translate("forTranslate", "\345\276\205\345\214\271\351\205\215X", Q_NULLPTR);
	QString forWrapY = QApplication::translate("forTranslate", "\345\276\205\345\214\271\351\205\215Y", Q_NULLPTR);
	QString pg = QApplication::translate("forTranslate", "\350\257\204\344\274\260", Q_NULLPTR);

	QStringList headers;
	headers << show << num << baseX << baseX << forWrapX << forWrapY << pg;
	model_ = new TreeModel(headers, "");
	ui_->view->setModel(model_);

	connect(ui_->view->header(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
		model_, SLOT(sortByRow(int, Qt::SortOrder)));

	connect(model_, SIGNAL(dataChanged(const QModelIndex, const QModelIndex, const QVector<int> &)), this, SLOT(tileItemChange(const QModelIndex, const QModelIndex, const QVector<int> &)));
}

MatchTileShowDialog::~MatchTileShowDialog()
{
	DELETEPTR(model_);
}

void MatchTileShowDialog::loadTilesFile()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		tr("Tile File:!"),
		QDir::currentPath(),
		tr("*.*"));
	if (QFile::exists(fileName))
	{
		QFile file(fileName);
		file.open(QIODevice::ReadOnly);

		if (model_)
		{
			clearTree();

			QString data = file.readAll();
			model_->resetData(data.split(QString("\n")));
			ui_->view->expandAll();

			int child_num = model_->rowCount();
			QModelIndex parendt_idx = model_->index(0, 0);

			for (int i = 0; i < child_num; i++)
			{
				QModelIndex idx = model_->index(i, 0, parendt_idx.parent());
				TreeItem * item = model_->getItem(idx);
				if (item) emit addLabelItem(item);
			}
		}
	}
}

void MatchTileShowDialog::saveTilesFile()
{
	QString save_data_dir = QCoreApplication::applicationDirPath() + "/data/";

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File As..."),
		save_data_dir,
		tr("txt (*.txt )"));

	if (fileName.isEmpty())
		return;
	else
	{
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly))
		{
			QMessageBox::information(this, tr("Unable to open file"), file.errorString());
			return;
		}

		QDataStream out(&file);

		QModelIndex index = model_->index(0, 0);
		int row_num = model_->rowCount();
		int clo_num = model_->columnCount();
		for (int i = 0; i < row_num; i++)
		{
			QModelIndex child_idx = model_->index(i, 0, index.parent());
			TreeItem * item = model_->getItem(child_idx);
			if (item)
			{
				out << item;
			}
		}

		file.close();
	}
}

void MatchTileShowDialog::addTiles(double*pos)
{
	QModelIndex index = model_->index(0, 0);
	int rowC = model_->rowCount(index.parent());
	if (!model_->insertRow(rowC, index.parent()))
		return;

	QModelIndex child = model_->index(rowC, 0, index.parent());

	QVariant checkstate(2);
	model_->setData(child, checkstate, Qt::CheckStateRole);
	child = model_->index(rowC, 1, index.parent());

	QModelIndex preIndex = model_->index(rowC - 1, 0, index.parent());
	TreeItem * preItem = model_->getItem(preIndex);
	QString text = preItem->getLabelText();
	int code = text.toInt();

	QVariant data1(code + 1);
	model_->setData(child, data1, Qt::EditRole);

	for (int column = 2; column < 6; ++column)
	{
		QModelIndex child = model_->index(rowC, column, index.parent());
		QVariant data(pos[column - 2]);
		model_->setData(child, data, Qt::EditRole);
	}

	index = model_->index(rowC, 0, index.parent());
	TreeItem * item = model_->getItem(index);


	item->updateLabelText(QString::number(code + 1));

	if (item) emit addLabelItem(item);
}

void MatchTileShowDialog::delLastTiles()
{
	int lastRow = model_->rowCount();
	if (lastRow > 0)
	{
		QModelIndex index = model_->index(lastRow - 1, 0);

		TreeItem * item = model_->getItem(index);
		if (item) emit removeLabelItem(item);

		model_->removeRow(index.row());
	}

}

void MatchTileShowDialog::delCurrentTiles()
{
	QModelIndex index = ui_->view->selectionModel()->currentIndex();

	TreeItem * item = model_->getItem(index);
	if (item) emit removeLabelItem(item);

	model_->removeRow(index.row());
}

void MatchTileShowDialog::datasGenerated(QList<qreal> data)
{
	QStringList data_list;
	for (int i = 0; i < data.length(); i += 5)
	{
		QString one;
		one += QString::number(1, 10);                  one += "\t";
		one += QString::number(i / 5, 10);			   one += "\t";
		one += QString::number(data.at(i + 0), 10, 5); one += "\t";
		one += QString::number(data.at(i + 1), 10, 5); one += "\t";
		one += QString::number(data.at(i + 2), 10, 5); one += "\t";
		one += QString::number(data.at(i + 3), 10, 5); one += "\t";
		one += QString::number(data.at(i + 4), 10, 5);

		data_list.append(one);
	}

	if (model_)
	{
		clearTree();
		model_->resetData(data_list);
		ui_->view->expandAll();
	}

	int child_num = model_->rowCount();
	QModelIndex parendt_idx = model_->index(0, 0);

	for (int i = 0; i < child_num; i++)
	{
		QModelIndex idx = model_->index(i, 0, parendt_idx.parent());
		TreeItem * item = model_->getItem(idx);
		if (item) emit addLabelItem(item);
	}


	if (this->isHidden())
		show();
}

void MatchTileShowDialog::tileItemChange(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
	int colIdx = topLeft.column();

	QModelIndex parendt_idx = model_->index(0, 0);

	int result = -1; TreeItem * item;
	double x, y;
	if (colIdx == 2)
	{
		QVariant basex = model_->data(topLeft);
		QModelIndex idx = model_->index(topLeft.row(), topLeft.column() + 1, parendt_idx.parent());
		QVariant basey = model_->data(idx);

		item = model_->getItem(topLeft);
		x = basex.value<double>();
		y = basey.value<double>();
		result = 0;
	}
	else if (colIdx == 3)
	{
		QVariant basey = model_->data(topLeft);
		QModelIndex idx = model_->index(topLeft.row(), topLeft.column() - 1, parendt_idx.parent());
		QVariant basex = model_->data(idx);

		item = model_->getItem(topLeft);
		x = basex.value<double>();
		y = basey.value<double>();
		result = 0;

	}
	else if (colIdx == 4)
	{
		QVariant basex = model_->data(topLeft);
		QModelIndex idx = model_->index(topLeft.row(), topLeft.column() + 1, parendt_idx.parent());
		QVariant basey = model_->data(idx);

		item = model_->getItem(topLeft);
		x = basex.value<double>();
		y = basey.value<double>();
		result = 1;

	}
	else if (colIdx == 5)
	{
		QVariant basey = model_->data(topLeft);
		QModelIndex idx = model_->index(topLeft.row(), topLeft.column() - 1, parendt_idx.parent());
		QVariant basex = model_->data(idx);

		item = model_->getItem(topLeft);
		x = basex.value<double>();
		y = basey.value<double>();
		result = 1;

	}
	else
	{

	}
	if (result == 0)
	{
		QPoint point(x, y);

		emit updateLabelItem(item, true, point);
	}
	else if (result == 1)
	{
		QPoint point(x, y);
		emit updateLabelItem(item, false, point);
	}

}

void MatchTileShowDialog::locationTile()
{

}

void MatchTileShowDialog::onOff()
{
	visiable_item_ = !visiable_item_;

	QModelIndex index = model_->index(0, 0);

	QVariant flag;
	(visiable_item_) ? flag.setValue(2) : flag.setValue(0);

	for (int row = 0; row < model_->rowCount(); ++row) {
		QModelIndex child = model_->index(row, 0, index.parent());
		model_->setData(child, flag, Qt::CheckStateRole);
	}

	emit  visiableAllTilesItem(visiable_item_);
}

void MatchTileShowDialog::update()
{
}

void MatchTileShowDialog::hideTile()
{
	hide();
}

void MatchTileShowDialog::orderErr()
{

}

void MatchTileShowDialog::clearTree()
{
	if (model_->hasChildren())
	{
		int child_num = model_->rowCount();
		QModelIndex parendt_idx = model_->index(0, 0);

		for (int i = 0; i < child_num; i++)
		{
			QModelIndex idx = model_->index(i, 0, parendt_idx.parent());
			TreeItem * item = model_->getItem(idx);
			if (item) emit removeLabelItem(item);
		}

		model_->removeRows(0, model_->rowCount());
	}
}

QTreeView* MatchTileShowDialog::getTreeView()
{
	return ui_->view;
}

