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

 /* 功能模块 UI中CheckBox控件的名称 */
 // "settingCheckBox"  , "updateCheckBox"          , "realDataCheckBox"  ,
 // "cacheCheckBox"    , "DatabaseCheckBox"        , "logCheckBox"       ,   // 业务选择模块(默认选择项，不做处理)
const QString strWidgetNameLib[] = {"updateCheckBox" ,                       //0:升级服务
    "chargeCheckBox"   , "scheduleCheckBox"        , "devManageCheckBox",    //1-3:业务选择模块选择按钮名称:1-3
    "remoteCheckBox"   , "localCheckBox"           , "webServerCheckBox",    //4-6
    "equipmentCheckBox", "ammeteCheckBox"          , "humitureCheckBox" ,    //7-9
    "screenCheckBox"   , "screenSinglePileCheckBox", "gpioCheckBox"     ,    //10-12
    "ticketCheckBox"   , "scannerCheckBox"         , "cardCheckBox"     };   //13-15 设备模块选择按钮名称：4-15

 /* 功能模块 libconfig.ini 对应的文本信息  依次为：type  library  enable  库的名字*.so  */
const QString strKeyNameLib[][4] = {
    { "UPDATE/type"          , "UPDATE/library"          , "UPDATE/enable"          , "libUpdate.so"                }, //0  : 升级服务
    { "CHARGESERVICE/type"   , "CHARGESERVICE/library"   , "CHARGESERVICE/enable"   , "libChargeService.so"         }, //1  : 充电服务
    { "SCHEDULE/type"        , "SCHEDULE/library"        , "SCHEDULE/enable"        , "libLoadSchedule.so"          }, //2  : 负荷调度
    { "DEVICEMANAGE/type"    , "DEVICEMANAGE/library"    , "DEVICEMANAGE/enable"    , "libDeviceManage.so"          }, //3  : 设备管理
    { "SERVER0/type"         , "SERVER0/library"         , "SERVER0/enable"         , "libIEC104Server.so"          }, //4  : 云平台
    { "SERVER1/type"         , "SERVER1/library"         , "SERVER1/enable"         , "libIEC104Server.so"          }, //5  : 场站平台
    { "WEBSERVER/type"       , "WEBSERVER/library"       , "WEBSERVER/enable"       , "libWebServer.so"             }, //6  : Web平台
    { "CHARGEEQUIPMENT/type" , "CHARGEEQUIPMENT/library" , "CHARGEEQUIPMENT/enable" , "libChargeEquipment.so"       }, //7  : 充电设备
    { "AMMETER/type"         , "AMMETER/library"         , "AMMETER/enable"         , "libAmmeter.so"               }, //8  : 电表
    { "TEMPHUMI/type"        , "TEMPHUMI/library"        , "TEMPHUMI/enable"        , "libTempHumi.so"              }, //9  : 温度计
    { "SCREEN/type"          , "SCREEN/library"          , "SCREEN/enable"          , "libSerialScreen.so"          }, //10 : 显示屏
    { "SCREENSINGLEPILE/type", "SCREENSINGLEPILE/library", "SCREENSINGLEPILE/enable", "libSerialScreenSinglePile.so"}, //11 : 显示屏-直流单桩
    { "GPIO/type"            , "GPIO/library"            , "GPIO/enable"            , "libGPIO.so"                  }, //12 : GPIO设备
    { "TICKET/type"          , "TICKET/library"          , "TICKET/enable"          , "libTicketDev.so"             }, //13 : 小票机
    { "SCANCODE/type"        , "SCANCODE/library"        , "SCANCODE/enable"        , "libScanCode.so"              }, //14 : 扫码器
    { "CARD/type"            , "CARD/library"            , "CARD/enable"            , "libCard.so"                 }}; //15 : 刷卡器


 /* 配置选项一 UI中控件的名称 */
