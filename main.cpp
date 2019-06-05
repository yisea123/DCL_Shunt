#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <QDebug>
#include <QtMessageHandler>
#include <QFile>
#include <QDir>

#include <UI/logviewinstancegetter.h>

using namespace std;

void WriteLog(QString str_Info, QString str_LogType)
{
    QString str_FileFolder = qApp->applicationDirPath() +
                            "/Log/" +
                            QDateTime::currentDateTime().toString("yyyy-MM-dd");

    QDir o_dir(str_FileFolder);

    if(!o_dir.exists()){
        o_dir.mkpath(str_FileFolder);
    }

    QString str_FilePath = QString("%1/%2.log").arg(str_FileFolder).arg(str_LogType);

    QString str_ToWrite = QStringLiteral("\r\n����-") + QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    str_ToWrite.append(QStringLiteral("\r\n��־��Ϣ-%1").arg(str_Info));
    str_ToWrite.append("\r\n---------------------------------------------------------------------");

    if(LogViewInstanceGetter::GetInstance() != NULL){
        LogViewInstanceGetter::GetInstance()->ShowLog(str_ToWrite);
    }

    QFile o_File(str_FilePath);
    o_File.open(QIODevice::WriteOnly | QIODevice::Append);

    QTextStream o_QTextStream(&o_File);
    o_QTextStream<<str_ToWrite;
    o_File.flush();
    o_File.close();
    //��ӡ������̨

    std::cout<<str_ToWrite.toLocal8Bit().constData() << std::endl;
 }

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    static QMutex mutex;
    mutex.lock();

    QString str_TxtMessage = "";
    QString str_MessageType = "";

    switch (type)    {
    case QtDebugMsg:    //������Ϣ��ʾ
         str_MessageType = "Debug";
         str_TxtMessage = QString("Debug: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(QString::number(context.line)).arg(context.function);
         break;
    case QtInfoMsg:
         str_MessageType = "Info";
         str_TxtMessage = QString("Warning: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(QString::number(context.line)).arg(context.function);
         break;
     case QtWarningMsg:    //һ���warning��ʾ
         str_MessageType = "Waring";
         str_TxtMessage = QString("Warning: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(QString::number(context.line)).arg(context.function);
         break;
     case QtCriticalMsg:    //���ش�����ʾ
         str_MessageType = "Critical";
         str_TxtMessage = QString("Critical: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(QString::number(context.line)).arg(context.function);
         break;
     case QtFatalMsg:    //����������ʾ
         str_MessageType = "Fatal";
         str_TxtMessage = QString("Fatal: %1 (%2:%3, %4)\n").arg(msg).arg(context.file).arg(QString::number(context.line)).arg(context.function);
         abort();
     }

     WriteLog(str_TxtMessage, str_MessageType);

     mutex.unlock();
 }

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //�����߳������Ϣ���ᵼ�³����������������̲߳���ȫ�����ƽ�
    qInstallMessageHandler(myMessageOutput);

    MainWindow w;

    w.show();

    return a.exec();
}
