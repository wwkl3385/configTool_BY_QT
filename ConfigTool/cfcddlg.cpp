#include "cfcddlg.h"
#include "ui_cfcddlg.h"

CFCDDlg::CFCDDlg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CFCDDlg)
{
    ui->setupUi(this);
}

CFCDDlg::~CFCDDlg()
{
    delete ui;
}
