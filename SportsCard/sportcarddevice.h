#ifndef SPORTCARDDEVICE_H
#define SPORTCARDDEVICE_H

#include <QObject>
#include <QThread>

#include "zauxdll.h"
#include "sportcard_define.h"

class SportCardDevice : public QThread
{
    Q_OBJECT
public:
    explicit SportCardDevice(QThread *parent = 0);

    ~SportCardDevice();

    void SetID(int n_ID);

    int GetID();

    void SetDeviceIP(const QString &str_IP);

    bool ConnectDevice();

    bool DisconnectDevice();

    bool GetConnectState();

    bool Cmd_GetRemoveFullBox();

    bool Cmd_GetRemoveEmptyBox();

    bool Cmd_SendFullReady();

    bool Cmd_SendEmptyReady();

    bool WriteProductInfo(QList<PRODUCTINFO_> &list_ProductInfo);

    bool GetProductInfo(QList<PRODUCTINFO_> &list_ProductInfo);

    void UploadCmdSwitch(const bool &b_Switch);

private:
    bool GetData_uc(const ushort &us_Start,
                    const ushort &us_Num,
                    uchar *p_ucData);

    bool WriteData_uc(const ushort &us_Start,
                      const ushort &us_Num,
                      uchar * p_ucData);

    bool GetData_f(const ushort &us_Start,
                   const ushort &us_Num,
                   float *p_fData);

    bool WriteData_f(const ushort &us_Start,
                     const ushort &us_Num,
                     float * p_fData);

    void GetSportCardCmd();

private:
    void InitDevice();

    void WorkSleep(int n_Msec);

protected:
    void run();

signals:
    void sig_ConnectState(int n_ID, bool b_State);

    void sig_CmdData(int n_ID, SPC_SWAP_CMD Cmd);

    void sig_Cmd(int n_ID, SWAPCMD cmd);

public slots:

private slots:

private:
    ZMC_HANDLE m_hDevice;

    QString m_strIP;
    bool m_bConnectState;
    bool m_bRunSwitch;

    int m_nID;

    SPC_SWAP_CMD m_LastSwapCmd;
    SPC_SWAP_CMD m_CurrentSwapCmd;

    bool m_bUploadCmdSwitch;
};

#endif // SPORTCARDDEVICE_H
