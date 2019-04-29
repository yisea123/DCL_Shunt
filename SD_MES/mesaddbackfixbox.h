#ifndef MESADDBACKFIXBOX_H
#define MESADDBACKFIXBOX_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include "global_h.h"

class MESAddBackFixBox : public QObject
{
    Q_OBJECT
public:
    explicit MESAddBackFixBox(QUrl url, QObject *parent = 0);

    //发送上传坏品挑选到空框后，料框信息
    void sendBackFixBox(QString scannerName, QString boxId, QJsonArray sfcArr); //sfcArr电芯在新料框中的链表信息
    //设置mes访问地址，界面可以切换
    void setMesUrl(const QUrl url);

private:
    void sendBackFixBox(const QString scannerName, const QString boxId, const QJsonArray sfcArr, int resendCount);
    void analysisData(const QByteArray bytes, QJsonObject &jsonObj, bool &status);
    QJsonObject getJsonObj(QString jsonStr);
    QString getJsonString(const QString scannerName, const QString boxId, const QJsonArray sfcArr);

signals:
    //信号返回为最终结果
    void sigUploadResult(QString scannerName, QString boxId, int result);

public slots:
    //请求消息发送之前需要认证用户账号密码
    void slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

private:
    QUrl mesUrl;
};

#endif // MESADDBACKFIXBOX_H
