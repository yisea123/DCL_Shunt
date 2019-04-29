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
    if(m_mapScaner.contains(n_ID) || m_mapThread.contains(n_ID))
        return false;

    ScanerDevice *p_ScanerDevice = new ScanerDevice;

    p_ScanerDevice->SetDeviceID(n_ID);
    p_ScanerDevice->SetDeviceIPAndPort(str_IP, n_Port);

    connect(p_ScanerDevice, SIGNAL(sig_ConnectState(int, bool)),
            this, SLOT(slot_ConnectState(int, bool)));

    connect(p_ScanerDevice, SIGNAL(sig_CodeData(int, bool, QString)),
            this, SLOT(slot_CodeData(int, bool, QString)));

    QThread *p_QThread = new QThread;
    p_ScanerDevice->moveToThread(p_QThread);
    p_QThread->start();

    if(!p_ScanerDevice->ConnectDevice()){
        delete p_ScanerDevice;

        p_QThread->quit();
        p_QThread->wait();
        delete p_QThread;

        return false;
    }

    m_mapScaner.insert(n_ID, p_ScanerDevice);
    m_mapThread.insert(n_ID, p_QThread);

    return true;
}

bool ScanerManager::RemoveScanerDevice(const int &n_ID)
{
    if(!m_mapScaner.contains(n_ID) || !m_mapThread.contains(n_ID))
        return false;

    QThread *p_QThread = m_mapThread.value(n_ID);
    p_QThread->quit();
    p_QThread->wait();
    delete p_QThread;
    m_mapThread.remove(n_ID);

    ScanerDevice *p_ScanerDevice = m_mapScaner.value(n_ID);
    delete p_ScanerDevice;
    m_mapScaner.remove(n_ID);

    emit sig_ConnectState(n_ID, false);

    return true;
}

bool ScanerManager::ClearScanerDevice()
{
    if(m_mapScaner.isEmpty() || m_mapThread.isEmpty())
        return false;

    if(m_mapScaner.size() != m_mapThread.size())
        return false;

    QMapIterator<int, QThread *> map_Iterator_Thread(m_mapThread);
    while(map_Iterator_Thread.hasNext()){
          map_Iterator_Thread.next();
          QThread *p_QThread = map_Iterator_Thread.value();

          p_QThread->quit();
          p_QThread->wait();
          delete p_QThread;
    }

    QMapIterator<int, ScanerDevice *> map_Iterator_Scaner(m_mapScaner);
    while(map_Iterator_Scaner.hasNext()){
          map_Iterator_Scaner.next();
          ScanerDevice *p_ScanerDevice = map_Iterator_Scaner.value();

          if(p_ScanerDevice->GetConnect()){
              emit sig_ConnectState(p_ScanerDevice->GetDeviceID(), false);
          }

          WorkSleep(10);

          delete p_ScanerDevice;
    }

    m_mapThread.clear();
    m_mapScaner.clear();

    return true;
}

bool ScanerManager::RequsetCode(const int &n_ID)
{
    if(!m_mapScaner.contains(n_ID) || !m_mapThread.contains(n_ID))
        return false;

    ScanerDevice *p_ScanerDevice = m_mapScaner.value(n_ID);
    return p_ScanerDevice->RequestCode(n_ID);

//    for (int i =0; i < 10000; i++) {
//        QMapIterator<int, ScanerDevice *> map_Iterator_Scaner(m_mapScaner);
//        while(map_Iterator_Scaner.hasNext()){
//              map_Iterator_Scaner.next();
//              ScanerDevice *p_ScanerDevice = map_Iterator_Scaner.value();

//              p_ScanerDevice->RequestCode(map_Iterator_Scaner.key());
//              WorkSleep(20);
//        }
//        i ++;
//    }
//    return true;
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
}
