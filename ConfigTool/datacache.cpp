/************************************************************************
* Copyright (c) 2017, 青岛特来电新能源有限公司  All rights reserved.
*
* 文件名称：  datacache.cpp
* 描   述：  实现ui控件与成员变量数据的读写，配置文件（.ini）数据的存取
* 修改记录：
* 			V0.6，2017-11-25，创建，刘卫明
*
************************************************************************/
#include "datacache.h"
#include "configtool.h"
#include <QSettings>
#include <QDebug>
#include <string>

 /* 功能模块-- UI中CheckBox控件的名称 */
 // "settingCheckBox"  , "updateCheckBox"          , "realDataCheckBox"  ,
 // "cacheCheckBox"    , "DatabaseCheckBox"        , "logCheckBox"       ,   // 业务选择模块(默认选择项，不做处理)
const QString strWidgetNameLib[] = {"updateCheckBox" ,                       //0:升级服务
    "chargeCheckBox"   , "scheduleCheckBox"        , "devManageCheckBox",    //1-3:业务选择模块选择按钮名称:1-3
    "remoteCheckBox"   , "localCheckBox"           , "webServerCheckBox",    //4-6
    "equipmentCheckBox", "ammeteCheckBox"          , "humitureCheckBox" ,    //7-9
    "screenCheckBox"   , "screenSinglePileCheckBox", "gpioCheckBox"     ,    //10-12
    "ticketCheckBox"   , "scannerCheckBox"         , "cardCheckBox"          //13-15 设备模块选择按钮名称：4-15
    };

 /* 功能模块-- libconfig.ini 对应的文本信息  依次为：type  library  enable   */
const QString strKeyNameLib[][3] = {
    { "UPDATE/type"          , "UPDATE/library"          , "UPDATE/enable"          },  //0  : 升级服务
    { "CHARGESERVICE/type"   , "CHARGESERVICE/library"   , "CHARGESERVICE/enable"   },  //1  : 充电服务
    { "SCHEDULE/type"        , "SCHEDULE/library"        , "SCHEDULE/enable"        },  //2  : 负荷调度
    { "DEVICEMANAGE/type"    , "DEVICEMANAGE/library"    , "DEVICEMANAGE/enable"    },  //3  : 设备管理
    { "SERVER0/type"         , "SERVER0/library"         , "SERVER0/enable"         },  //4  : 云平台
    { "SERVER1/type"         , "SERVER1/library"         , "SERVER1/enable"         },  //5  : 场站平台
    { "WEBSERVER/type"       , "WEBSERVER/library"       , "WEBSERVER/enable"       },  //6  : Web平台
    { "CHARGEEQUIPMENT/type" , "CHARGEEQUIPMENT/library" , "CHARGEEQUIPMENT/enable" },  //7  : 充电设备
    { "AMMETER/type"         , "AMMETER/library"         , "AMMETER/enable"         },  //8  : 电表
    { "TEMPHUMI/type"        , "TEMPHUMI/library"        , "TEMPHUMI/enable"        },  //9  : 温度计
    { "SCREEN/type"          , "SCREEN/library"          , "SCREEN/enable"          },  //10 : 显示屏
    { "SCREENSINGLEPILE/type", "SCREENSINGLEPILE/library", "SCREENSINGLEPILE/enable"},  //11 : 显示屏-直流单桩
    { "GPIO/type"            , "GPIO/library"            , "GPIO/enable"            },  //12 : GPIO设备
    { "TICKET/type"          , "TICKET/library"          , "TICKET/enable"          },  //13 : 小票机
    { "SCANCODE/type"        , "SCANCODE/library"        , "SCANCODE/enable"        },  //14 : 扫码器
    { "CARD/type"            , "CARD/library"            , "CARD/enable"            }   //15 : 刷卡器
    };

