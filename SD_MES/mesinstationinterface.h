#ifndef MESINSTATININTERFACE_H
#define MESINSTATININTERFACE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include "global_h.h"

class MESInStationInterface : public QObject
{
    Q_OBJECT
public:
    explicit MESInStationInterface(QUrl url, QObject *parent = 0);

    void sendInStation(const QString scannerName, const QString boxId);
    void setMesUrl(const QUrl url);

private:
    void sendInStation(const QString scannerName, const QString boxId, int resendCount);
    QString getDataJsonString(const QString scannerName, const QString boxId);
    void analysisData(const QByteArray bytes, bool &status, QJsonObject &jsonObj);
    QJsonObject getJsonObj(QString jsonStr);

signals:
    //resultStr： FFR==反分容  IN==进高温库  result标识MES返回状态
    void sigInStationRespond(const QString scannerName, const QString boxId, const QString resultStr, int result);

public slots:
//    //请求消息发送之前需要认证用户账号密码
    void slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

private:
    QUrl mesUrl;
};

#endif // MESINSTATININTERFACE_H
