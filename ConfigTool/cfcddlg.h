#ifndef CFCDDLG_H
#define CFCDDLG_H

#include <QDialog>
#include "datacache.h"

namespace Ui {
class cfcdDlg;
}

class cfcdDlg : public QDialog
{
    Q_OBJECT

public:
    explicit cfcdDlg(QWidget *parent = 0);
    ~cfcdDlg();

    DataCache        Cache;
    QCacheMapConfg   mapConfig;    //config.ini map数据缓存区

private slots:
    void on_CFCDEnableCheckBox_clicked(bool checked);

    void on_CFCDSavePushButton_clicked();

private:
    Ui::cfcdDlg *ui;
};

#endif // CFCDDLG_H
