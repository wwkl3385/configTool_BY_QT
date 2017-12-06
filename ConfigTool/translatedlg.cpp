/************************************************************************
* Copyright (c) 2017, 青岛特来电新能源有限公司  All rights reserved.
*
* 文件名称：    translate.cpp
* 描    述：   秘钥转换实现函数
* 修改记录：
* 			V0.6，2017-11-24，创建，刘卫明
*
************************************************************************/
#include "translatedlg.h"
#include "ui_translatedlg.h"
#include<QMessageBox>
#include<QWidget>

/**********************************************************************
* 功     能：类的实例化
* 输     入：
* 输     出：
* 作     者：刘卫明
* 编写日期：2017.11.24
***********************************************************************/
translateDlg::translateDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::translateDlg)
{
    ui->setupUi(this);
    setWindowTitle("秘钥转换");
#if 0
    QHBoxLayout *pLayout = new QHBoxLayout();
    m_pButtonGroup = new QButtonGroup(this);

    // 设置互斥
    m_pButtonGroup->setExclusive(true);
    for (int i = 0; i < 3; ++i)
    {
        QRadioButton *pButton = new QRadioButton(this);

        // 设置文本
        pButton->setText(QString::fromLocal8Bit("切换%1").arg(i + 1));

        pLayout->addWidget(pButton);
        m_pButtonGroup->addButton(pButton);
    }
    pLayout->setSpacing(10);
    pLayout->setContentsMargins(10, 10, 10, 10);

    setLayout(pLayout);
#endif
}

translateDlg::~translateDlg()
{
    delete ui;
}

/**********************************************************************
* 功    能：字符串类型转为16进制类型(秘钥)
* 输    入：
* 输    出：
* 作    者：刘卫明
* 编写日期： 2017.11.24
***********************************************************************/
QString translateDlg::string2Hex(QString strAnsi)
{
    QString str;
    QString strHex;
    if(strAnsi.isEmpty())
    {
        QMessageBox::information(NULL, "秘钥装换", "输入信息为空", QMessageBox::NoButton, QMessageBox::NoButton);
        //QMessageBox::warning(NULL, "warning", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }

    for(int i = 0; i < strAnsi.toLocal8Bit().length(); i++)
    {
        str.sprintf("%02X", strAnsi.toLocal8Bit().at(i));
        strHex += str;
    }
    return strHex;
}

/**********************************************************************
* 功    能：16进制类型转为字符串类型(秘钥要求)
* 输    入：
* 输    出：
* 作    者：刘卫明
* 编写日期： 2017.11.24
***********************************************************************/
QString translateDlg::hex2String(QString strHex)
{
    int len = strHex.length();
    if(strHex.isEmpty())
    {
        QMessageBox::information(NULL, "秘钥装换", "输入信息为空", QMessageBox::NoButton, QMessageBox::NoButton);
        //QMessageBox::warning(NULL, "warning", "Content", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    if(len % 2)
    {
        QMessageBox::information(NULL, "秘钥装换", "输入字符数必须为偶数", QMessageBox::NoButton, QMessageBox::NoButton);
        return (" ");
    }

    QString str;
    QString strAnsi;
    for(int i=0; i<strHex.toLocal8Bit().length(); i=i+2)
    {
        str.sprintf("%c", (int)strtol(strHex.toLocal8Bit().mid(i, 2), NULL, 16));
        strAnsi += str;
    }
    return strAnsi;
}

void translateDlg::on_translatePushButton_clicked()
{
    ui->key2LineEdit->setText((string2Hex(ui->key1LineEdit->text())));
}