/* 功能模块---val:默认值 :type,库的名称*.so,val */
const QString strLibKeyVal[][3] = {
    { "device", "libUpdate.so"                , "true" },  //0  : 升级服务
    { "device", "libChargeService.so"         , "false"},  //1  : 充电服务
    { "device", "libLoadSchedule.so"          , "false"},  //2  : 负荷调度
    { "device", "libDeviceManage.so"          , "false"},  //3  : 设备管理
    { "remote", "libIEC104Server.so"          , "false"},  //4  : 云平台
    { "local" , "libIEC104Server.so"          , "false"},  //5  : 场站平台
    { "device", "libWebServer.so"             , "false"},  //6  : Web平台
    { "device", "libChargeEquipment.so"       , "false"},  //7  : 充电设备
    { "device", "libAmmeter.so"               , "false"},  //8  : 电表
    { "device", "libTempHumi.so"              , "false"},  //9  : 温度计
    { "device", "libSerialScreen.so"          , "false"},  //10 : 显示屏
    { "device", "libSerialScreenSinglePile.so", "false"},  //11 : 显示屏-直流单桩
    { "device", "libGPIO.so"                  , "false"},  //12 : GPIO设备
    { "device", "libTicketDev.so"             , "false"},  //13 : 小票机
    { "device", "libScanCode.so"              , "false"},  //14 : 扫码器
    { "device", "libCard.so"                  , "false"}   //15 : 刷卡器;
    };

/* 配置选项一 UI中控件的名称 */
const QString strFirstWidgetName[] = {
    "directLineEdit"         , "singleLineEdit"              , "threeLineEdit"                 ,
    "passwordLineEdit"       , "teuiTypeComboBox"            , "boardComboBox"                 ,  // 0-5  : CSCU系统配置

    "cardTypeComboBox"       , "vinTypeComboBox"             , "AbnormalOpenRadioButton"       ,
    "cardAutoOpenRadioButton", "vinAutoOpenRadioButton"      , "buttonOpenRadioButton"         ,
    "ticketOpenRadioButton"  , "cardMultiOpenRadioButton"    , "singlePileGroupOpenRadioButton",  // 6-14 : 充电服务配置

    "net1IpLineEdit"         , "net1MaskLineEdit"            , "net1GateLineEdit"              ,
    "net2IPLineEdit"         , "net2MaskLineEdit"            , "net2GateLineEdit"              ,
    "dnsLineEdit"            ,                                                                     //15-21 : 网络配置

    "can0AddrLineEdit"       , "can0BaudLineEdit"            ,
    "can1AddrLineEdit"       , "can1BaudLineEdit"            ,                                     //22-25 : can

    "remoteKeyLineEdit"      , "remoteEncryptOpenRadioButton", "remoteIpLineEdit"              ,
    "remotePortLineEdit"     , "remoteStationLineEdit"       ,                                     //26-30 : 云平台服务配置

    "localKeyLineEdit"       , "localEncryptOpenRadioButton" , "localIpLineEdit"               ,
    "localPortLineEdit"      , "localStationLineStation"     ,                                     //31-35 : 场站平台服务配置

    "webUrlComboBox"         , "webPortLineEdit"             , "dataSecretLineEdit"            ,
    "webAesKeyLineEdit"      , "webAesIVLineEdit"            , "OperatorIdLineEdit"            ,
    "operatorSecretLineEdit" ,                                                                     //36-42 : web平台服务器配置
    };

/* 控件对应的类型 QCheckBox;QLineEdit;QComboBox;QRadioButton; */
const int widgetType[] ={
    LINEEDIT_TYPE   , LINEEDIT_TYPE   , LINEEDIT_TYPE   ,
    LINEEDIT_TYPE   , COMBOBOX_TYPE   , COMBOBOX_TYPE   ,   //0-5  : CSCU系统配置

    COMBOBOX_TYPE   , COMBOBOX_TYPE   , RADIOBUTTON_TYPE,
    RADIOBUTTON_TYPE, RADIOBUTTON_TYPE, RADIOBUTTON_TYPE,
    RADIOBUTTON_TYPE, RADIOBUTTON_TYPE, RADIOBUTTON_TYPE,   //6-14 : 充电服务配置

    LINEEDIT_TYPE   , LINEEDIT_TYPE   , LINEEDIT_TYPE   ,
    LINEEDIT_TYPE   , LINEEDIT_TYPE   , LINEEDIT_TYPE   ,
    LINEEDIT_TYPE   ,                                       //15-21 : 网络配置

    LINEEDIT_TYPE   , LINEEDIT_TYPE   ,
    LINEEDIT_TYPE   , LINEEDIT_TYPE   ,                     //22-25 : can

    LINEEDIT_TYPE   , RADIOBUTTON_TYPE, LINEEDIT_TYPE   ,
    LINEEDIT_TYPE   , LINEEDIT_TYPE   ,                     //26-30 : 云平台服务配置

    LINEEDIT_TYPE   , RADIOBUTTON_TYPE, LINEEDIT_TYPE   ,
    LINEEDIT_TYPE   , LINEEDIT_TYPE   ,                     //31-35 : 场站平台服务配置

    COMBOBOX_TYPE   , LINEEDIT_TYPE   , LINEEDIT_TYPE   ,
    LINEEDIT_TYPE   , LINEEDIT_TYPE   , LINEEDIT_TYPE   ,
    LINEEDIT_TYPE                                           //36-42 : web平台服务器配置
                    };

