#include "cfcddlg.h"
#include "ui_cfcddlg.h"

#include<QMessageBox>
#include<QDebug>

cfcdDlg::cfcdDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::cfcdDlg)
{
    ui->setupUi(this);

    setWindowTitle("错峰充电设置");
    this->setFixedSize(615, 400);           //设置窗口尺寸

    /* 时段设置 */
    for (int i=0; i<24; i++)
    {
        ui->startHourComboBox->insertItem(i, QString::number(i, 10));
        ui->stopHourComboBox->insertItem(i, QString::number(i, 10));
    }
    for (int i=0; i<60; i+=30)
    {
        ui->startMinuteComboBox->insertItem(i, QString::number(i, 10));
        ui->stopMinuteComboBox->insertItem(i, QString::number(i, 10));
    }

//        ui->CFCDAddPushButton->setEnabled(false);
//        ui->CFCDDeletePushButton->setEnabled(false);

    /*config.ini 读缓存区数据，写入控件*/
    mapConfig = Cache.configIniReadCache();

    INI_CONFIG_CTRL *configIni;

    configIni = mapConfig["CFCDEnableCheckBox"];
    ui->CFCDEnableCheckBox->setChecked(configIni->value == "true" ? true : false);

    configIni = mapConfig["priorityCheckBox"];
    ui->priorityCheckBox->setChecked(configIni->value == "true" ? true : false);

    /* 默认状态 ：错峰充电未启用时*/
    if (ui->CFCDEnableCheckBox->checkState() == Qt::Unchecked)
    {
        ui->periodComboBox->setDisabled(true);
        ui->priorityCheckBox->setDisabled(true);
        ui->startHourComboBox->setDisabled(true);
        ui->startMinuteComboBox->setDisabled(true);
        ui->stopHourComboBox->setDisabled(true);
        ui->stopMinuteComboBox->setDisabled(true);
        ui->SOCLineEdit->setDisabled(true);
        ui->CFCDAddPushButton->setDisabled(true);
        ui->CFCDDeletePushButton->setDisabled(true);
        ui->currentLineEdit->setDisabled(true);
    }

    /*tableWidget 初始化设置*/
    ui->CFCDTableWidget->setColumnCount(5); //设置列数
    ui->CFCDTableWidget->horizontalHeader()->setDefaultSectionSize(110);
    ui->CFCDTableWidget->horizontalHeader()->setVisible(true);

    /*设置表头内容*/
    QStringList header;
    header << "时段" << "起始时间" << "结束时间" << "SOC限值" << "电流限值";
    ui->CFCDTableWidget->setHorizontalHeaderLabels(header);

    /*设置表头字体加粗*/
    QFont font = this->font();
    font.setBold(true);
    ui->CFCDTableWidget->horizontalHeader()->setFont(font);

    ui->CFCDTableWidget->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    ui->CFCDTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色

    ui->CFCDTableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->CFCDTableWidget->setFrameShape(QFrame::NoFrame); //设置无边框
    ui->CFCDTableWidget->setShowGrid(false); //设置不显示格子线
    ui->CFCDTableWidget->verticalHeader()->setVisible(false); //设置垂直头不可见
    ui->CFCDTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //整行选中的方式

    /*设置垂直滚动条样式*/
    ui->CFCDTableWidget->setStyleSheet("QScrollBar{background:transparent; width: 10px;}"
                                       "QScrollBar::handle{background:lightgray; border:2px solid transparent; border-radius:5px;}"
                                       "QScrollBar::handle:hover{background:gray;}"
                                       "QScrollBar::sub-line{background:transparent;}"
                                       "QScrollBar::add-line{background:transparent;}");
}

cfcdDlg::~cfcdDlg()
{
    delete ui;
}

/**********************************************************************
* 功    能：错峰充电按钮槽函数
* 输    入：
* 输    出：
* 作    者：刘卫明
* 编写日期：2017.12.2
***********************************************************************/
void cfcdDlg::on_CFCDEnableCheckBox_clicked(bool checked)
{
        ui->periodComboBox->setEnabled(checked);
        ui->priorityCheckBox->setEnabled(checked);
        ui->startHourComboBox->setEnabled(checked);
        ui->startMinuteComboBox->setEnabled(checked);
        ui->stopHourComboBox->setEnabled(checked);
        ui->stopMinuteComboBox->setEnabled(checked);
        ui->SOCLineEdit->setEnabled(checked);
//        ui->CFCDAddPushButton->setEnabled(checked);
//        ui->CFCDDeletePushButton->setEnabled(checked);
        ui->currentLineEdit->setEnabled(checked);

        /* 不启用错峰充电功能时， 禁用车辆优先级*/
        if (checked == false)
            ui->priorityCheckBox->setChecked(false);
}

