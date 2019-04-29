#ifndef MESNETTEST_H
#define MESNETTEST_H

#include <QObject>
#include <QXmlStreamReader>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include "global_h.h"

class MESNetTest : public QObject
{
    Q_OBJECT
public:
    explicit MESNetTest(QUrl url, QObject *parent = 0);

    //发送测试网络接口
    void sendTestMes();
    void setMesUrl(const QUrl url);

private:
    void sendTestMes(int resendCount);

signals:
    void sigTestResult(int type);

public slots:
//    //请求消息发送之前需要认证用户账号密码
    void slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

private:
    QUrl mesUrl;
};

#endif // MESNETTEST_H
