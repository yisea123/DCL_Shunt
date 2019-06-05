#include "datafile.h"
#include <QApplication>

#include <QDir>
#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>

bool DataFile::GetScanerIPList(QList<QString> &list_IP)
{
    QString str_FilePath = qApp->applicationDirPath() +
                "/Config";

        QDir o_QDir(str_FilePath);
        if(!o_QDir.exists()){
            o_QDir.mkpath(str_FilePath);
        }

        str_FilePath += "/ScanerIP.xml";

        QFile o_QFileConfig(str_FilePath);

        if(!o_QFileConfig.open(QIODevice::ReadOnly | QIODevice::Text)){
            return false;
        }

        QXmlStreamReader o_QXmlStreamReader(&o_QFileConfig);
        while(!o_QXmlStreamReader.atEnd()){
            QXmlStreamReader::TokenType Token = o_QXmlStreamReader.readNext();
            if(Token == QXmlStreamReader::StartDocument){
                continue;
            }

            if(Token == QXmlStreamReader::EndElement){
                continue;
            }

            if(Token == QXmlStreamReader::Characters){
                continue;
            }

            if(o_QXmlStreamReader.name().contains("IP-")){
                o_QXmlStreamReader.readNext();
                list_IP.append(o_QXmlStreamReader.text().toString());
            }
        }

        if(o_QXmlStreamReader.hasError()){
            qDebug() << QString::fromLocal8Bit("xml错误信息：%1  行号：%2  列号：%3  字符位移：%4").
                        arg(o_QXmlStreamReader.errorString()).
                        arg(o_QXmlStreamReader.lineNumber()).
                        arg(o_QXmlStreamReader.columnNumber()).
                        arg(o_QXmlStreamReader.characterOffset());

            o_QFileConfig.close();
            return false;
        }

        o_QFileConfig.close();
        return true;
}

bool DataFile::GetPLCIP(QString &str_IP)
{
    QString str_FilePath = qApp->applicationDirPath() +
                "/Config";

        QDir o_QDir(str_FilePath);
        if(!o_QDir.exists()){
            o_QDir.mkpath(str_FilePath);
        }

        str_FilePath += "/PLCIP.xml";

        QFile o_QFileConfig(str_FilePath);

        if(!o_QFileConfig.open(QIODevice::ReadOnly | QIODevice::Text)){
            return false;
        }

        QXmlStreamReader o_QXmlStreamReader(&o_QFileConfig);
        while(!o_QXmlStreamReader.atEnd()){
            QXmlStreamReader::TokenType Token = o_QXmlStreamReader.readNext();
            if(Token == QXmlStreamReader::StartDocument){
                continue;
            }

            if(Token == QXmlStreamReader::EndElement){
                continue;
            }

            if(Token == QXmlStreamReader::Characters){
                continue;
            }

            if(o_QXmlStreamReader.name() == "IP"){
                o_QXmlStreamReader.readNext();
                str_IP = o_QXmlStreamReader.text().toString();
            }
        }

        if(o_QXmlStreamReader.hasError()){
            qDebug() << QString::fromLocal8Bit("xml错误信息：%1  行号：%2  列号：%3  字符位移：%4").
                        arg(o_QXmlStreamReader.errorString()).
                        arg(o_QXmlStreamReader.lineNumber()).
                        arg(o_QXmlStreamReader.columnNumber()).
                        arg(o_QXmlStreamReader.characterOffset());
            o_QFileConfig.close();
            return false;
        }

        o_QFileConfig.close();
        return true;
}

