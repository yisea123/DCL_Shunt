﻿#include "mesreqbadinfo.h"
#include <QXmlStreamReader>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QTimer>
#include <QStringRef>

MESReqBadInfo::MESReqBadInfo(QUrl url, QObject *parent) : QObject(parent)
{
    mesUrl = url;
}

void MESReqBadInfo::sendReqBadInfo(QString scannerName, QString boxId)
{
    int resendCount = 0;
    sendReqBadInfo(scannerName, boxId, resendCount);
}

void MESReqBadInfo::setMesUrl(const QUrl url)
{
    mesUrl = url;
}

void MESReqBadInfo::sendReqBadInfo(const QString scannerName, const QString boxId, int resendCount)
{
    QJsonObject jsonObj;
    jsonObj.insert("RESOURCE", scannerName);
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
            this, &MESReqBadInfo::slotAuthenticationRequired);
    QNetworkReply *reply = manager->post(req,soapXML.toUtf8());
    qDebug()<<"pos msg"<<soapXML.toUtf8();

    QAbstractSocket::connect(reply,&QNetworkReply::finished,[=](){
        if(reply->error() != QNetworkReply::NoError)
        {
            qDebug()<<"error msg:"<<reply->errorString();
            int errCount = resendCount + 1;
            if(errCount < NetworkErrorResendTimes)
            {
                QTimer::singleShot(1000, [=](){
                    sendReqBadInfo(scannerName, boxId, errCount);});
                return;
            }
//            emit sigIsEmptyRespond(scannerName, boxId, NetWorkError);
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
                        sendReqBadInfo(scannerName, boxId, errCount);});
                    return;
                }
//                emit sigIsEmptyRespond(scannerName, boxId, MesStatusError);
            }
            else
            {
//                emit sigIsEmptyRespond(scannerName, boxId, result);
            }
        }
    }
    );
}

void MESReqBadInfo::analysisData(const QByteArray bytes, QJsonObject &jsonObj, bool &status)
{
    qDebug()<<"rec msg:"<<bytes;
    QXmlStreamReader reader(bytes);
    while (!reader.atEnd())
    {
        if(reader.isStartElement())
        {
            if(reader.name().toString().compare("status", Qt::CaseInsensitive) == 0)
            {
                qDebug()<<reader.name();
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
                    qDebug()<<"status = "<<status;
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

QJsonObject MESReqBadInfo::getJsonObj(QString jsonStr)
{
    QJsonParseError jsonRrr;
    QJsonDocument jsonDoc(QJsonDocument::fromJson(jsonStr.toUtf8(), &jsonRrr));
    if(jsonRrr.error != QJsonParseError::NoError)
    {
        qDebug() << "mes json error";
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

void MESReqBadInfo::slotAuthenticationRequired(QNetworkReply *reply, QAuthenticator *authenticator)
{
    qDebug()<<"log in mes";
    Q_UNUSED(reply);
    authenticator->setUser("sapint");
    authenticator->setPassword("sap12345");
}
