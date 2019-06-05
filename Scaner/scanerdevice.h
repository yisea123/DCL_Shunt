#ifndef SCANERDEVICE_H
#define SCANERDEVICE_H

#include <QObject>
#include <QTcpSocket>
#include "scandatamanage.h"
#include <QMutex>
#include <QMutexLocker>

const char REQUESTCODE[3]       = {0x16, 0x54, 0x0D};
const int REQUESTCODE_TIMEOUT   = 2500;

class ScanerDevice : public QTcpSocket
{
    Q_OBJECT
public:
    explicit ScanerDevice(QObject *parent = 0);
    ~ScanerDevice();

    bool SetDeviceID(int n_ID);

    int GetDeviceID();

    bool SetDeviceIPAndPort(const QString &str_IP,
                            const int &n_Port);

    void GetSetDeviceIPAndPort(QString &str_IP,
                               int &n_Port);

private:
    bool ConnectDevice();

    bool DisConnectDevice();

public:
    bool GetConnect();

    QString GetCodeData();


private:
    bool RequestCode(const int &n_ID);

    void WorkSleep(int n_Msec);

signals:
    void sig_CodeData(int n_ID,
                      bool b_ScanState,
                      QString str_Code);

    void sig_ConnectState(int n_ID,
                          bool b_ConnectState);

public slots:    
    void slot_ConnectDevice(int n_ID);

    void slot_DisconnectDevice(int n_ID);

    void slot_RequestCode(int n_ID);

private slots:
    void slot_ScanData();

    void slot_Connect();

    void slot_Disconnect();

    void slot_CodeData(QByteArray byte_CodeData);

private:
    QTcpSocket *m_pTcpSocket;
    ScanDataManage m_oScanDataManage;

    int m_nDeviceID;
    QString m_strIP;
    int m_nPort;
    bool m_bConnectState;

    int m_nWriteTime;
    int m_nScanTime;
    bool m_bSacnResponse;

    QString m_strCodeData;

    QMutex m_oQMutex;
};

#endif // SCANERDEVICE_H
