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

    m_pTcpSocket = new QTcpSocket;
    connect(m_pTcpSocket, SIGNAL(connected()),
            this, SLOT(slot_Connect()));
    connect(m_pTcpSocket, SIGNAL(disconnected()),
            this, SLOT(slot_Disconnect()));
    connect(m_pTcpSocket, SIGNAL(readyRead()),
            this, SLOT(slot_ScanData()));

    connect(&m_oScanDataManage, SIGNAL(sig_CodeData(QByteArray)),
            this, SLOT(slot_CodeData(QByteArray)));
}

ScanerDevice::~ScanerDevice()
{
    if(m_pTcpSocket != NULL){
        DisConnectDevice();
        m_pTcpSocket->close();
        delete m_pTcpSocket;
        m_pTcpSocket = NULL;
    }
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

bool ScanerDevice::SetDeviceIPAndPort(QString str_IP, int n_Port)
{
    m_strIP = str_IP;
    m_nPort = n_Port;

    return true;
}

bool ScanerDevice::ConnectDevice()
{
    if(m_bConnectState || m_pTcpSocket->state() != UnconnectedState){
        m_pTcpSocket->disconnectFromHost();
        m_pTcpSocket->waitForDisconnected(1000);
    }

    m_pTcpSocket->connectToHost(m_strIP, m_nPort);

    return m_pTcpSocket->waitForConnected(1000);
}

bool ScanerDevice::DisConnectDevice()
{
    if(m_pTcpSocket->state() == ConnectedState){
        m_pTcpSocket->disconnectFromHost();
        return m_pTcpSocket->waitForDisconnected(1000);
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

    m_nWriteTime ++;

    if(m_nWriteTime == 3){
        m_nWriteTime = 0;
        emit sig_CodeData(m_nDeviceID, false, SACNFAILECODE);
        return false;
    }

    if(m_pTcpSocket->state() != ConnectedState){
        ConnectDevice();
        return RequestCode(n_ID);
    }

    if(m_pTcpSocket->write(REQUESTCODE, sizeof(REQUESTCODE)/sizeof(char)) != -1){
        if(m_pTcpSocket->waitForBytesWritten(500)){
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
