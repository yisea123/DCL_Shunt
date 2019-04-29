#ifndef MESREQBADINFO_H
#define MESREQBADINFO_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include <QJsonArray>
#include "global_h.h"

class MESReqBadInfo : public QObject
{
    Q_OBJECT
public:
    explicit MESReqBadInfo(QUrl url, QObject *parent = 0);

    //发送请求坏品挑选信息
    void sendReqBadInfo(QString scannerName, QString boxId);
    //设置mes访问地址，界面可以切换
    void setMesUrl(const QUrl url);

private:
    void sendReqBadInfo(const QString scannerName, const QString boxId, int resendCount);
    void analysisData(const QByteArray bytes, QJsonObject &jsonObj, bool &status);
    QJsonObject getJsonObj(QString jsonStr);

signals:
    //信号返回为最终结果
    void sigReqBadInfo(QString scannerName, QString boxId, QJsonArray sfcArr, int result);

public slots:
    //请求消息发送之前需要认证用户账号密码
    void slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

private:
    QUrl mesUrl;
};

#endif // MESREQBADINFO_H
