#ifndef DATAFILE_H
#define DATAFILE_H

#include <QString>
#include <QList>

class DataFile
{
public:
    bool GetScanerIPList(QList<QString> &list_IP);

    bool GetPLCIP(QString &str_IP);

    bool GetSportCardIPList(QList<QString> &list_IP);

    bool SaveScanerIPList(const QList<QString> &list_IP);

    bool SavePLCIP(const QString &str_IP);

    bool SaveSportCardIPList(const QList<QString> &list_IP);

};

#endif // DATAFILE_H
