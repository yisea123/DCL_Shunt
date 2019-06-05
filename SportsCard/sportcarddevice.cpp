#include "sportcarddevice.h"

#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QMetaType>

SportCardDevice::SportCardDevice(QThread *parent)
    : QThread (parent)
{
    qRegisterMetaType<SPC_SWAP_CMD>("SPC_SWAP_CMD");
    qRegisterMetaType<SWAPCMD>("SWAPCMD");

    InitDevice();
}

SportCardDevice::~SportCardDevice()
{
    m_bRunSwitch = false;

    this->quit();
    this->wait();

    DisconnectDevice();
}

void SportCardDevice::SetID(int n_ID)
{
    m_nID = n_ID;
}

int SportCardDevice::GetID()
{
    return m_nID;
}

void SportCardDevice::SetDeviceIP(const QString &str_IP)
{
    m_strIP = str_IP;
}

bool SportCardDevice::ConnectDevice()
{
    QByteArray byte_Data = m_strIP.toLatin1();

    if(ZAux_OpenEth(byte_Data.data(), &m_hDevice) != 0)
        return false;

    emit sig_ConnectState(m_nID, true);

    return true;
}

bool SportCardDevice::DisconnectDevice()
{
   if(m_hDevice == NULL)
       return false;

    if(ZAux_Close(m_hDevice) != 0)
        return false;

    emit sig_ConnectState(m_nID, false);
    m_hDevice = NULL;

    return true;
}

bool SportCardDevice::GetConnectState()
{
    return m_hDevice != NULL ? true : false;
}

bool SportCardDevice::Cmd_GetRemoveFullBox()
{
    uchar ucCmd[2] = {0, 0};

    return WriteData_uc(680, 1, ucCmd);
}

bool SportCardDevice::Cmd_GetRemoveEmptyBox()
{
    uchar ucCmd[2] = {0, 0};

    return WriteData_uc(681, 1, ucCmd);
}

bool SportCardDevice::Cmd_SendFullReady()
{
    uchar ucCmd[2] = {1, 0};

    return WriteData_uc(682, 1, ucCmd);
}

bool SportCardDevice::Cmd_SendEmptyReady()
{
    uchar ucCmd[2] = {1, 0};

    return WriteData_uc(683, 1, ucCmd);
}

bool SportCardDevice::WriteProductInfo(QList<PRODUCTINFO_> &list_ProductInfo)
{
    if(list_ProductInfo.isEmpty())
        return false;

    uchar ucIDData[2401] = {};
    uchar ucQualityData[49] = {};

    memset(ucIDData, 0, sizeof(ucIDData)/sizeof(uchar));
    memset(ucQualityData, 0, sizeof(ucQualityData)/sizeof(uchar));

    for(int i = 0; i < list_ProductInfo.size(); i++){
        if(list_ProductInfo.at(i).n_SequencnNumber != i)
            return false;

        QByteArray byte_Data = list_ProductInfo.at(i).str_ID.toLatin1();

        if(byte_Data.size() > 50)
            return false;

        memcpy(ucIDData + i*50, byte_Data.data(), byte_Data.size());

        byte_Data = list_ProductInfo.at(i).str_Quality.toLatin1();

        memcpy(ucQualityData + i, byte_Data.data(), byte_Data.size());

    }

    float fIDData[601] = {};

    memset(fIDData, 0, sizeof(fIDData)/sizeof(float));

    memcpy(fIDData, ucIDData, sizeof(ucIDData)/sizeof(uchar));

    for(int i = 0; i < 600/50; i++){
        if(!WriteData_f(1000 + i*50*4, 50, fIDData + i*50))
            return false;
    }

    for(int i = 0; i < 48/8; i++){
        if(!WriteData_uc(501 + i*8, 8, ucQualityData + i*8))
            return false;
    }

    return true;
}

