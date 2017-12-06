/************************************************************************
* Copyright (c) 2017, 青岛特来电新能源有限公司  All rights reserved.
*
* 文件名称：  configtool.cpp
* 描   述：  配置工具处理函数
* 修改记录：
* 			V0.6，2017-11-24，创建，刘卫明
*
************************************************************************/
#include "configtool.h"
#include "datacache.h"
#include "ui_configtool.h"
#include "translatedlg.h"
#include <QDebug>
#include <qnamespace.h>
#include <QMessageBox>

ConfigTool::ConfigTool(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ConfigTool)
{
    ui->setupUi(this);
    setWindowTitle("集控器配置工具");
    setWindowIcon(QIcon("ConfigTool.ico")); //设置软件图标
    this->setFixedSize(822, 814);           //设置窗口尺寸

    mapLib = Cache.libConfigIniReadCache(); //maplib初始化为缓存区数据。

    QCacheMapLib::iterator it;
    for(it = mapLib.begin(); it != mapLib.end(); ++it)
    {
        INI_LIBCONFIG_CTRL *libIni;
        libIni = it.value();
        ui->centralWidget->findChild<QCheckBox *>(it.key())->setChecked(libIni->value[2] == "true" ? true : false);
    }

    /*groupbox 背景颜色设置*/
#if 0
    ui->netGroupBox->setStyleSheet("QGroupBox{background:darkcyan}");
    ui->can0GroupBox->setStyleSheet("QGroupBox{background:orange}");
    ui->can1GroupBox->setStyleSheet("QGroupBox{background:orange}");
    ui->webGroupBox->setStyleSheet("QGroupBox{background:lightgreen}");
    ui->remoteGroupBox->setStyleSheet("QGroupBox{background:lightgreen}");
    ui->stationGroupBox->setStyleSheet("QGroupBox{background:lightgreen}");
    ui->chargeGroupBox->setStyleSheet("QGroupBox{background:khaki}");
    ui->cscuGroupBox->setStyleSheet("QGroupBox{background:aquamarine}");
#endif
    // ui->AbnormalCloseRadioButton->setEnabled(TRUE);
    //ui->AbnormalOpenRadioButton->setChecked(1);
    //ui->AbnormalOpenRadioButton->setEnabled(0);
    //ui->MODULE("moduleTab")->setStyleSheet("QGroupBox{background:beige}");
    //ui->NAME(moduleTab)->setStyleSheet("QGroupBox{background:beige}");
    ui->moduleTab->setStyleSheet("QGroupBox{background:beige}");
    ui->firstSettingTab->setStyleSheet("QGroupBox{background:bisque}");
    ui->secondSettingTab->setStyleSheet("QGroupBox{background:wheat}");
    ui->exportPushButton->setStyleSheet("QPushButton{background:aquamarine}");
    //ui->generatePushButton->setStyleSheet("QPushButton{background:hotpink}");
    QPushButton *db = ui->centralWidget->findChild<QPushButton *>("generatePushButton"); //使用字符串关联到到控件
    db->setStyleSheet("QPushButton{background:hotpink}");

}

ConfigTool::~ConfigTool()
{
    delete ui;
}

/**********************************************************************
* 功    能：秘钥转换按钮
* 输    入：
* 输    出：
* 作    者：刘卫明
* 编写日期：2017.11.24
***********************************************************************/
void ConfigTool::on_keyTranslatePushButton_clicked()
{
    translateDlg dlg;
    dlg.exec();
}

/**********************************************************************
* 功    能：菜单-帮助，提示信息
* 输    入：
* 输    出：
* 作    者：刘卫明
* 编写日期：2017.11.24
***********************************************************************/
void ConfigTool::on_actionAbout_triggered()
{
    QMessageBox::about(NULL, "关于", "Copyright (c) 2017, 青岛特来电新能源有限公司, All rights reserved."
                                   "\n\n集控器配置工具V0.9\n\n智能充电中心");
}

/**********************************************************************
* 功    能： 生成配置按钮关联的槽函数，根据UI界面控件的实际值更新map缓冲区并写入libconfig.ini
* 输    入：
* 输    出：
* 作    者：刘卫明
* 编写日期：2017.11.28
***********************************************************************/
void ConfigTool::on_generatePushButton_clicked()
{
    INI_LIBCONFIG_CTRL *plibConfigIniTmp;

    QCacheMapLib::iterator it;
    for(it = mapLib.begin(); it != mapLib.end(); ++it)
    {
        plibConfigIniTmp = it.value();
        plibConfigIniTmp->value[2] = (ui->centralWidget->findChild<QCheckBox *>(it.key())->checkState() == 2 ? "true" : "false");
    }

    Cache.libConfigIniWriteCache(mapLib);
}
