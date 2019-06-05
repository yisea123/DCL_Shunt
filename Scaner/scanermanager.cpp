#include "scanermanager.h"
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QThread>

ScanerManager::ScanerManager(QObject *parent)
    : QObject(parent)
{

}

ScanerManager::~ScanerManager()
{
    ClearScanerDevice();
}

bool ScanerManager::AddScanerDevice(const int &n_ID,
                                    const QString &str_IP,
                                    const int &n_Port)
{
    QMutexLocker o_Locker(&m_oQMutex);

    if(m_mapScaner.contains(n_ID) || m_mapThread.contains(n_ID))
        return false;

    ScanerDevice *p_ScanerDevice = new ScanerDevice;

    p_ScanerDevice->SetDeviceID(n_ID);
    p_ScanerDevice->SetDeviceIPAndPort(str_IP, n_Port);

    connect(p_ScanerDevice, SIGNAL(sig_ConnectState(int, bool)),
            this, SLOT(slot_ConnectState(int, bool)));

    connect(p_ScanerDevice, SIGNAL(sig_CodeData(int, bool, QString)),
            this, SLOT(slot_CodeData(int, bool, QString)));

    connect(this, SIGNAL(sig_ConnectDevice(int)),
            p_ScanerDevice, SLOT(slot_ConnectDevice(int)));

    connect(this, SIGNAL(sig_DisconnectDevice(int)),
            p_ScanerDevice, SLOT(slot_DisconnectDevice(int)));

    connect(this, SIGNAL(sig_RequestCode(int)),
            p_ScanerDevice, SLOT(slot_RequestCode(int)));

    QThread *p_QThread = new QThread;
    p_ScanerDevice->moveToThread(p_QThread);
    p_QThread->start();

    emit sig_ConnectDevice(n_ID);

    m_mapScaner.insert(n_ID, p_ScanerDevice);
    m_mapThread.insert(n_ID, p_QThread);

    m_listNeedConnectScaner.append(n_ID);

    return true;
}

bool ScanerManager::RemoveScanerDevice(const int &n_ID)
{
    QMutexLocker o_Locker(&m_oQMutex);

    if(!m_mapScaner.contains(n_ID) || !m_mapThread.contains(n_ID))
        return false;

    emit sig_DisconnectDevice(n_ID);
    m_listNeedConnectScaner.removeAll(n_ID);

    return true;
}

bool ScanerManager::ClearScanerDevice()
{
    QMutexLocker o_Locker(&m_oQMutex);

    if(m_mapScaner.isEmpty() || m_mapThread.isEmpty())
        return false;

    if(m_mapScaner.size() != m_mapThread.size())
        return false;

    QMapIterator<int, ScanerDevice *> map_Iterator_Scaner(m_mapScaner);
    while(map_Iterator_Scaner.hasNext()){
          map_Iterator_Scaner.next();
          ScanerDevice *p_ScanerDevice = map_Iterator_Scaner.value();

          emit sig_DisconnectDevice(p_ScanerDevice->GetDeviceID());
    }

    m_listNeedConnectScaner.clear();

    return true;
}

bool ScanerManager::RequsetCode(const int &n_ID)
{
    QMutexLocker o_Locker(&m_oQMutex);
    if(!m_mapScaner.contains(n_ID) || !m_mapThread.contains(n_ID)){
        sig_CodeData(n_ID, false, "000000000001");
        return false;
    }

    emit sig_RequestCode(n_ID);
    return true;
}

bool ScanerManager::GetConnectScanerList(QList<int> &list_ConnectScaner)
{
    if(m_listConnectScaner.isEmpty())
        return false;

    list_ConnectScaner = m_listConnectScaner;

    return true;
}

void ScanerManager::WorkSleep(int n_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(n_Msec);
    while(QTime::currentTime() < o_DieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void ScanerManager::slot_CodeData(int n_ID,
                                  bool b_ScanState,
                                  QString str_Code)
{
    emit sig_CodeData(n_ID, b_ScanState, str_Code);
}

void ScanerManager::slot_ConnectState(int n_ID,
                                      bool b_ConnectState)
{
    emit sig_ConnectState(n_ID, b_ConnectState);

    if(b_ConnectState){
        m_listConnectScaner.append(n_ID);
    }
    else{
        if(!m_mapScaner.contains(n_ID) || !m_mapThread.contains(n_ID))
            return;

        m_oQMutex.lock();

        QThread *p_QThread = m_mapThread.value(n_ID);

        p_QThread->quit();
        p_QThread->wait();
        p_QThread->deleteLater();

        QString str_IP = "";
        int n_Port = 0;

        ScanerDevice *p_ScanerDevice = m_mapScaner.value(n_ID);

        p_ScanerDevice->GetSetDeviceIPAndPort(str_IP, n_Port);

        p_ScanerDevice->deleteLater();

        m_mapThread.remove(n_ID);
        m_mapScaner.remove(n_ID);
        m_listConnectScaner.removeAll(n_ID);

        m_oQMutex.unlock();

        if(m_listNeedConnectScaner.contains(n_ID)){
            AddScanerDevice(n_ID, str_IP, n_Port);
        }
    }
}
