/***********************************************************************
* Copyright (c) 2017, 青岛特来电新能源有限公司  All rights reserved.
*
* 文件名称： configtool.h
* 描    述：
* 修改记录：
* 			V0.6，2017-11-24，创建，刘卫明
*
***********************************************************************/
#ifndef CONFIGTOOL_H
#define CONFIGTOOL_H

#include <QMainWindow>
#include <windows.h>
#include "datacache.h"

#define MAX_PATH   260

#define MODULE(name)  #name
#define NAME(instr)   #instr


namespace Ui {
class ConfigTool;
}

class ConfigTool : public QMainWindow
{
    Q_OBJECT

public:
    explicit ConfigTool(QWidget *parent = 0);
    ~ConfigTool();
    DataCache Cache;
    QCacheMapLib   mapLib;    //libconfig.ini map数据缓存区
    QCacheMapConfg mapConfig; //config.ini   map数据缓存区

    BOOL m_bCharge;
    BOOL m_bSchedule;
    BOOL m_bSetting;
    BOOL m_bUpdate;
    BOOL m_bReal;
    BOOL m_bCache;
    BOOL m_bDatabase;
    BOOL m_bLog;
    BOOL m_bRemote;
    BOOL m_bLocal;
    BOOL m_bEquipment;
    BOOL m_bAmmeter;
    BOOL m_bHumiture;
    BOOL m_bTicket;
    BOOL m_bCard;
    BOOL m_bScreen;
    BOOL m_bGPIO;
    BOOL m_bDevManager;
    BOOL m_bWebServer;
    BOOL m_bScreenSinglePile;
    BOOL m_bScanner;
    void updateData(uchar);

private slots:
    void on_actionAbout_triggered();
    void on_screenCheckBox_clicked();
    void on_generatePushButton_clicked();
    void on_keyTranslatePushButton_clicked();
    void on_screenSinglePileCheckBox_clicked();
    void on_webUrlComboBox_currentTextChanged(const QString &arg1);
    void on_chargeTypeComboBox_currentIndexChanged(int index);

private:
    Ui::ConfigTool *ui;

};
#endif // CONFIGTOOL_H