#if 0
array<int, 4> type = {
    CHECKBOX_TYPE,
    LINEEDIT_TYPE,
    COMBOBOX_TYPE,
    RADIOBUTTON_TYPE };
#endif

/* 配置选项一 config.ini 中对应的文本信息:section/key */
const QString strFirstKeyName[] = {
    "CSCUSys/DirectCurrent"               , "CSCUSys/SinglePhase"               , "CSCUSys/ThreePhase"                 ,  // 0-2: 直流终端数量，单相终端数量，三相终端数量
    "CSCUSys/Password"                    , "CSCUSys/NormalCard"                , "CSCUSys/BoardType"                  ,  // 3-5: 设置密码，交互类型，底板类型
    "CHARGE/CardType"                     , "CHARGE/VinType"                    , "CHARGE/EnergyFilter"                ,  // 6-8: 卡片类型，车牌号/VIN,异常电量过滤
    "CHARGE/CardAuto"                     , "CHARGE/VinAuto"                    , "CHARGE/LocalStop"                   ,  // 9-11: 刷卡触发充电，VIN触发充电，结束充电按钮
    "CHARGE/TicketEnable"                 , "CHARGE/SwitpingCardMultChooseMode" , "CHARGE/DevType"                     ,  // 12-14: 打印小票，刷卡多选模式，单桩群充模式

    "NET_0/IP"                            , "NET_0/NetMask"                     , "NET_0/Gateway"                      ,  //网口1IP地址，网口1子网掩码，网口1网关
    "NET_1/IP"                            , "NET_1/NetMask"                     , "NET_1/Gateway"                      ,  //网口2IP地址，网口2子网掩码，网口2网关
    "CSCUSys/DNS",                                                                                                        //DNS

    "CAN_0/CanAddr"                       , "CAN_0/Rate"                        ,                                         //can0配置
    "CAN_1/CanAddr"                       , "CAN_1/Rate"                        ,                                         //can1配置

    "SERVER0/AesKey"                      , "SERVER0/Encrypt"                   ,"SERVER0/ServerIP"                    ,
    "SERVER0/ServerPort"                  , "SERVER0/StationNo"                 ,                                          //云平台服务器配置

    "SERVER1/AesKey"                      , "SERVER1/Encrypt"                   , "SERVER1/ServerIP"                   ,
    "SERVER1/ServerPort"                  , "SERVER1/StationNo"                 ,                                          //场站平台服务器配置

    "WEBSERVER/Url"                       , "WEBSERVER/Port"                    , "WEBSERVER/DataSecret"               ,
    "WEBSERVER/AesKey"                    , "WEBSERVER/AesIV"                   , "WEBSERVER/OperatorID"               ,
    "WEBSERVER/OperatorSecret"            ,                                                                                //web平台服务器配置

    "POWER_LIMIT/PowerLimit_Enable"       , "POWER_LIMIT/CCUCount"              , "POWER_LIMIT/StationLimitPower"      ,
    "POWER_LIMIT/SafeLimitPower"          , "POWER_LIMIT/SumPower_Manual"       , "POWER_LIMIT/SumPower_Ammeter_Enable",
    "POWER_LIMIT/SumPower_Server_Enable"  , "POWER_LIMIT/SumPower_Manual_Enable",

    "SMART_CHARGE/SmartCharge_Enable"     ,
    "SMART_CAR/SmartCar_Enable"           ,

    "IOIN/DIN1"                           , "IOIN/DIN2"                         , "IOIN/DIN3"                          ,
    "IOIN/DIN4"                           , "IOIN/DIN5"                         , "IOIN/DIN6"                          ,
    "IOIN/DIN7"                           , "IOIN/DIN8"                         , "IOIN/DIN9"                          ,
    "IOIN/DIN10"                          ,

    "SINGLEPILESys/MasterMode"            , "SINGLEPILESys/SlaveMode"           , "SINGLEPILESys/PwdCharge"            ,

    "LIGHTSETTING/lightcontrol_enable"    , "LIGHTSETTING/lightopentime_hour"   , "LIGHTSETTING/lightopentime_minute"  ,
    "LIGHTSETTING/lightclosetime_hour"    , "LIGHTSETTING/lightclosetime_minute",

    "MAGNETICSWITCH/bOpenDoorPowerOutages"
    };

 /* 配置选项一 config.ini 中对应的文本信息:section/val */
