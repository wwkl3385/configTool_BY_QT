#ifndef CFCDDLG_H
#define CFCDDLG_H

#include <QWidget>

namespace Ui {
class CFCDDlg;
}

class CFCDDlg : public QWidget
{
    Q_OBJECT

public:
    explicit CFCDDlg(QWidget *parent = 0);
    ~CFCDDlg();

private:
    Ui::CFCDDlg *ui;
};

#endif // CFCDDLG_H
