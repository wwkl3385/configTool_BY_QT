#include "cfcddlg.h"
#include "ui_cfcddlg.h"

CFCDDlg::CFCDDlg(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::CFCDDlg)
{
    ui->setupUi(this);
    setWindowTitle("错峰充电设置");

    /* 时段设置 */
    for (int i=0; i<24; i++)
    {
        ui->startHourComboBox->insertItem(i, QString::number(i, 10));
        ui->stopHourComboBox->insertItem(i, QString::number(i, 10));
    }
    for (int i=0; i<60; i+=30)
    {
        ui->startMinuteComboBox->insertItem(i, QString::number(i, 10));
        ui->stopMinuteComboBox->insertItem(i, QString::number(i, 10));
    }

}

CFCDDlg::~CFCDDlg()
{
    delete ui;
}
