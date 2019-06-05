#include "sportcardmanager.h"
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>

SportCardManager::SportCardManager(QObject *parent)
    :QObject (parent)
{

}

SportCardManager::~SportCardManager()
{
    ClearSportCard();
}

bool SportCardManager::AddSportCard(int n_ID, QString str_IP)
{
    if(m_mapSportCard.contains(n_ID))
        return false;

    SportCardDevice *p_SportCardDevice = new SportCardDevice;

    p_SportCardDevice->SetID(n_ID);
    p_SportCardDevice->SetDeviceIP(str_IP);

    connect(p_SportCardDevice, &SportCardDevice::sig_ConnectState,
            this, &SportCardManager::slot_ConnectState);
    connect(p_SportCardDevice, &SportCardDevice::sig_CmdData,
            this, &SportCardManager::slot_CmdData);
    connect(p_SportCardDevice, &SportCardDevice::sig_Cmd,
            this, &SportCardManager::slot_Cmd);

    if(!p_SportCardDevice->ConnectDevice()){
        delete p_SportCardDevice;
        return false;
    }

    m_mapSportCard.insert(n_ID, p_SportCardDevice);
    m_listConnectSportCard.append(n_ID);

    return true;
}

bool SportCardManager::RemoveSportCard(int n_ID)
{
    if(!m_mapSportCard.contains(n_ID))
        return false;

    SportCardDevice *p_SportCardDevice = m_mapSportCard.value(n_ID);

    delete p_SportCardDevice;

    emit sig_ConnectState(n_ID, false);

    return true;
}

bool SportCardManager::ClearSportCard()
{
    if(m_mapSportCard.isEmpty())
        return false;

    QMapIterator<int, SportCardDevice *> map_Iterator_SportCard(m_mapSportCard);
    while(map_Iterator_SportCard.hasNext()){
        map_Iterator_SportCard.next();

        SportCardDevice *p_SportCardDevice = map_Iterator_SportCard.value();

        emit sig_ConnectState(p_SportCardDevice->GetID(), false);

        delete p_SportCardDevice;
    }

    m_mapSportCard.clear();
    m_listConnectSportCard.clear();

    return true;
}

bool SportCardManager::Cmd_GetRemoveFullBox(int n_ID)
{
    if(!m_mapSportCard.contains(n_ID))
        return false;

    return m_mapSportCard.value(n_ID)->Cmd_GetRemoveFullBox();
}

bool SportCardManager::Cmd_GetRemoveEmptyBox(int n_ID)
{
    if(!m_mapSportCard.contains(n_ID))
        return false;

    return m_mapSportCard.value(n_ID)->Cmd_GetRemoveEmptyBox();
}

bool SportCardManager::Cmd_SendFullReady(int n_ID)
{
    if(!m_mapSportCard.contains(n_ID))
        return false;

    return m_mapSportCard.value(n_ID)->Cmd_SendFullReady();
}

bool SportCardManager::Cmd_SendEmptyReady(int n_ID)
{
    if(!m_mapSportCard.contains(n_ID))
        return false;

    return m_mapSportCard.value(n_ID)->Cmd_SendEmptyReady();
}

bool SportCardManager::WriteProductInfo(int n_ID, QList<PRODUCTINFO_> &list_ProductInfo)
{
    if(!m_mapSportCard.contains(n_ID))
        return false;

    return m_mapSportCard.value(n_ID)->WriteProductInfo(list_ProductInfo);
}

bool SportCardManager::GetProductInfo(int n_ID, QList<PRODUCTINFO_> &list_ProductInfo)
{
    if(!m_mapSportCard.contains(n_ID))
        return false;

    return m_mapSportCard.value(n_ID)->GetProductInfo(list_ProductInfo);
}

void SportCardManager::UploadCmdSwitch(const bool &b_Switch)
{
    if(m_mapSportCard.isEmpty())
        return;

    QMapIterator<int, SportCardDevice *> map_Iterator_SportCard(m_mapSportCard);
    while(map_Iterator_SportCard.hasNext()){
        map_Iterator_SportCard.next();

        SportCardDevice *p_SportCardDevice = map_Iterator_SportCard.value();

        p_SportCardDevice->UploadCmdSwitch(b_Switch);
    }
}

void SportCardManager::WorkSleep(int n_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(n_Msec);
    while(QTime::currentTime() < o_DieTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void SportCardManager::slot_ConnectState(int n_ID, bool b_State)
{
    emit sig_ConnectState(n_ID, b_State);
}

void SportCardManager::slot_CmdData(int n_ID, SPC_SWAP_CMD Cmd)
{
    emit sig_CmdData(n_ID, Cmd);
}

void SportCardManager::slot_Cmd(int n_ID, SWAPCMD Cmd)
{
    emit sig_SportCardCmd(n_ID, Cmd);
}
