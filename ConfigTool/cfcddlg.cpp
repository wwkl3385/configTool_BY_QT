#include "cfcddlg.h"
#include "ui_cfcddlg.h"

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
    ui->CFCDTableWidget->setColumnCount(5);
    ui->CFCDTableWidget->horizontalHeader()->setDefaultSectionSize(110);

    QStringList header;
    header << "时段" << "起始时间" << "结束时间" << "SOC限值" << "电流限值";
    ui->CFCDTableWidget->setHorizontalHeaderLabels(header);

    /*设置表头字体加粗*/
    QFont font = this->font();
    font.setBold(true);
    ui->CFCDTableWidget->horizontalHeader()->setFont(font);

    ui->CFCDTableWidget->horizontalHeader()->setStretchLastSection(true); //设置充满表宽度
    ui->CFCDTableWidget->setStyleSheet("selection-background-color:lightblue;");


    ui->CFCDTableWidget->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

}

cfcdDlg::~cfcdDlg()
{
    delete ui;
}

void cfcdDlg::on_CFCDEnableCheckBox_clicked(bool checked)
{
        ui->periodComboBox->setEnabled(checked);
        ui->priorityCheckBox->setEnabled(checked);
        ui->startHourComboBox->setEnabled(checked);
        ui->startMinuteComboBox->setEnabled(checked);
        ui->stopHourComboBox->setEnabled(checked);
        ui->stopMinuteComboBox->setEnabled(checked);
        ui->SOCLineEdit->setEnabled(checked);
        ui->CFCDAddPushButton->setEnabled(checked);
        ui->CFCDDeletePushButton->setEnabled(checked);
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
}
