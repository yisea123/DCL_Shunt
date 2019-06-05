#include "scanerdevice.h"
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QThread>
#include <QTimer>

ScanerDevice::ScanerDevice(QObject *parent)
    : QTcpSocket(parent)
{
    m_pTcpSocket    = NULL;
    m_nDeviceID     = 0;
    m_strIP         = "";
    m_nPort         = 0;
    m_nWriteTime    = 0;
    m_nScanTime     = 0;
    m_bSacnResponse = false;
    m_bConnectState = false;
    m_strCodeData   = "";
}

ScanerDevice::~ScanerDevice()
{
    DisConnectDevice();
}

bool ScanerDevice::SetDeviceID(int n_ID)
{
    m_nDeviceID = n_ID;
    return true;
}

int ScanerDevice::GetDeviceID()
{
    return m_nDeviceID;
}

bool ScanerDevice::SetDeviceIPAndPort(const QString &str_IP, const int &n_Port)
{
    m_strIP = str_IP;
    m_nPort = n_Port;

    return true;
}

void ScanerDevice::GetSetDeviceIPAndPort(QString &str_IP, int &n_Port)
{
    str_IP = m_strIP;
    n_Port = m_nPort;
}

bool ScanerDevice::ConnectDevice()
{   
    QMutexLocker o_Locker(&m_oQMutex);

    m_pTcpSocket = new QTcpSocket;
    connect(m_pTcpSocket, SIGNAL(connected()),
            this, SLOT(slot_Connect()));
    connect(m_pTcpSocket, SIGNAL(disconnected()),
            this, SLOT(slot_Disconnect()));
    connect(m_pTcpSocket, SIGNAL(readyRead()),
            this, SLOT(slot_ScanData()));

    connect(&m_oScanDataManage, SIGNAL(sig_CodeData(QByteArray)),
            this, SLOT(slot_CodeData(QByteArray)));

    if(m_bConnectState || m_pTcpSocket->state() != UnconnectedState){
        m_pTcpSocket->disconnectFromHost();
        m_pTcpSocket->waitForDisconnected(2000);
    }

    m_pTcpSocket->connectToHost(m_strIP, m_nPort);

    bool b_Ret = m_pTcpSocket->waitForConnected(5000);

    if(!b_Ret)
        emit sig_ConnectState(m_nDeviceID, false);

    return b_Ret;
}

bool ScanerDevice::DisConnectDevice()
{
    QMutexLocker o_Locker(&m_oQMutex);

    if(m_pTcpSocket != NULL){
        if(m_pTcpSocket->state() == ConnectedState){
            m_pTcpSocket->disconnectFromHost();
            if(m_pTcpSocket->state() != QAbstractSocket::UnconnectedState){
                m_pTcpSocket->waitForDisconnected(2000);
            }
        }

        delete m_pTcpSocket;
        m_pTcpSocket = NULL;
    }

    return false;
}

bool ScanerDevice::GetConnect()
{
    return m_bConnectState;
}

QString ScanerDevice::GetCodeData()
{
    return m_strCodeData;
}

bool ScanerDevice::RequestCode(const int &n_ID)
{
    if(n_ID != m_nDeviceID)
        return false;

    if(m_pTcpSocket == NULL){
        emit sig_CodeData(m_nDeviceID, false, SACNUNRESPONSECODE);
        return false;
    }

    m_nWriteTime ++;

    if(m_nWriteTime == 3){
        m_nWriteTime = 0;
        emit sig_CodeData(m_nDeviceID, false, SACNUNRESPONSECODE);
        return false;
    }

    if(m_pTcpSocket->state() != ConnectedState){
        return RequestCode(n_ID);
    }

    if(m_pTcpSocket->write(REQUESTCODE, sizeof(REQUESTCODE)/sizeof(char)) != -1){
        if(m_pTcpSocket->waitForBytesWritten(2000)){
            m_nWriteTime = 0;
            m_bSacnResponse = false;

            QTimer::singleShot(REQUESTCODE_TIMEOUT, this, [&](){
                if(!m_bSacnResponse)
                    emit sig_CodeData(m_nDeviceID, false, SACNUNRESPONSECODE);
            });

            return true;
        }
        return RequestCode(n_ID);
    }

    emit sig_CodeData(m_nDeviceID, false, SACNUNRESPONSECODE);
    return false;
}

void ScanerDevice::WorkSleep(int n_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(n_Msec);
    while(QTime::currentTime() < o_DieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void ScanerDevice::slot_ConnectDevice(int n_ID)
{
    if(n_ID != m_nDeviceID)
        return;

    ConnectDevice();
}

void ScanerDevice::slot_DisconnectDevice(int n_ID)
{
    if(n_ID != m_nDeviceID)
        return;

    DisConnectDevice();
}

void ScanerDevice::slot_RequestCode(int n_ID)
{
    RequestCode(n_ID);
}

void ScanerDevice::slot_ScanData()
{
    if(m_pTcpSocket->bytesAvailable() != 0 &&
            m_pTcpSocket->bytesAvailable() != -1){
        QByteArray byte_ReadData = m_pTcpSocket->readAll();

        m_oScanDataManage.InputScanData(byte_ReadData);
    }
}

void ScanerDevice::slot_Connect()
{
    m_bConnectState = true;
    emit sig_ConnectState(m_nDeviceID, m_bConnectState);
}

void ScanerDevice::slot_Disconnect()
{
    m_bConnectState = false;
    emit sig_ConnectState(m_nDeviceID, m_bConnectState);
}

void ScanerDevice::slot_CodeData(QByteArray byte_CodeData)
{
    m_strCodeData = QString(byte_CodeData);

    m_bSacnResponse = true;

    if(m_strCodeData == SACNFAILECODE){
        if(m_nScanTime == 3){
            m_nScanTime ++;
            RequestCode(m_nDeviceID);
        }
        else{
            m_nScanTime = 0;
            emit sig_CodeData(m_nDeviceID, false, SACNFAILECODE);
        }
    }
    else{
        m_nScanTime = 0;
        emit sig_CodeData(m_nDeviceID, true, m_strCodeData);
    }

}
