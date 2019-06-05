#include "scandatamanage.h"

ScanDataManage::ScanDataManage(QObject *parent)
    : QObject(parent)
{

}

ScanDataManage::~ScanDataManage()
{

}

void ScanDataManage::InputScanData(QByteArray &byte_ReadByte)
{
    while(!byte_ReadByte.isEmpty()){
        //包头查找
        int n_HeaderIndex = byte_ReadByte.indexOf(PACKETHANDER_CHAR);
        //包头在开头
        if(n_HeaderIndex == 0){
            if(byte_ReadByte.size() < COMPLETEDATALENGHT){
                //缓存
                m_byteCacheData.append(byte_ReadByte);
                byte_ReadByte.clear();
            }
            else{
                //包处理
                QByteArray byte_Data = byte_ReadByte.left(COMPLETEDATALENGHT);
                HandleCodeData(byte_Data);
                byte_ReadByte.remove(0, COMPLETEDATALENGHT);
            }
        }
        //没有包头
        else if(n_HeaderIndex == -1){
            //缓存
            m_byteCacheData.append(byte_ReadByte);
            byte_ReadByte.clear();
            //缓存包处理
            HandleCacheData();
        }
        //包头在中间
        else{
            //缓存
            m_byteCacheData.append(byte_ReadByte.left(n_HeaderIndex));
            byte_ReadByte.remove(0, n_HeaderIndex);

            HandleCacheData();
        }
    }
}

void ScanDataManage::HandleCodeData(QByteArray &byte_ReadByte)
{
    QByteArray byte_Data = byte_ReadByte.mid(PACKETHANDERLENGHT + CODELENGHT, PACKETTAILLENGHT);

    if(byte_Data.contains(PACKETTAIL_CHAR)){
        byte_Data = byte_ReadByte.mid(PACKETHANDERLENGHT, CODELENGHT);
        emit sig_CodeData(byte_Data);
    }
    else{
        emit sig_CodeData(SACNFAILEBYTE);
    }
}

void ScanDataManage::HandleCacheData()
{
    //包头查找
    int n_HeaderIndex = m_byteCacheData.indexOf(PACKETHANDER_CHAR);

    //数据太短不处理
    if(m_byteCacheData.size() < COMPLETEDATALENGHT){
        return;
    }

    //包头检查开头，清除数据
    if(n_HeaderIndex == -1){
        m_byteCacheData.clear();
        return;
    }
    else if(n_HeaderIndex == 0){
        //往下处理
    }
    else{
        m_byteCacheData.remove(0, n_HeaderIndex);
    }

    QByteArray byte_Data = m_byteCacheData.mid(PACKETHANDERLENGHT + CODELENGHT, PACKETTAILLENGHT);

    if(byte_Data.contains(PACKETTAIL_CHAR)){
        byte_Data = m_byteCacheData.mid(PACKETHANDERLENGHT, CODELENGHT);
        emit sig_CodeData(byte_Data);
    }
    else{
        emit sig_CodeData(SACNFAILEBYTE);
    }

    m_byteCacheData.remove(0, COMPLETEDATALENGHT);
}
