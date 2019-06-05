#ifndef SCANERDEBUGVIEW_H
#define SCANERDEBUGVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QButtonGroup>

#include "ui_define.h"
#include "Scaner/scanermanager.h"

namespace Ui {
class ScanerDebugView;
}

class ScanerDebugView : public QWidget
{
    Q_OBJECT

public:
    explicit ScanerDebugView(ScanerManager *&p_ScanerManager,QWidget *parent = nullptr);
    ~ScanerDebugView();

    bool AddScaner(const int &n_ID);

    bool RemoveSacner(const int &n_ID);

    bool RequsetCode(const int &n_ID);

private:
    void InitView();

    void ErrorBox(QString str_Info);

private slots:
    void slot_ButtonClick(int n_ID);


private slots:
    //扫码数据
    void slot_ScanCodeData(int n_ID,
                           bool b_ScanState,
                           QString str_Code);

    //设备连接状态
    void slot_ScanerConnectState(int n_ID,
                                 bool b_ConnectState);


private:
    Ui::ScanerDebugView *ui;
    ScanerManager *m_pScanerManager;

    QList<QLabel *> m_listState;
    QList<QLineEdit *> m_listCode;

    QButtonGroup *m_pButtonGroup;

    int m_nClickID;

    QList<QString> m_listScanerIP;
};

#endif // SCANERDEBUGVIEW_H
