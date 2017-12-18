#ifndef CFCDDLG_H
#define CFCDDLG_H

#include <QDialog>
#include <QVector>
#include "datacache.h"
#include "../include/sqlite3.h"

typedef struct _CFCD_INFO
{
    int iId;
    int iPeriod;
    int iStartHour;
    int iStartMinute;
    int iStopHour;
    int iStopMinute;
    int iSoc;
    int iCurrent;
    int iStatus;
}CFCD_INFO;

typedef QVector<CFCD_INFO> QCFCDvector;

namespace Ui {
class cfcdDlg;
}

class cfcdDlg : public QDialog
{
    Q_OBJECT

public:
    explicit cfcdDlg(QWidget *parent = 0);
    ~cfcdDlg();

    QCFCDvector      m_arCache[4];
    DataCache        Cache;
    QCacheMapConfg   mapConfig;    //config.ini map数据缓存区
    QStringList      m_listCFCD;

private slots:
    void on_CFCDEnableCheckBox_clicked(bool checked);
    void on_CFCDSavePushButton_clicked();
    void ReadConfigDatabase(void);
    void WriteConfigDatabase(void);
    void on_CFCDAddPushButton_clicked();
    void on_periodComboBox_currentIndexChanged(int index);

private:
    Ui::cfcdDlg *ui;

};

#endif // CFCDDLG_H
