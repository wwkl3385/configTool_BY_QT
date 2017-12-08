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

    /*  设置界面 灯条时间 选项 赋值 */
    for (int i=0; i<24; i++)
    {
        ui->lightOpenHourComboBox->insertItem(i, QString::number(i, 10));  //0-23 时
        ui->lightCloseHourComboBox->insertItem(i, QString::number(i, 10)); //0-23 时
    }
    for (int i=0; i<60; i++)
    {
        ui->lightOpenMinuteComboBox->insertItem(i, QString::number(i, 10));   //0-59 秒
        ui->lightCloseMinuteComboBox->insertItem(i, QString::number(i, 10));  //0-59 秒

    }

    /* 充电模式 ui 按钮控制  充电模式为远程充电时*/
    if (ui->chargeTypeComboBox->currentIndex() == 0)
    {
        ui->localChargeTypeComboBox->setDisabled(true);
        ui->localChargePasswordLineEdit->setDisabled(true);
    }

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

        if (configIni->ctlType == COMBOBOX_TYPE)
        {
            /*  过滤不同类型的值 --服务器地址 灯条时间  */
            if ((configIni->value.length() > 3) || (it.key() == "lightOpenHourComboBox") || (it.key() == "lightCloseHourComboBox")
                    || (it.key() == "lightOpenMinuteComboBox") || (it.key() == "lightCloseMinuteComboBox"))
            {
                ui->centralWidget->findChild<QComboBox *>(it.key())->setCurrentText(configIni->value);
                continue;
            }
            ui->centralWidget->findChild<QComboBox *>(it.key())->setCurrentIndex(configIni->value.toInt() - 1);
        }

        if (configIni->ctlType == RADIOBUTTON_TYPE)
        {
            bool b = ((configIni->value == "1" ? true : false) || (configIni->value == "true" ? true : false)) ? true : false;

            ui->centralWidget->findChild<QRadioButton *>(it.key() + "Open")->setChecked(b);
            ui->centralWidget->findChild<QRadioButton *>(it.key() + "Close")->setChecked(!b);

            //            /* 互斥按钮处理  */
            //            if ((it.key() == "AbnormalOpenRadioButton") & ((configIni->value != "1") & (configIni->value != "true")))
            //                ui->centralWidget->findChild<QRadioButton *>("AbnormalCloseRadioButton")->setChecked(true);
            //            if ((it.key() == "buttonOpenRadioButton") & ((configIni->value != "1") & (configIni->value != "true")))
            //                ui->centralWidget->findChild<QRadioButton *>("buttonCloseRadioButton")->setChecked(true);
            //            if ((it.key() == "cardAutoOpenRadioButton") & ((configIni->value != "1") & (configIni->value != "true")))
            //                ui->centralWidget->findChild<QRadioButton *>("cardAutoCloseRadioButton")->setChecked(true);
            //            if ((it.key() == "cardMultiOpenRadioButton") & ((configIni->value != "1") & (configIni->value != "true")))
            //                ui->centralWidget->findChild<QRadioButton *>("cardMultiCloseRadioButton")->setChecked(true);
            //            if ((it.key() == "localEncryptOpenRadioButton") & ((configIni->value != "1") & (configIni->value != "true")))
            //                ui->centralWidget->findChild<QRadioButton *>("localEncryptCloseRadioButton")->setChecked(true);
            //            if ((it.key() == "remoteEncryptOpenRadioButton") & ((configIni->value != "1") & (configIni->value != "true")))
            //                ui->centralWidget->findChild<QRadioButton *>("remoteEncryptCloseRadioButton")->setChecked(true);
            //            if ((it.key() == "singlePileGroupOpenRadioButton") & ((configIni->value != "1") & (configIni->value != "true")))
            //                ui->centralWidget->findChild<QRadioButton *>("singlePileGroupCloseRadioButton")->setChecked(true);
            //            if ((it.key() == "ticketOpenRadioButton") & ((configIni->value != "1") & (configIni->value != "true")))
            //                ui->centralWidget->findChild<QRadioButton *>("ticketCloseRadioButton")->setChecked(true);
            //            if ((it.key() == "vinAutoOpenRadioButton") & ((configIni->value != "1") & (configIni->value != "true")))
            //                ui->centralWidget->findChild<QRadioButton *>("vinAutoCloseRadioButton")->setChecked(true);
        }

        if (configIni->ctlType == CHECKBOX_TYPE)
        {
            ui->centralWidget->findChild<QCheckBox *>(it.key())->setChecked(configIni->value == "true" ? true : false);
        }
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
* 功    能：菜单-帮助，提示信息
* 输    入：
* 输    出：
* 作    者：刘卫明
* 编写日期：2017.11.24
***********************************************************************/
void ConfigTool::on_actionAbout_triggered()
{
    QMessageBox::about(NULL, "关于", "Copyright (c) 2017, 青岛特来电新能源有限公司, All rights reserved." \
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

    /* 写libconfig.ini*/
    QCacheMapLib::iterator its;
    for (its = mapLib.begin(); its != mapLib.end(); ++its)
    {
        INI_LIBCONFIG_CTRL *plibConfigIniTmp;
        plibConfigIniTmp = its.value();
        plibConfigIniTmp->value[2] = (ui->centralWidget->findChild<QCheckBox *>(its.key())->checkState() == 2 ? "true" : "false");
    }

    Cache.libConfigIniWriteCache(mapLib);

    /* 写config.ini*/
    QCacheMapConfg::iterator it;
    for (it = mapConfig.begin(); it != mapConfig.end(); ++it)
    {
        INI_CONFIG_CTRL *configIni;
        configIni = it.value();

        if (configIni->ctlType == LINEEDIT_TYPE)
            configIni->value = ui->centralWidget->findChild<QLineEdit *>(it.key())->text();

        if (configIni->ctlType == RADIOBUTTON_TYPE)
        {
            configIni->value = (ui->centralWidget->findChild<QRadioButton *>(it.key() + "Open")->isChecked() == 0 ? "0" : "1");

            /* 特殊按钮处理 */
            if ((it.key() == "localEncryptRadioButton") | (it.key() == "remoteEncryptRadioButton"))
                    configIni->value = (configIni->value == "1" ? "true" : "false");
         }

        if (configIni->ctlType == COMBOBOX_TYPE)
        {
            if (it.key() == ("webUrlComboBox") || (it.key() ==  "lightOpenHourComboBox") || (it.key() ==  "lightCloseHourComboBox")
                    || (it.key() == "lightOpenMinuteComboBox" ) || (it.key() == "lightCloseMinuteComboBox"))
            {
               configIni->value = ui->centralWidget->findChild<QComboBox *>(it.key())->currentText();
            }
            else
            {
                configIni->value = QString::number((ui->centralWidget->findChild<QComboBox *>(it.key())->currentIndex()) + 1, 10); //整型转为对应的字符串。
            }
        }

        if (configIni->ctlType == CHECKBOX_TYPE)
        {
            configIni->value = (ui->centralWidget->findChild<QCheckBox *>(it.key())->checkState() == 2 ? "true" : "false");
        }
    }

    Cache.configIniWriteCache(mapConfig);

    QMessageBox::information(this, "提示", "配置文件生成成功！");

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

/**********************************************************************
* 功    能： 服务器地址与对应端口号
* 输    入：
* 输    出：
* 作    者：
* 编写日期：2017.12.8
***********************************************************************/
void ConfigTool::on_webUrlComboBox_currentTextChanged(const QString &arg1)
{
   if ( arg1 == "http://api.teld.cn")
       ui->webPortLineEdit->setText("8001");
   if (arg1 == "https://ctrl.teld.cn")
       ui->webPortLineEdit->setText("443");
}

/**********************************************************************
* 功    能：充电模式控件ui -显示处理函数
* 输    入：
* 输    出：
* 作    者：
* 编写日期：2017.12.8
***********************************************************************/
void ConfigTool::on_chargeTypeComboBox_currentIndexChanged(int index)
{
    if (index == 0)
    {
        ui->localChargeTypeComboBox->setCurrentIndex(-1);
        ui->localChargeTypeComboBox->setDisabled(true);
        ui->localChargePasswordLineEdit->setDisabled(true);
    }
    else
    {
        ui->localChargeTypeComboBox->setCurrentIndex(0);
        ui->localChargeTypeComboBox->setEnabled(true);
        ui->localChargePasswordLineEdit->setEnabled(true);
    }
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
