#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <maincontrol.h>
#include <QLabel>
#include <QButtonGroup>
#include <QMouseEvent>

#include <UI/scanerdebugview.h>
#include <UI/plcdebugview.h>
#include <UI/sportcarddebugview.h>
#include <UI/mesdebugview.h>
#include <UI/deviceip.h>
#include <UI/logviewinstancegetter.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void InitMW();

    void ExitMW();

    void ErrorBox(QString str_Info);

private:
    bool PrepareAutoWork();

    bool StartAutoWork();

    bool PauseAutoWork();

    bool StopAutoWork();

    void SetMesType(MESURLTYPE MESUrl_Type);

    void ScanerDebugUI();

    void PLCDebugUI();

    void MESDebugUI();

    void SportCardDebugUI();

    void DeviceIPUI();

    void CurrentLog();

protected:
//    void mouseMoveEvent(QMouseEvent *event);

    void closeEvent(QCloseEvent *event);

private slots:
    void slot_ButtonClick(int n_ID);

private slots:
    //扫码枪设备连接状态
    void slot_ScanerConnectState(int n_ID,
                                bool b_ConnectState);

    //运动板卡设备连接状态
    void slot_SportCardConnectState(int n_ID, bool b_State);

    //plc设备连接状态
    void slot_PLCConnectState(bool b_Connect);

    //MES反馈结果，心跳状态
    void slot_Result_Heartbeat(MESRESPOND RespondState);

    //异常通知
    void slot_AnormalWork(int n_Station, ANORMALWORK ERROR);

    void slot_ReadPLCData(PLC_DATAAREA PLCData);

    void slot_SportCardCmdData(int n_ID, SPC_SWAP_CMD Cmd);

    void slot_ScanCodeData(int n_ID, QString str_Code);

    void slot_MesWork(int n_ID,  bool b_State);

private:
    Ui::MainWindow *ui;

    MainControl *m_pMainControl;

    LogViewInstanceGetter m_oLogViewInstanceGetter;
    LogView *m_pLogView;

    QList<QLabel *> m_listScanerStateLaber;
    QList<QLabel *> m_listPLCStateLaber;
    QList<QLabel *> m_listMESStateLaber;
    QList<QLabel *> m_listSportCardStateLaber;

    QList<QLabel *> m_listScanerInfoLaber;
    QList<QLabel *> m_listPLCInfoLaber;
    QList<QLabel *> m_listMESInfoLaber;
    QList<QLabel *> m_listSportCardInfoLaber;

    QButtonGroup *m_pButtonGroup;
    int m_nClickID;
};

#endif // MAINWINDOW_H
