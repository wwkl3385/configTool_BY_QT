/***********************************************************************
* Copyright (c) 2017, 青岛特来电新能源有限公司  All rights reserved.
*
* 文件名称： cfcddlg.h
* 描    述： 错峰充电配置
* 修改记录：
* 			V0.6，2017-12-8，创建，刘卫明
*
***********************************************************************/
#ifndef CFCDDLG_H
#define CFCDDLG_H

#include<QDialog>

namespace Ui {
class CFCDDlg;
}

class CFCDDlg : public QDialog
{
    Q_OBJECT

public:
    explicit CFCDDlg(QDialog *parent = 0);
    ~CFCDDlg();

private:
    Ui::CFCDDlg *ui;
};

#endif // CFCDDLG_H