bool SportCardDevice::GetProductInfo(QList<PRODUCTINFO_> &list_ProductInfo)
{
    uchar ucIDData[2401] = {};
    uchar ucQualityData[49] = {};

    memset(ucIDData, 0, sizeof(ucIDData)/sizeof(uchar));
    memset(ucQualityData, 0, sizeof(ucQualityData)/sizeof(uchar));

    float fIDData[601] = {};
    memset(fIDData, 0, sizeof(fIDData)/sizeof(float));

    for(int i = 0; i < 600/50; i++){
        if(!GetData_f(4000 + i*50*4, 50, fIDData + i*50))
            return false;
    }

    for(int i = 0; i < 48/8; i++){
        if(!GetData_uc(601 + i*8, 8, ucQualityData + i*8))
            return false;
    }

    memcpy(ucIDData, fIDData, sizeof(ucIDData)/sizeof(uchar));

    PRODUCTINFO_ ProductInfo;
    QString str_ID = "";
    char cData1[51] = {};
    char cData2[2] = {};

    memset(cData1, 0, sizeof(cData1)/sizeof(char));
    memset(cData2, 0, sizeof(cData2)/sizeof(char));

    for(int i = 0; i < 48; i++){
        memcpy(cData1, ucIDData + i*50, 50);

        str_ID = QString("%1").arg(cData1);

        if(str_ID == "")
            break;

        ProductInfo.str_ID = str_ID;

        memcpy(cData2, ucQualityData + i, 1);

        ProductInfo.str_Quality = QString("%1").arg(cData2);

        ProductInfo.n_SequencnNumber = i;

        list_ProductInfo.append(ProductInfo);
    }

    return true;
}

void SportCardDevice::UploadCmdSwitch(const bool &b_Switch)
{
    m_bUploadCmdSwitch = b_Switch;
}

bool SportCardDevice::GetData_uc(const ushort &us_Start,
                                 const ushort &us_Num,
                                 uchar *p_ucData)
{
    if(m_hDevice == NULL)
        return false;

    if(us_Num > 8)
        return false;

    if(ZAux_Modbus_Get0x(m_hDevice, us_Start, us_Num, p_ucData) != 0)
        return false;

    return true;
}

bool SportCardDevice::WriteData_uc(const ushort &us_Start,
                                   const ushort &us_Num,
                                   uchar *p_ucData)
{
    if(m_hDevice == NULL)
        return false;

    if(us_Num > 8)
        return false;

    if(ZAux_Modbus_Set0x(m_hDevice, us_Start, us_Num, p_ucData) != 0)
        return false;

    return true;
}

bool SportCardDevice::GetData_f(const ushort &us_Start,
                                const ushort &us_Num,
                                float *p_fData)
{
    if(m_hDevice == NULL)
        return false;

    if(us_Num > 50)
        return false;

    if(ZAux_Modbus_Get4x_Float(m_hDevice, us_Start, us_Num, p_fData) != 0)
        return false;

    return true;
}

bool SportCardDevice::WriteData_f(const ushort &us_Start,
                                  const ushort &us_Num,
                                  float *p_fData)
{
    if(m_hDevice == NULL)
        return false;

    if(us_Num > 50)
        return false;

    if(ZAux_Modbus_Set4x_Float(m_hDevice, us_Start, us_Num, p_fData) != 0)
        return false;

    return true;
}

void SportCardDevice::GetSportCardCmd()
{
    if(!m_bUploadCmdSwitch)
        return;

    if(m_CurrentSwapCmd.uc_RemoveFullBox_CompleteTake != m_LastSwapCmd.uc_RemoveFullBox_CompleteTake &&
            m_CurrentSwapCmd.uc_RemoveFullBox_CompleteTake == 1)
        emit sig_Cmd(m_nID, REMOVEFULLBOX_COMPLETETAKE);

    if(m_CurrentSwapCmd.uc_RemoveEmptyBox_CompletePut != m_LastSwapCmd.uc_RemoveEmptyBox_CompletePut &&
            m_CurrentSwapCmd.uc_RemoveEmptyBox_CompletePut == 1)
        emit sig_Cmd(m_nID, REMOVEEMPTYBOX_COMPLETEPUT);
}

void SportCardDevice::InitDevice()
{
    m_hDevice = NULL;
    m_bRunSwitch = true;
    m_bConnectState = false;
    m_bUploadCmdSwitch = false;

    m_strIP = "";
    m_nID = 0;

    this->start();
}

void SportCardDevice::WorkSleep(int n_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(n_Msec);
    while(QTime::currentTime() < o_DieTime && m_bRunSwitch){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void SportCardDevice::run()
{
    while(m_bRunSwitch){
        WorkSleep(500);

        if(m_hDevice == NULL)
            continue;

        uchar ucData[4] = {};
        memset(ucData, 0, sizeof(ucData)/sizeof(uchar));

        if(!GetData_uc(680, sizeof(ucData)/sizeof(uchar), ucData))
            continue;

        memcpy(&m_CurrentSwapCmd, ucData, sizeof(ucData)/sizeof(uchar));

        emit sig_CmdData(m_nID, m_CurrentSwapCmd);

        GetSportCardCmd();
    }
}
