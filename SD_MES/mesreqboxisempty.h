#ifndef MESREQBOXISEMPTY_H
#define MESREQBOXISEMPTY_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include "global_h.h"

class MESReqBoxIsEmpty : public QObject
{
    Q_OBJECT
public:
    explicit MESReqBoxIsEmpty(QUrl url, QObject *parent = 0);

    //发送请求是否为空框请求
    void sendReqBoxIsEmpty(const QString devName, const QString boxId);
    //设置mes访问地址，界面可以切换
    void setMesUrl(const QUrl url);

private:
    void sendReqBoxIsEmpty(const QString devName, const QString boxId, int resendCount);
    void analysisData(const QByteArray data, QJsonObject &jsonObj, bool &status);
    QJsonObject getJsonObj(QString jsonStr);

signals:
    //信号返回为最终结果  boxType  1==空框  2==满框  3==料框不存在系统中
    void sigIsEmptyRespond(QString devName, QString boxId, int boxType, int result);

public slots:
    //    //请求消息发送之前需要认证用户账号密码
    void slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

private:
    QUrl mesUrl;
};

#endif // MESREQBOXISEMPTY_H
