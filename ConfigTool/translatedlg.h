/***********************************************************************
* Copyright (c) 2017, 青岛特来电新能源有限公司  All rights reserved.
*
* 文件名称： translate.h
* 描    述： 秘钥转换类
* 修改记录：
* 			V0.6，2017-11-24，创建，刘卫明
*
***********************************************************************/
#ifndef TRANSLATEDLG_H
#define TRANSLATEDLG_H

#include<QDialog>
#include<QString>

namespace Ui {
class translateDlg;
}

class translateDlg : public QDialog
{
    Q_OBJECT

public:
    explicit translateDlg(QWidget *parent = 0);
    ~translateDlg();
    QString string2Hex(QString strAnsi);
    QString hex2String(QString strHex);

private slots:
    void on_translatePushButton_clicked();

private:
    Ui::translateDlg *ui;
};

#endif // TRANSLATEDLG_H
