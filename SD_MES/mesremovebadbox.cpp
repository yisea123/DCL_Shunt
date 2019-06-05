#include "mesremovebadbox.h"
#include <QXmlStreamReader>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonArray>
#include <QTimer>
#include <QStringRef>

MESRemoveBadBox::MESRemoveBadBox(QUrl url, QObject *parent) : QObject(parent)
{
    mesUrl = url;
}

void MESRemoveBadBox::sendRemoveBadInfo(QString scannerName, QString boxId)
{
    int resendCount = 0;
    sendRemoveBadInfo(scannerName, boxId, resendCount);
}

void MESRemoveBadBox::setMesUrl(const QUrl url)
{
    mesUrl = url;
}

void MESRemoveBadBox::sendRemoveBadInfo(const QString scannerName, const QString boxId, int resendCount)
{
    QJsonObject jsonObj;
    jsonObj.insert("RESOURCE", scannerName);
    jsonObj.insert("ACTION", ACTIONR);
    jsonObj.insert("CONTAINER_ID", boxId);
    QJsonDocument doc(jsonObj);
    QString data(doc.toJson(QJsonDocument::Compact));
    QNetworkRequest req;
    req.setUrl(mesUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader,("text/xml;charst=utf-8"));
    QString soapXML = QString("<SOAP-ENV:Envelope xmlns:SOAP-ENV='http://schemas.xmlsoap.org/soap/envelope/' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>"
            "<SOAP-ENV:Header/>"
            "<SOAP-ENV:Body>"
                "<yq1:execute xmlns:yq1='http://base.ws.sapdev.com/'>"
                  "<pRequest>"
                     "<site>1003</site>"
                      "<data>%1</data>"
                     "<serviceCode>GetSfcsInfoForHptxService</serviceCode>"
                  "</pRequest>"
               "</yq1:execute>"
            "</SOAP-ENV:Body>"
         "</SOAP-ENV:Envelope>").arg(data);
    auto manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::authenticationRequired,
            this, &MESRemoveBadBox::slotAuthenticationRequired);
    QNetworkReply *reply = manager->post(req,soapXML.toUtf8());
//    qDebug()<<"pos msg"<<soapXML.toUtf8();
    reply->setParent(this);

    QAbstractSocket::connect(reply,&QNetworkReply::finished,[=](){
        reply->abort();
        if(reply->error() != QNetworkReply::NoError)
        {
//            qDebug()<<"error msg:"<<reply->errorString();
            int errCount = resendCount + 1;
            if(errCount < NetworkErrorResendTimes)
            {
                QTimer::singleShot(1000, [=](){
                    sendRemoveBadInfo(scannerName, boxId, errCount);});
                return;
            }
            emit sigUploadResult(scannerName, boxId, NetWorkError);
        }
        else
        {
            QJsonObject resObj;
            bool status = false;;
            analysisData(reply->readAll(), resObj, status);
            if(!status)
            {
                int errCount = resendCount + 1;
                if(errCount < NetworkErrorResendTimes)
                {
                    QTimer::singleShot(1000, [=](){
                        sendRemoveBadInfo(scannerName, boxId, errCount);});
                    return;
                }
                emit sigUploadResult(scannerName, boxId, MesStatusError);
            }
            else
            {
                emit sigUploadResult(scannerName, boxId, MESRespondSuccess);
            }
        }
    }
    );
}

void MESRemoveBadBox::analysisData(const QByteArray bytes, QJsonObject &jsonObj, bool &status)
{
//    qDebug()<<"rec msg:"<<bytes;
    QXmlStreamReader reader(bytes);
    while (!reader.atEnd())
    {
        if(reader.isStartElement())
        {
            if(reader.name().toString().compare("status", Qt::CaseInsensitive) == 0)
            {
//                qDebug()<<reader.name();
                reader.readNext();
                if(reader.atEnd())
                {
                    break;
                }
                if(reader.isCharacters())
                {
                    QString statusStr = reader.text().toString();
                    if(statusStr.compare("true", Qt::CaseInsensitive) == 0)
                    {
                        status = true;
                    }
                    else
                    {
                        status = false;
                    }
//                    qDebug()<<"status = "<<status;
                }
            }
            if(reader.name().toString().compare("returnList", Qt::CaseInsensitive) == 0)
            {
                reader.readNext();
                if(reader.atEnd())
                {
                    break;
                }
                if(reader.isCharacters())
                {
                    QString jsonStr = reader.text().toString();
                    jsonObj = getJsonObj(jsonStr);

                }
            }
        }
        reader.readNext();
    }
}

QJsonObject MESRemoveBadBox::getJsonObj(QString jsonStr)
{
    QJsonParseError jsonRrr;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonStr.toUtf8(), &jsonRrr));
    if(jsonRrr.error != QJsonParseError::NoError)
    {
//        qDebug() << "mes json error";
        return QJsonObject();
    }
    if(jsonDoc.isObject())
    {
        return jsonDoc.object();
    }
    else
    {
        return QJsonObject();
    }
}

void MESRemoveBadBox::slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
//    qDebug()<<"log in mes";
    Q_UNUSED(reply);
    authenticator->setUser("sapint");
    authenticator->setPassword("sap12345");
}
