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
        //��ͷ����
        int n_HeaderIndex = byte_ReadByte.indexOf(PACKETHANDER_CHAR);
        //��ͷ�ڿ�ͷ
        if(n_HeaderIndex == 0){
            if(byte_ReadByte.size() < COMPLETEDATALENGHT){
                //����
                m_byteCacheData.append(byte_ReadByte);
                byte_ReadByte.clear();
            }
            else{
                //������
                QByteArray byte_Data = byte_ReadByte.left(COMPLETEDATALENGHT);
                HandleCodeData(byte_Data);
                byte_ReadByte.remove(0, COMPLETEDATALENGHT);
            }
        }
        //û�а�ͷ
        else if(n_HeaderIndex == -1){
            //����
            m_byteCacheData.append(byte_ReadByte);
            byte_ReadByte.clear();
            //���������
            HandleCacheData();
        }
        //��ͷ���м�
        else{
            //����
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
    //��ͷ����
    int n_HeaderIndex = m_byteCacheData.indexOf(PACKETHANDER_CHAR);

    //����̫�̲�����
    if(m_byteCacheData.size() < COMPLETEDATALENGHT)
    {
        return;
    }

    //��ͷ��鿪ͷ���������
    if(n_HeaderIndex == -1)
    {
        m_byteCacheData.clear();
        return;
    }
    else if(n_HeaderIndex == 0){
        //���´���
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
