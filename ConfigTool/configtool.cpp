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

    /* 功能模块默认选项配置 --设置CheckBox 不可用*/
    ui->updateCheckBox->setDisabled(true);
    ui->settingCheckBox->setDisabled(true);
    ui->realDataCheckBox->setDisabled(true);
    ui->cacheCheckBox->setDisabled(true);
    ui->DatabaseCheckBox->setDisabled(true);
    ui->logCheckBox->setDisabled(true);

    /*libconfig.ini 读缓存区数据，写入控件*/
    mapLib = Cache.libConfigIniReadCache(); //maplib初始化为缓存区数据。
    QCacheMapLib::iterator its;
    for (its = mapLib.begin(); its != mapLib.end(); ++its)
    {
        INI_LIBCONFIG_CTRL *libIni;
        libIni = its.value();
        ui->centralWidget->findChild<QCheckBox *>(its.key())->setChecked(libIni->value[2] == "true" ? true : false); //数据写入控件
    }

    /*config.ini 读缓存区数据，写入控件*/
    mapConfig = Cache.configIniReadCache();
    QCacheMapConfg::iterator it;
    for (it = mapConfig.begin(); it != mapConfig.end(); ++it)
    {
        INI_CONFIG_CTRL *configIni;
        configIni = it.value();
        if (configIni->ctlType == LINEEDIT_TYPE)
            ui->centralWidget->findChild<QLineEdit *>(it.key())->setText(configIni->value);
#if 1
        if (configIni->ctlType == RADIOBUTTON_TYPE)
        {
            ui->centralWidget->findChild<QRadioButton *>(it.key())->setChecked((configIni->value == "1" ? true : false) | (configIni->value == "true" ? true : false));
            if ((configIni->value != "1") | (configIni->value != "true"))
                ; //按钮开关设置
        }

        if (configIni->ctlType == COMBOBOX_TYPE)
        {
           if (configIni->value.length() > 3) //过滤不同类型的值
           {
               ui->centralWidget->findChild<QComboBox *>(it.key())->setCurrentText(configIni->value);
               continue;
           }
           ui->centralWidget->findChild<QComboBox *>(it.key())->setCurrentIndex(configIni->value.toInt() - 1);
        }
#endif

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
    for (it = mapLib.begin(); it != mapLib.end(); ++it)
    {
        plibConfigIniTmp = it.value();
        plibConfigIniTmp->value[2] = (ui->centralWidget->findChild<QCheckBox *>(it.key())->checkState() == 2 ? "true" : "false");
    }

    Cache.libConfigIniWriteCache(mapLib);
}

/**********************************************************************
* 功    能：显示屏槽函数
* 输    入：
* 输    出：
* 作    者：
* 编写日期：2017.12.6
***********************************************************************/
void ConfigTool::on_screenCheckBox_clicked()
{
    ui->screenSinglePileCheckBox->setChecked(false); //显示屏与显示屏-直流单桩按钮 互斥操作
}

/**********************************************************************
* 功    能：显示屏-直流单桩槽函数
* 输    入：
* 输    出：
* 作    者：
* 编写日期：2017.12.6
***********************************************************************/
void ConfigTool::on_screenSinglePileCheckBox_clicked()
{
    ui->screenCheckBox->setChecked(false);   //显示屏与显示屏-直流单桩按钮 互斥操作
}
