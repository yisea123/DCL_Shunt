#ifndef SIEMENSPLCDEVICE_H
#define SIEMENSPLCDEVICE_H

#include <QObject>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include "snap7.h"
#include "dcl_plc_define.h"

class SiemensPLCDevice : public QThread
{
    Q_OBJECT
public:
    explicit SiemensPLCDevice(QThread *parent = 0);
    ~SiemensPLCDevice();

    void InitSiemensPLC();

    void SetDeviceIP(QString str_IP);

//    void SetDeviceID(int n_ID);

    void SetWriteParameter(int n_DBNumber, int n_Start);
    void SetReadParameter(int n_DBNumber, int n_Start);

    bool ConnectDevice();

    bool DisconnectDevice();

    bool GetConnectState();

    bool GetReadData(PLC_DATAAREA &PLCData);

    bool GetWriteData(PC_DATAAREA &PCData);

    bool WriteData(PC_DATAAREA &PCData);

private:
    void WorkSleep(int n_Msec);

protected:
    void run();

signals:
    void sig_ConnectState(bool b_Connect);

    void sig_ReadData(PLC_DATAAREA PLCData);


public slots:

private slots:

private:
    TS7Client *m_pTS7Client;

    QString m_strIP;

    int m_nWriteDBNumber;
    int m_nWriteStart;
    int m_nReadDBNumber;
    int m_nReadStart;

    bool m_bConnectState;

    bool m_bRunSwitch;
};

#endif // SIEMENSPLCDEVICE_H
