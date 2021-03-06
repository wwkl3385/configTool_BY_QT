/***********************************************************************
* Copyright (c) 2017, 青岛特来电新能源有限公司  All rights reserved.
*
* 文件名称：  datacache.h
* 描    述： 数据缓冲区，处理配置文件，实现读写，存取功能
* 修改记录：
* 			V0.6，2017-11-25，创建，刘卫明
*
***********************************************************************/

#ifndef DATACACHE_H
#define DATACACHE_H

#include <QObject>
#include <QMap>
#include<array>


/*UI控件类型的枚举*/
typedef enum _WIDGET_TYPE
{
    LINEEDIT_TYPE = 1,
    COMBOBOX_TYPE,
    RADIOBUTTON_TYPE,
    CHECKBOX_TYPE,
    CHECKBOX_TYPE_CFCD,           //错峰充电
}WIDGET_TYPE;

typedef struct _INI_LIBCONFIG_CTRL
{
    QString      settingName[3];  //配置文件libconfig.ini的section段
    QString      value[3];        //键值
}INI_LIBCONFIG_CTRL;

typedef QMap<QString, INI_LIBCONFIG_CTRL*> QCacheMapLib;

typedef struct _INI_CONFIG_CTRL
{
    QString      settingName;    //配置文件config.ini的section段
    QString      value;          //键值
    int          ctlType;        //UI控件的类型
}INI_CONFIG_CTRL;

typedef QMap<QString, INI_CONFIG_CTRL*> QCacheMapConfg;

class DataCache : public QObject
{
    Q_OBJECT
public:
    explicit DataCache(QObject *parent = nullptr);

    QCacheMapLib    libConfigIniReadCache();
    QCacheMapConfg  configIniReadCache();

    void libConfigIniWriteCache(QCacheMapLib cacheMapLib);
    void configIniWriteCache(QCacheMapConfg cacheMapConfg);

signals:

public slots:
};

#endif // DATACACHE_H