void cfcdDlg::on_CFCDSavePushButton_clicked()
{
    /* 写config.ini*/
    INI_CONFIG_CTRL *configIni;

    configIni = mapConfig["CFCDEnableCheckBox"];
    configIni->value = ui->CFCDEnableCheckBox->checkState() == Qt::Checked ? "true" : "false";

    configIni = mapConfig["priorityCheckBox"];
    configIni->value = ui->priorityCheckBox->checkState() == Qt::Checked ? "true" : "false";

    Cache.configIniWriteCache(mapConfig);
    this->close();
}

void cfcdDlg::ReadConfigDatabase()
{
    sqlite3  *db     = nullptr;
    QString  strSql;
    char     *err    = NULL;
    char     **result;
    int      row     = 0;
    int      col     = 0;

    sqlite3_open("param_config.db", &db);
    if (!db)
    {
        QMessageBox::information(this, "提示", "open database failed!");
        return;
    }

    strSql = "create table if not exists tpfv_param_table(id integer primary key autoincrement, time_seg integer, start_hour integer,"
             " start_minute integer,stop_hour integer, stop_minute integer, limit_soc integer, limit_current integer);";
    if (sqlite3_exec(db, strSql.toUtf8().constData(), NULL, NULL, &err) != SQLITE_OK)
    {
        QMessageBox::information(this, "提示", "Query table tpfv_param_table failed");
        sqlite3_free(err);
        sqlite3_close(db);
        return;
    }

    strSql = "SELECT * FROM tpfv_param_table";
    if (sqlite3_get_table(db, strSql.toUtf8().constData(), &result, &row, &col, &err) != SQLITE_OK)
    {
        QMessageBox::information(this, "提示", "Query table tpfv_param_table failed");
        sqlite3_free(err);
        sqlite3_close(db);
        return;
    }

    for (int i=0; i<row; i++)
    {
        CFCD_INFO info;
        memset(&info, 0, sizeof(CFCD_INFO));
        info.iId = atoi(result[col++]);
        info.iPeriod = atoi(result[col++]);
        info.iStartHour = atoi(result[col++]);
        info.iStartMinute = atoi(result[col++]);
        info.iStopHour = atoi(result[col++]);
        info.iStopMinute = atoi(result[col++]);
        info.iSoc = atoi(result[col++]);
        info.iCurrent = atoi(result[col++]);
        m_arCache[info.iPeriod - 1].push_back(info);
    }

    sqlite3_free_table(result);
    sqlite3_close(db);

}

void cfcdDlg::WriteConfigDatabase()
{
#if 0
    CFCD_INFO info;
    QString  strSql;
    sqlite3   *db = NULL;
    char*     err;

    sqlite3_open("param_config.db", &db);
    if (!db)
    {
        QMessageBox::information(this, "提示", "Query table tpfv_param_table failed");
        return;
    }
    strSql = "DELETE FROM tpfv_param_table;";
    if (sqlite3_exec(db, strSql.toUtf8().constData(), NULL, NULL, &err) != SQLITE_OK)
    {
        QMessageBox::information(this, "提示", "Query table tpfv_param_table failed");
        sqlite3_free(err);
        sqlite3_close(db);
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < m_arCache[i].length(), j++)
        {
            info = m_arCache[i].GetAt(j);
            strSql.Format("INSERT INTO tpfv_param_table(time_seg, start_hour, start_minute, stop_hour, \
                          stop_minute, limit_soc, limit_current) VALUES(%d, %d, %d, %d, %d, %d, %d);",
            info.iPeriod, info.iStartHour, info.iStartMinute, info.iStopHour, info.iStopMinute, info.iSoc, info.iCurrent);
            if (sqlite3_exec(db, (LPCSTR)strSql, NULL, NULL, &err) != SQLITE_OK)
            {
                AfxMessageBox(_T("Create table tpfv_param_table failed!"));
                sqlite3_free(err);
                sqlite3_close(db);
                return;
            }
        }
    }
#endif
}

