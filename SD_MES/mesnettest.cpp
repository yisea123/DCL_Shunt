#include "mesnettest.h"
#include <QTimer>

MESNetTest::MESNetTest(QUrl url, QObject *parent) : QObject(parent)
{
    mesUrl = url;
}

void MESNetTest::sendTestMes()
{
    int errCount = 0;
    sendTestMes(errCount);
}

void MESNetTest::setMesUrl(const QUrl url)
{
    mesUrl = url;
}

void MESNetTest::sendTestMes(int resendCount)
{
    Q_UNUSED(resendCount);

    QNetworkRequest req;
    req.setUrl(mesUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader,("text/xml;charst=utf-8"));
    QString soapXML = "<soapenv:Envelope xmlns:soapenv='http://schemas.xmlsoap.org/soap/envelope/' xmlns:acc='http://accesstest.uppermachine.ws.sapdev.com/'>"
                      "<soapenv:Header/>"
                      "<soapenv:Body>"
                      "<acc:getResourceDescription>"
                      "<pRequest>"
                      "<site>1003</site>"
                      "<resource>3BJP0001</resource>"
                      "</pRequest>"
                      "</acc:getResourceDescription>"
                      "</soapenv:Body>"
                      "</soapenv:Envelope>";
//    qDebug()<<"send msg:"<<soapXML;
    auto manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::authenticationRequired,
            this, &MESNetTest::slotAuthenticationRequired);
    QNetworkReply *reply = manager->post(req,soapXML.toUtf8());
    reply->setParent(this);

    QAbstractSocket::connect(reply,&QNetworkReply::finished,this, [=](){
        reply->abort();
        if(reply->error() != QNetworkReply::NoError)
        {
            qDebug()<<"error msg:"<<reply->errorString();
            emit sigTestResult(NetWorkError);
        }
        else
        {
            QByteArray bytes = reply->readAll();
            qDebug()<<bytes<<reply->errorString();
            QXmlStreamReader reader(bytes);
            bool status = false;
            while (!reader.atEnd())
            {
                if(reader.isStartElement())
                {
                    if(reader.name() == "status")
                    {
                        reader.readNext();
                        if(reader.atEnd())
                        {
                            break;
                        }
                        if(reader.isCharacters())
                        {
                            QString resultCode = reader.text().toString();
                            if(resultCode.compare("true", Qt::CaseInsensitive) == 0)
                            {
                                status = true;
                            }
                        }
                    }
                }
                reader.readNext();
            }
            if(status)
            {
                emit sigTestResult(MESRespondSuccess);
            }
            else
            {
                emit sigTestResult(MesStatusError);
            }
        }

//        reply->deleteLater();
    });
}

void MESNetTest::slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
//    qDebug()<<"log in mes";
    Q_UNUSED(reply);
    authenticator->setUser("sapint");
    authenticator->setPassword("sap12345");
}