const QString strWidgetName[] = {
    "directLineEdit"         , "singleLineEdit"          , "threeLineEdit"                 ,
    "passwordLineEdit"       , "teuiTypeComboBox"        , "boardComboBox"                 ,  // 0-5 :CSCU系统配置
    "cardTypeComboBox"       , "vinTypeComboBox"         , "AbnormalOpenRadioButton"       ,
    "cardAutoOpenRadioButton", "vinAutoOpenRadioButton"  , "buttonOpenRadioButton"         ,
    "ticketOpenRadioButton"  , "cardMultiOpenRadioButton", "singlePileGroupOpenRadioButton"};  // 6-14 :充电服务配置


 /* 配置选项一 config.ini 中对应的文本信息*/
const QString strKeyName[] = {
    "CSCUSys/DirectCurrent", "CSCUSys/SinglePhase"              , "CSCUSys/ThreePhase" ,  // 0-2: 直流终端数量，单相终端数量，三相终端数量
    "CSCUSys/Password"     , "CSCUSys/NormalCard"               , "CSCUSys/BoardType"  ,  // 3-5: 设置密码，交互类型，底板类型
    "CHARGE/CardType"      , "CHARGE/VinType"                   , "CHARGE/EnergyFilter",  // 6-8: 卡片类型，车牌号/VIN,异常电量过滤
    "CHARGE/CardAuto"      , "CHARGE/VinAuto"                   , "CHARGE/LocalStop"   ,  // 9-11: 刷卡触发充电，VIN触发充电，结束充电按钮
    "CHARGE/TicketEnable"  , "CHARGE/SwitpingCardMultChooseMode", "CHARGE/DevType"     }; // 12-14: 打印小票，刷卡多选模式，单桩群充模式


QCacheMapLib   dataCacheMapLib;    //libconfig.ini文件的 map数据缓冲区
//QCacheMapConfg dataCacheMapConfig; //config.ini文件的 map数据缓冲区

/**********************************************************************
* 功    能：DataCache构造函数，创建一个map:dataCacheMaplib缓存区并初始化
* 输    入：
* 输    出：
* 作    者：刘卫明
* 编写日期：2017.11.27
***********************************************************************/
DataCache::DataCache(QObject *parent) : QObject(parent)
{
    INI_LIBCONFIG_CTRL *plibConfigIni;

    for(uint i=0; i<MODULE_NUM; i++)
    {
        plibConfigIni = new  INI_LIBCONFIG_CTRL;

        /* 写入libconfig.ini配置文件的默认部分 :type和library对应的值*/
        plibConfigIni->value[0] = "device";            //section/key
        plibConfigIni->value[1] = strKeyNameLib[i][3]; // section/library

        /* 段名/键名 */
        for(int j=0; j<3; j++)
        {
            plibConfigIni->settingName[j] = strKeyNameLib[i][j];
        }

        dataCacheMapLib.insert(strWidgetNameLib[i], plibConfigIni); //对应的数据映射到dataCacheMapLib
    }
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
    INI_LIBCONFIG_CTRL *plibConfigIniTmp;

    QCacheMapLib::iterator it;
    for(it = dataCacheMapLib.begin(); it != dataCacheMapLib.end(); ++it)
    {
        plibConfigIniTmp = it.value();

        /*只读取enable项对应的值*/
        plibConfigIniTmp->value[2] = configIniRead->value(plibConfigIniTmp->settingName[2]).toString();

        //dataCacheMapLib[it.key()] = plibConfigIniTmp; //使用结构体指针不需要这个赋值。
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
    for(it = cacheMapLib.begin(); it != cacheMapLib.end(); ++it)
    {
        plibConfigIniTmp = it.value();

        /* 写入libconfig.ini配置文件 */
        for(int i=0; i<3; i++)
        {
            configIniWrite->setValue(plibConfigIniTmp->settingName[i], plibConfigIniTmp->value[i]);
        }
    }

    delete configIniWrite;
}
