#include "siemensplcdevice.h"
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>

SiemensPLCDevice::SiemensPLCDevice(QThread *parent)
    : QThread(parent)
{

}

SiemensPLCDevice::~SiemensPLCDevice()
{

}

void SiemensPLCDevice::InitSiemensPLC()
{
    m_pTS7Client = NULL;
    m_pTS7Client = new TS7Client;
}

void SiemensPLCDevice::SetDeviceIP(QString str_IP)
{
    m_strIP = str_IP;
}

void SiemensPLCDevice::SetWriteParameter(int n_DBNumber, int n_Start)
{
    m_nWriteDBNumber = n_DBNumber;
    m_nWriteStart = n_Start;
}

void SiemensPLCDevice::SetReadParameter(int n_DBNumber, int n_Start)
{
    m_nReadDBNumber = n_DBNumber;
    m_nReadStart = n_Start;
}

bool SiemensPLCDevice::ConnectDevice()
{
    if(m_pTS7Client->PlcStatus() != par_stopped)
        return false;

    if(m_pTS7Client->ConnectTo(m_strIP.toLatin1(), 0 ,1) != 0)
        return false;

    return true;
}

bool SiemensPLCDevice::DisconnectDevice()
{
    if(m_pTS7Client->Disconnect() != 0)
        return false;

    return true;
}

bool SiemensPLCDevice::GetConnectState()
{
    return m_pTS7Client->Connected();
}

bool SiemensPLCDevice::GetReadData(PLC_DATAAREA &PLCData)
{
    if(m_pTS7Client->PlcStatus() == par_stopped)
        return false;

    if(m_pTS7Client->DBRead(m_nReadDBNumber, m_nReadStart, 24, &PLCData) != 0)
        return false;

    return true;
}

bool SiemensPLCDevice::GetWriteData(PC_DATAAREA &PCData)
{
    if(m_pTS7Client->PlcStatus() == par_stopped)
        return false;

    if(m_pTS7Client->DBRead(m_nWriteDBNumber, m_nWriteStart, 24, &PCData) != 0)
        return false;

    return true;
}

bool SiemensPLCDevice::WriteData(PC_DATAAREA &PCData)
{
    if(m_pTS7Client->PlcStatus() == par_stopped)
        return false;

    if(m_pTS7Client->DBWrite(m_nWriteDBNumber, m_nWriteStart, 24, &PCData) != 0)
        return false;

    return true;
}

void SiemensPLCDevice::WorkSleep(int n_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(n_Msec);
    while(QTime::currentTime() < o_DieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void SiemensPLCDevice::run()
{

}
