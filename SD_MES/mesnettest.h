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

    void sendTestMes();
    void setMesUrl(const QUrl url);

private:
    void sendTestMes(int resendCount);

signals:
    void sigTestResult(int type);

public slots:
    void slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator);

private:
    QUrl mesUrl;
};

#endif // MESNETTEST_H
