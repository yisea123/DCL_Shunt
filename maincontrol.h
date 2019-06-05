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

    //�ϴ���Ʒ��Ŵ�������󣬻�Ʒ�Ͽ���Ϣ
    bool MES_UploadBadProductList(const int &n_ScanerId,
                                  const QString &str_Code,
                                  const QList<PRODUCTINFO> &list_Product);

    //������ݺ�˳��ڷ��� ����¿� �뷴����
    bool MES_RequestExportDirection(const int &n_ScanerId,
                                    const QString &str_Code);

    //��ɷּ�Ʒ
    bool MES_CompleteSortingBadProduct(const int &n_ScanerId,
                                       const QString &str_Code);

    //�����ƷƷ����Ϣ
    bool MES_RequestProductListInfo(const int &n_ScanerId,
                                    const QString &str_Code);

    //�����Ƿ�Ϊ�տ�
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
    //ɨ��ǹ�豸����״̬
    void sig_ScanerConnectState(int n_ID,
                                bool b_ConnectState);

    //�˶��忨�豸����״̬
    void sig_SportCardConnectState(int n_ID, bool b_State);

    //plc�豸����״̬
    void sig_PLCConnectState(bool b_Connect);

    //MES�������������״̬
    void sig_Result_Heartbeat(MESRESPOND RespondState);

    //�쳣֪ͨ
    void sig_AnormalWork(int n_STtion, ANORMALWORK ERROR);

    void sig_ReadPLCData(PLC_DATAAREA PLCData);

    void sig_SportCardCmdData(int n_ID, SPC_SWAP_CMD Cmd);

    void sig_ScanCodeData(int n_ID, QString str_Code);

    void sig_MesWork(int n_ID,  bool b_State);

public slots:



private slots:
    //ɨ������
    void slot_ScanCodeData(int n_ID,
                           bool b_ScanState,
                           QString str_Code);

    //�豸����״̬
    void slot_ScanerConnectState(int n_ID,
                                 bool b_ConnectState);
private slots:
    //MES����������ϴ���Ʒ��Ŵ�������󣬻�Ʒ�Ͽ���Ϣ
    void slot_Result_SendBadProductList(int n_ScanerId,
                                        QString str_Code,
                                        MESRESPOND RespondState);

    //MES������������ݺ�˳��ڷ���
    void slot_Result_RequestExportDirection(int n_ScanerId,
                                            QString str_Code,
                                            EXPORTDIRECTION Direction,
                                            MESRESPOND RespondState);

    //MES�������������״̬
    void slot_Result_Heartbeat(MESRESPOND RespondState);

    //MES�����������ɷּ�Ʒ
    void slot_Result_CompleteSortingBadProduct(int n_ScanerId,
                                               QString str_Code,
                                               MESRESPOND RespondState);

    //MES�����������ƷƷ����Ϣ
    void slot_Result_RequestProductListInfo(int n_ScanerId,
                                            QString str_Code,
                                            QList<PRODUCTINFO> list_Product,
                                            MESRESPOND RespondState);

    //MES����������Ƿ�����
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
