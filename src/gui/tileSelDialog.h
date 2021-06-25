
#ifndef IR_TILESELDIALOG_H
#define IR_TILESELDIALOG_H

#include <QDialog>

namespace Ui {
	class TileSelDialogForm;
} // namespace Ui


class TileSelDialog : public QDialog
{
	Q_OBJECT

public:
	TileSelDialog(QWidget* parent);

	virtual ~TileSelDialog();

Q_SIGNALS:
	void	showTiles();
	void	addTile(double*);
    void	deleteLastTile();

private Q_SLOTS:
	void show_tiles();
	void add_tile();
	void del_tile();

    void real_update_base_point_pos(QPoint);
    void addSelBasePoint(QPoint);
    void addSelWrapPoint(QPoint);
protected:

	Ui::TileSelDialogForm* ui_;
};
#endif
