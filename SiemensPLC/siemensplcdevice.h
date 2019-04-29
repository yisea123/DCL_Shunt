#ifndef SIEMENSPLCDEVICE_H
#define SIEMENSPLCDEVICE_H

#include <QObject>
#include <QThread>

#include "snap7.h"
#include "dcl_plc_define.h"

class SiemensPLCDevice : public QThread
{
    Q_OBJECT
public:
    explicit SiemensPLCDevice(QThread *parent = 0);
    ~SiemensPLCDevice();

    void SetDeviceIP(QString str_IP);

//    void SetDeviceID(int n_ID);

    void SetWriteParameter(int n_DBNumber, int n_Start);
    void SetReadParameter(int n_DBNumber, int n_Start);

    void ConnectDevice();

    void DisconnectDevice();

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

    int m_WriteDBNumber;
    int n_WriteStart;
    int n_ReadDBNumber;
    int n_ReadStart;

    bool m_bConnectState;
};

#endif // SIEMENSPLCDEVICE_H