const QString strFirstValue[] = {
    ""                    , ""                , ""                    ,  // 0-2: 直流终端数量，单相终端数量，三相终端数量
    "300001"              , "1"               , "2"                   ,  // 3-5: 设置密码，交互类型，底板类型
    "2"                   , "3"               , "0"                   ,  // 6-8: 卡片类型，车牌号/VIN,异常电量过滤
    "0"                   , "0"               , "1"                   ,  // 9-11: 刷卡触发充电，VIN触发充电，结束充电按钮
    "0"                   , "0"               , "0"                   ,  // 12-14: 打印小票，刷卡多选模式，单桩群充模式

    ""                    , ""                , ""                    ,  //网口1IP地址，网口1子网掩码，网口1网关
    ""                    , ""                , ""                    ,  //网口2IP地址，网口2子网掩码，网口2网关
    ""                    ,                                              //DNS

    "250"                 , "100000"          ,                          //can0配置
    "250"                 , "100000"          ,                          //can1配置

    "41414141414141414141414141414141", "true", "comm.teld.cn"        ,
    "8867"                , ""                ,                          //云平台服务器配置

    "41414141414141414141414141414141", "true", "comm.teld.cn"        ,
    "8867"                , ""                ,                          //场站平台服务器配置

    "https://ctrl.teld.cn", "443"             , "h9ZJ8nevVWdNVcfD"    ,
    "0iyYbIL6h33WIafh"    , "TqwJM4NhCGzmCeda", "teldrml5alnrv6zuyk7k",
    "eSD2fHUcwPzn1MY2"                                                   //web平台服务器配置
};


/**-------------------------------代码----------------------------------------**/

QCacheMapLib   dataCacheMapLib;    //libconfig.ini文件的 map数据缓冲区
QCacheMapConfg dataCacheMapConfig; //config.ini文件的 map数据缓冲区

/**********************************************************************
** 功    能：DataCache构造函数，创建一个map:dataCacheMaplib缓存区并初始化
** 输    入：
** 输    出：
** 作    者：刘卫明
** 编写日期：2017.11.27
***********************************************************************/
DataCache::DataCache(QObject *parent) : QObject(parent)
{
    INI_LIBCONFIG_CTRL *plibConfigIni;   //libconfig.ini

    for (uint i=0; i < sizeof(strWidgetNameLib)/(sizeof(QString)); i++) //sizeof(strWidgetNameLib)/(sizeof(QString)):功能模块控件的数量
    {
        plibConfigIni = new  INI_LIBCONFIG_CTRL;

        /* 写入libconfig.ini配置文件的默认部分 :type和library对应的值*/
        plibConfigIni->value[0] = strLibKeyVal[i][0]; // section/key
        plibConfigIni->value[1] = strLibKeyVal[i][1]; // section/library
        plibConfigIni->value[2] = strLibKeyVal[i][2]; // section/val

        /* 段名/键名 */
        for (int j=0; j<3; j++)
        {
            plibConfigIni->settingName[j] = strKeyNameLib[i][j];
        }

        dataCacheMapLib.insert(strWidgetNameLib[i], plibConfigIni); //对应的数据映射到dataCacheMapLib
    }

     /* config.ini 初始化 */
    INI_CONFIG_CTRL    *pconfigIni;     //config.ini
    for (uint j=0; j < sizeof(strFirstWidgetName)/(sizeof(QString)); j++) //sizeof(strWidgetNameLib)/(sizeof(QString)):功能模块控件的数量
    {
        pconfigIni = new INI_CONFIG_CTRL;

        /* 写入config.ini配置文件的默认部分 :type和library对应的值*/
        pconfigIni->ctlType = widgetType[j];              // 控件类型
        pconfigIni->settingName = strFirstKeyName[j];     // section/key
        pconfigIni->value = strFirstValue[j];             // section/val

        dataCacheMapConfig.insert(strFirstWidgetName[j], pconfigIni); //对应的数据映射到dataCacheMapLib
    }

}