bool DataFile::GetSportCardIPList(QList<QString> &list_IP)
{
    QString str_FilePath = qApp->applicationDirPath() +
                "/Config";

        QDir o_QDir(str_FilePath);
        if(!o_QDir.exists()){
            o_QDir.mkpath(str_FilePath);
        }

        str_FilePath += "/SportCardIP.xml";

        QFile o_QFileConfig(str_FilePath);

        if(!o_QFileConfig.open(QIODevice::ReadOnly | QIODevice::Text)){
            return false;
        }

        QXmlStreamReader o_QXmlStreamReader(&o_QFileConfig);
        while(!o_QXmlStreamReader.atEnd()){
            QXmlStreamReader::TokenType Token = o_QXmlStreamReader.readNext();
            if(Token == QXmlStreamReader::StartDocument){
                continue;
            }

            if(Token == QXmlStreamReader::EndElement){
                continue;
            }

            if(Token == QXmlStreamReader::Characters){
                continue;
            }

            if(o_QXmlStreamReader.name().contains("IP-")){
                o_QXmlStreamReader.readNext();
                list_IP.append(o_QXmlStreamReader.text().toString());
            }
        }

        if(o_QXmlStreamReader.hasError()){
            qDebug() << QString::fromLocal8Bit("xml错误信息：%1  行号：%2  列号：%3  字符位移：%4").
                        arg(o_QXmlStreamReader.errorString()).
                        arg(o_QXmlStreamReader.lineNumber()).
                        arg(o_QXmlStreamReader.columnNumber()).
                        arg(o_QXmlStreamReader.characterOffset());
            o_QFileConfig.close();
            return false;
        }

        o_QFileConfig.close();
        return true;
}

bool DataFile::SaveScanerIPList(const QList<QString> &list_IP)
{
    QString str_FilePath = qApp->applicationDirPath() +
                "/Config";

        QDir o_QDir(str_FilePath);
        if(!o_QDir.exists()){
            o_QDir.mkpath(str_FilePath);
        }

        str_FilePath += "/ScanerIP.xml";

        QFile o_QFileConfig(str_FilePath);

        if(!o_QFileConfig.open(QIODevice::WriteOnly | QIODevice::Text)){
            return false;
        }

        QXmlStreamWriter o_QXmlStreamWriter(&o_QFileConfig);
        o_QXmlStreamWriter.setAutoFormatting(true);
        o_QXmlStreamWriter.writeStartDocument();
        o_QXmlStreamWriter.writeStartElement("ScanerIP");

        QString str_TextName = "";
        for(int i = 1; i <= list_IP.size(); i++){
            str_TextName = "IP-" + QString::number(i);
            o_QXmlStreamWriter.writeTextElement(str_TextName, list_IP.at(i - 1));
        }

        o_QXmlStreamWriter.writeEndElement();
        o_QXmlStreamWriter.writeEndDocument();
        o_QFileConfig.close();

        return true;
}

bool DataFile::SavePLCIP(const QString &str_IP)
{
    QString str_FilePath = qApp->applicationDirPath() +
                "/Config";

        QDir o_QDir(str_FilePath);
        if(!o_QDir.exists()){
            o_QDir.mkpath(str_FilePath);
        }

        str_FilePath += "/PLCIP.xml";

        QFile o_QFileConfig(str_FilePath);

        if(!o_QFileConfig.open(QIODevice::WriteOnly | QIODevice::Text)){
            return false;
        }

        QXmlStreamWriter o_QXmlStreamWriter(&o_QFileConfig);
        o_QXmlStreamWriter.setAutoFormatting(true);
        o_QXmlStreamWriter.writeStartDocument();
        o_QXmlStreamWriter.writeStartElement("PLCIP");

        o_QXmlStreamWriter.writeTextElement("IP", str_IP);

        o_QXmlStreamWriter.writeEndElement();
        o_QXmlStreamWriter.writeEndDocument();
        o_QFileConfig.close();

        return true;
}

bool DataFile::SaveSportCardIPList(const QList<QString> &list_IP)
{
    QString str_FilePath = qApp->applicationDirPath() +
                "/Config";

        QDir o_QDir(str_FilePath);
        if(!o_QDir.exists()){
            o_QDir.mkpath(str_FilePath);
        }

        str_FilePath += "/SportCardIP.xml";

        QFile o_QFileConfig(str_FilePath);

        if(!o_QFileConfig.open(QIODevice::WriteOnly | QIODevice::Text)){
            return false;
        }

        QXmlStreamWriter o_QXmlStreamWriter(&o_QFileConfig);
        o_QXmlStreamWriter.setAutoFormatting(true);
        o_QXmlStreamWriter.writeStartDocument();
        o_QXmlStreamWriter.writeStartElement("SportCardIP");

        QString str_TextName = "";
        for(int i = 1; i <= list_IP.size(); i++){
            str_TextName = "IP-" + QString::number(i);
            o_QXmlStreamWriter.writeTextElement(str_TextName, list_IP.at(i - 1));
        }

        o_QXmlStreamWriter.writeEndElement();
        o_QXmlStreamWriter.writeEndDocument();
        o_QFileConfig.close();

        return true;
}
