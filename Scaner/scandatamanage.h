#ifndef SCANDATAMANAGE_H
#define SCANDATAMANAGE_H

#include <QByteArray>
#include <QString>
#include <QObject>

const int PACKETHANDERLENGHT        = 5;
const int CODELENGHT                = 12;
const int PACKETTAILLENGHT          = 2;
const int COMPLETEDATALENGHT        = 19;

const char PACKETHANDER_CHAR[5]     = {0x72, 0x61, 0x70, 0x6f, 0x6f};
const char PACKETTAIL_CHAR[2]       = {0x0D, 0x0A};

const QString SACNFAILECODE         = "000000000000";
const QString SACNUNRESPONSECODE    = "000000000001";
const QByteArray SACNFAILEBYTE      = "000000000000";

class ScanDataManage : public QObject
{
    Q_OBJECT
public:
    explicit ScanDataManage(QObject *parent = 0);
    ~ScanDataManage();

    void InputScanData(QByteArray &byte_ReadByte);

private:
    void HandleCodeData(QByteArray &byte_ReadByte);

    void HandleCacheData();

signals:
    void sig_CodeData(QByteArray byte_CodeData);

private:
    //»º´æÊý¾Ý
    QByteArray m_byteCacheData;
};

#endif // SCANDATAMANAGE_H