/**********************************************************************
* 功    能： 读config.ini,存入缓存区map(dataCacheMapConfig)
* 输    入：
* 输    出：
* 作    者：刘卫明
* 编写日期：2017.12.6
***********************************************************************/
QCacheMapConfg DataCache::configIniReadCache()
{
    QSettings        *configIniRead = new QSettings("../bin/config.ini", QSettings::IniFormat);
    INI_CONFIG_CTRL  *pconfigIniTmp;

    QCacheMapConfg::iterator it;
    for (it = dataCacheMapConfig.begin(); it != dataCacheMapConfig.end(); ++it)
    {
        pconfigIniTmp = it.value();

        pconfigIniTmp->value = configIniRead->value(pconfigIniTmp->settingName, pconfigIniTmp->value).toString();

        //dataCacheMapConfig[it.key()] = pconfigIniTmp; //使用结构体指针不需要这个赋值。更新map数据
    }

    return  dataCacheMapConfig;
}


/**********************************************************************
* 功    能： 把map缓冲区的值写入config.ini
* 输    入：
* 输    出：
* 作    者：刘卫明
* 编写日期：2017.12.7
***********************************************************************/
void DataCache::configIniWriteCache(QCacheMapConfg cacheMapConfg)
{
    QSettings          *configIniWrite = new QSettings("../bin/config.ini", QSettings::IniFormat);
    INI_CONFIG_CTRL    *pconfigIniTmp;

    QCacheMapConfg::iterator it;
    for (it = cacheMapConfg.begin(); it != cacheMapConfg.end(); ++it)
    {
        pconfigIniTmp = it.value();

        /* 写入config.ini配置文件 */
        configIniWrite->setValue(pconfigIniTmp->settingName, pconfigIniTmp->value);
    }

    delete configIniWrite;
}


/**********************************************************************
* 功    能： 读libconfig.ini,存入缓存区map(dataCacheMapLib)
* 输    入：
* 输    出：
* 作    者：刘卫明
* 编写日期：2017.11.27
***********************************************************************/
QCacheMapLib DataCache::libConfigIniReadCache()
{
    QSettings          *configIniRead = new QSettings("../bin/libconfig.ini", QSettings::IniFormat);
    ////判断是否读到配置文件内容
    INI_LIBCONFIG_CTRL *plibConfigIniTmp;

    QCacheMapLib::iterator it;
    for (it = dataCacheMapLib.begin(); it != dataCacheMapLib.end(); ++it)
    {
        plibConfigIniTmp = it.value();
        for (int i=0; i<3; i++)
        {
            plibConfigIniTmp->value[i] = configIniRead->value(plibConfigIniTmp->settingName[i], plibConfigIniTmp->value[i]).toString();
        }

        //dataCacheMapLib[it.key()] = plibConfigIniTmp; //使用结构体指针不需要这个赋值。更新map数据
    }

    return  dataCacheMapLib;
}

/**********************************************************************
* 功    能： 把map缓冲区的值写入libconfig.ini
* 输    入：
* 输    出：
* 作    者：刘卫明
* 编写日期：2017.11.27
***********************************************************************/
void DataCache::libConfigIniWriteCache(QCacheMapLib cacheMapLib)
{
    QSettings          *configIniWrite = new QSettings("../bin/libconfig.ini", QSettings::IniFormat);
    INI_LIBCONFIG_CTRL *plibConfigIniTmp;

    QCacheMapLib::iterator it;
    for (it = cacheMapLib.begin(); it != cacheMapLib.end(); ++it)
    {
        plibConfigIniTmp = it.value();

        /* 写入libconfig.ini配置文件 */
        for (int i=0; i<3; i++)
        {
            configIniWrite->setValue(plibConfigIniTmp->settingName[i], plibConfigIniTmp->value[i]);
        }
    }

    delete configIniWrite;
}