void cfcdDlg::on_CFCDAddPushButton_clicked()
{
    QString strTime;
    int     iCount = 0;

    CFCD_INFO info;
    memset(&info, 0, sizeof(CFCD_INFO));
    info.iPeriod = ui->periodComboBox->currentIndex();
    info.iStartHour = ui->startHourComboBox->currentText().toInt();
    info.iStartMinute = ui->startMinuteComboBox->currentText().toInt();
    info.iStopHour = ui->stopHourComboBox->currentText().toInt();
    info.iStopMinute = ui->stopMinuteComboBox->currentText().toInt();
    info.iSoc = ui->SOCLineEdit->text().toInt();
    info.iCurrent = ui->currentLineEdit->text().toInt();

    iCount = ui->CFCDTableWidget->rowCount();
    if (iCount >= 5)
    {
        QMessageBox::information(this, "提示", "最多设置五个时间段");
        return;
    }
    ui->CFCDTableWidget->insertRow(iCount);

    QTableWidgetItem *item0 = new QTableWidgetItem();
    QTableWidgetItem *item1 = new QTableWidgetItem();
    QTableWidgetItem *item2 = new QTableWidgetItem();
    QTableWidgetItem *item3 = new QTableWidgetItem();
    QTableWidgetItem *item4 = new QTableWidgetItem();

    item0->setText(ui->periodComboBox->currentText());
    strTime.sprintf("%02d:%02d", ui->startHourComboBox->currentText().toInt(), ui->startMinuteComboBox->currentText().toInt());
    item1->setText(strTime);
    strTime.sprintf("%02d:%02d", ui->stopHourComboBox->currentText().toInt(), ui->stopMinuteComboBox->currentText().toInt());
    item2->setText(strTime);
    item3->setText(ui->SOCLineEdit->text());
    item4->setText(ui->currentLineEdit->text());

    ui->CFCDTableWidget->setItem(iCount, 0, item0);
    ui->CFCDTableWidget->setItem(iCount, 1, item1);
    ui->CFCDTableWidget->setItem(iCount, 2, item2);
    ui->CFCDTableWidget->setItem(iCount, 3, item3);
    ui->CFCDTableWidget->setItem(iCount, 4, item4);

    m_arCache[ui->periodComboBox->currentIndex() - 1].push_back(info);

//    qDebug() <<ui->periodComboBox->currentIndex();
//    qDebug() << m_arCache[0].count();
//    qDebug() << m_arCache[1].count();
//    qDebug() << m_arCache[2].count();
//    qDebug() << m_arCache[3].count();

}


void cfcdDlg::on_periodComboBox_currentIndexChanged(int index)
{
    int iFirst;
    int iLast;
    ui->CFCDTableWidget->clearContents();
    ui->CFCDTableWidget->setRowCount(0);

    if (index == 0)
    {
        ui->CFCDAddPushButton->setDisabled(true);
        ui->CFCDDeletePushButton->setDisabled(true);
        iFirst = 1;
        iLast = 5;
    }
    else
    {
        ui->CFCDAddPushButton->setEnabled(true);
        ui->CFCDDeletePushButton->setEnabled(true);
        iFirst = ui->periodComboBox->currentIndex();
        iLast= ui->periodComboBox->currentIndex() + 1;
    }
#if 0
    CFCD_INFO info;
    QString   str = nullptr;
    int       iCount = 0;
    for (int i = iFirst; i < iLast; i++)
    {
        for (int j = 0; j < m_arCache[i].length(); j++)
        {
            info   = m_arCache[i - 1].value(j);
            iCount = ui->CFCDTableWidget->rowCount();

            QTableWidgetItem *item0 = new QTableWidgetItem();
            QTableWidgetItem *item1 = new QTableWidgetItem();
            QTableWidgetItem *item2 = new QTableWidgetItem();
            QTableWidgetItem *item3 = new QTableWidgetItem();
            QTableWidgetItem *item4 = new QTableWidgetItem();

            item0->setText(str.sprintf("%d", info.iPeriod));
            str.sprintf("%02d:%02d", info.iStartHour, info.iStartMinute);
            item1->setText(str);
            str.sprintf("%02d:%02d", info.iStopHour, info.iStopMinute);
            item2->setText(str);
            item3->setText(str.sprintf("%d", info.iSoc));
            item4->setText(str.sprintf("%d", info.iCurrent));

            ui->CFCDTableWidget->setItem(iCount, 0, item0);
            ui->CFCDTableWidget->setItem(iCount, 1, item1);
            ui->CFCDTableWidget->setItem(iCount, 2, item2);
            ui->CFCDTableWidget->setItem(iCount, 3, item3);
            ui->CFCDTableWidget->setItem(iCount, 4, item4);
        }
    }
    #endif
}
