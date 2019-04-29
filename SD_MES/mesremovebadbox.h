#ifndef MESREMOVEBADBOX_H
#define MESREMOVEBADBOX_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>
#include "global_h.h"

class MESRemoveBadBox : public QObject
{
    Q_OBJECT
public:
    explicit MESRemoveBadBox(QUrl url, QObject *parent = 0);

    //上传坏品挑选后料框信息
    void sendRemoveBadInfo(QString scannerName, QString boxId);
    //设置mes访问地址，界面可以切换
    void setMesUrl(const QUrl url);

private:
    void sendRemoveBadInfo(const QString scannerName, const QString boxId, int resendCount);
    void analysisData(const QByteArray bytes, QJsonObject &jsonObj, bool &status);
    QJsonObject getJsonObj(QString jsonStr);

signals:
    //信号返回为最终结果
    void sigUploadResult(QString scannerName, QString boxId, int result);

public slots:
    //请求消息发送之前需要认证用户账号密码
    void slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

private:
    QUrl mesUrl;
};

#endif // MESREMOVEBADBOX_H
