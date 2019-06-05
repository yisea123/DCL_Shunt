#include "mesinstationinterface.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QJsonArray>
#include <QXmlStreamReader>
#include <QTimer>

MESInStationInterface::MESInStationInterface(QUrl url, QObject *parent) : QObject(parent)
{
    mesUrl = url;
}

void MESInStationInterface::sendInStation(const QString scannerName, const QString boxId)
{
    int resendCount = 0;
    sendInStation(scannerName, boxId, resendCount);
}

void MESInStationInterface::setMesUrl(const QUrl url)
{
    mesUrl = url;
}

void MESInStationInterface::sendInStation(const QString scannerName, const QString boxId, int resendCount)
{
    QString jsonStr = getDataJsonString(scannerName, boxId);
    QNetworkRequest req;
    req.setUrl(mesUrl);
    req.setHeader(QNetworkRequest::ContentTypeHeader,("text/xml;charst=utf-8"));
    QString soapXML = QString("<SOAP-ENV:Envelope xmlns:SOAP-ENV='http://schemas.xmlsoap.org/soap/envelope/' xmlns:xs='http://www.w3.org/2001/XMLSchema' xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance'>"
                              "<SOAP-ENV:Body>"
                              "<yq1:execute xmlns:yq1='http://base.ws.sapdev.com/'>"
                              "<pRequest>"
                              "<site>1003</site>"
                              "<data>%1</data>"
                              "<serviceCode>GetAddressAndContainerIdOfBakeService</serviceCode>"
                              "</pRequest>"
                              "</yq1:execute>"
                              "</SOAP-ENV:Body>"
                              "</SOAP-ENV:Envelope>").arg(jsonStr);
//    qDebug()<<"send msg:"<<soapXML;
    auto manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::authenticationRequired,
            this, &MESInStationInterface::slotAuthenticationRequired);
    QNetworkReply *reply = manager->post(req,soapXML.toUtf8());
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
                sendInStation(scannerName, boxId, errCount);});
                return;
            }
            emit sigInStationRespond(scannerName, boxId, "", NetWorkError);
        }
        else
        {
            QJsonObject jsonObj;
            bool status;
            analysisData(reply->readAll(), status, jsonObj);
            if(!status)
            {
                int errCount = resendCount + 1;
                if(errCount < NetworkErrorResendTimes)
                {
                    QTimer::singleShot(1000, [=](){
                    sendInStation(scannerName, boxId, errCount);});
                    return;
                }
                emit sigInStationRespond(scannerName, boxId, "", MesStatusError);
            }
            else
            {
                QString resultStr = jsonObj.value("ACTION").toString();
                emit sigInStationRespond(scannerName, boxId, resultStr, MESRespondSuccess);
            }
        }
    }
    );
}

QString MESInStationInterface::getDataJsonString(const QString scannerName, const QString boxId)
{
    QJsonObject jsonObj;
    jsonObj.insert("RESOURCE", QJsonValue(scannerName));
    jsonObj.insert("CONTAINER_ID", QJsonValue(boxId)); //"A217127000011"
    jsonObj.insert("ACTION", QJsonValue("S"));
    QJsonDocument doc(jsonObj);
    QString data(doc.toJson(QJsonDocument::Compact));
    return data;
}

void MESInStationInterface::analysisData(const QByteArray bytes, bool &status, QJsonObject &jsonObj)
{
//    qDebug()<<"rev msg:"<<bytes;
    QXmlStreamReader reader(bytes);
    while (!reader.atEnd())
    {
        if(reader.isStartElement())
        {
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
//                qDebug()<<"jsonObj = "<<status;
            }
            if(reader.name().toString().compare("status", Qt::CaseInsensitive) == 0)
            {
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
        }
        reader.readNext();
    }
}

QJsonObject MESInStationInterface::getJsonObj(QString jsonStr)
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

void MESInStationInterface::slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
//    qDebug()<<"log in mes";
    Q_UNUSED(reply);
    authenticator->setUser("sapint");
    authenticator->setPassword("sap12345");
}
