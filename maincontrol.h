#ifndef MAINCONTROL_H
#define MAINCONTROL_H

#include <QObject>

#include "Scaner/scanermanager.h"
#include "SD_MES/sdmesmanager.h"
#include "SiemensPLC/siemensplcdevice.h"
#include "SportsCard/sportcardmanager.h"

#include "dcl_shunt_define.h"

class MainControl : public QObject
{
    Q_OBJECT
public:
    explicit MainControl(QObject *parent = 0);
    ~MainControl();

    bool PrepareAutoWork();

    bool StartAutoWork();

    bool PauseAutoWork();

    bool ContinueAutoWork();

    bool StopAutoWork();

    WORKSTATE GetWorkState();

    bool SetMesType(MESURLTYPE MESUrl_Type = HEARTBEAT_CURRENTUSE);

public:
    ScanerManager * GetPoint_ScanerManager();
    SDMesManager * GetPoint_SDMesManager();
    SiemensPLCDevice * GetPoint_SiemensPLCDevice();
    SportCardManager * GetPoint_SportCardManager();

private:
    void SendAnormalWorkInfo(const int &n_Station, const ANORMALWORK &ERROR);

    bool RequsetCode(const int &n_ID);

    bool SendPLCCmd(const int &n_ID, const PC_CMD &cmd);

    bool SportCardCmd_GetRemoveFullBox(const int &n_ID);

    bool SportCardCmd_Cmd_GetRemoveEmptyBox(const int &n_ID);

    bool SportCardCmd_Cmd_SendFullReady(const int &n_ID);

    bool SportCardCmd_Cmd_SendEmptyReady(const int &n_ID);

    bool SportCardCmd_WriteProductInfo(const int &n_ID,
                                       QList<PRODUCTINFO_> &list_ProductInfo);

    bool SportCardCmd_GetProductInfo(const int &n_ID,
                                     QList<PRODUCTINFO_> &list_ProductInfo);

    //上传坏品存放处，满框后，坏品料框信息
    bool MES_UploadBadProductList(const int &n_ScanerId,
                                  const QString &str_Code,
                                  const QList<PRODUCTINFO> &list_Product);

    //请求分容后端出口方向 入高温库 入反分容
    bool MES_RequestExportDirection(const int &n_ScanerId,
                                    const QString &str_Code);

    //完成分拣坏品
    bool MES_CompleteSortingBadProduct(const int &n_ScanerId,
                                       const QString &str_Code);

    //请求产品品质信息
    bool MES_RequestProductListInfo(const int &n_ScanerId,
                                    const QString &str_Code);

    //请求是否为空框
    bool MES_RequestBoxIsEmpty(const int &n_ScanerId,
                               const QString &str_Code);


private:
    void InitMainControl();

    void ExitMainControl();

    bool ConnectAllDevice();

    void WorkSleep(int n_Msec);

    void PauseWork();

    bool GetProductbQuality(QList<PRODUCTINFO> &list_Product);

    void TranferData1(QList<PRODUCTINFO> &list_Product,
                     QList<PRODUCTINFO_> &list_ProductInfo_);


    void TranferData2(QList<PRODUCTINFO_> &list_ProductInfo_,
                     QList<PRODUCTINFO> &list_Product);

signals:
    //扫码枪设备连接状态
    void sig_ScanerConnectState(int n_ID,
                                bool b_ConnectState);

    //运动板卡设备连接状态
    void sig_SportCardConnectState(int n_ID, bool b_State);

    //plc设备连接状态
    void sig_PLCConnectState(bool b_Connect);

    //MES反馈结果，心跳状态
    void sig_Result_Heartbeat(MESRESPOND RespondState);

    //异常通知
    void sig_AnormalWork(int n_STtion, ANORMALWORK ERROR);

    void sig_ReadPLCData(PLC_DATAAREA PLCData);

    void sig_SportCardCmdData(int n_ID, SPC_SWAP_CMD Cmd);

    void sig_ScanCodeData(int n_ID, QString str_Code);

    void sig_MesWork(int n_ID,  bool b_State);

public slots:



private slots:
    //扫码数据
    void slot_ScanCodeData(int n_ID,
                           bool b_ScanState,
                           QString str_Code);

    //设备连接状态
    void slot_ScanerConnectState(int n_ID,
                                 bool b_ConnectState);
private slots:
    //MES反馈结果，上传坏品存放处，满框后，坏品料框信息
    void slot_Result_SendBadProductList(int n_ScanerId,
                                        QString str_Code,
                                        MESRESPOND RespondState);

    //MES反馈结果，分容后端出口方向
    void slot_Result_RequestExportDirection(int n_ScanerId,
                                            QString str_Code,
                                            EXPORTDIRECTION Direction,
                                            MESRESPOND RespondState);

    //MES反馈结果，心跳状态
    void slot_Result_Heartbeat(MESRESPOND RespondState);

    //MES反馈结果，完成分拣坏品
    void slot_Result_CompleteSortingBadProduct(int n_ScanerId,
                                               QString str_Code,
                                               MESRESPOND RespondState);

    //MES反馈结果，产品品质信息
    void slot_Result_RequestProductListInfo(int n_ScanerId,
                                            QString str_Code,
                                            QList<PRODUCTINFO> list_Product,
                                            MESRESPOND RespondState);

    //MES反馈结果，是否满框
    void slot_Result_RequestBoxIsEmpty(int n_ScanerId,
                                       QString str_Code,
                                       BOXSTATE BoxState,
                                       MESRESPOND RespondState);

private slots:
    void slot_PLCConnectState(bool b_Connect);

    void slot_ReadPLCData(PLC_DATAAREA PLCData);

    void slot_GetCmd(int n_Station, PLC_CMD cmd);


private slots:
    void slot_SportCardConnectState(int n_ID, bool b_State);

    void slot_SportCardCmdData(int n_ID, SPC_SWAP_CMD Cmd);

    void slot_SportCardCmd(int n_ID, SWAPCMD Cmd);

private:
    ScanerManager *m_pScanerManager;
    SDMesManager *m_pSDMesManager;
    SiemensPLCDevice *m_pSiemensPLCDevice;
    SportCardManager *m_pSportCardManager;

    WORKSTATE m_WorkState;

    QMap<int, QString> m_mapScanCode;
};

#endif // MAINCONTROL_H
