
#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
    class scrollDialogForm;
} // namespace Ui



class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    SettingDialog(QWidget* parent);

    virtual ~SettingDialog();


};
#endif
