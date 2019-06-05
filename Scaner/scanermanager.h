#ifndef SCANERMANAGER_H
#define SCANERMANAGER_H

#include <QObject>
#include <QMap>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>

#include "scanerdevice.h"

class ScanerManager : public QObject
{
    Q_OBJECT
public:
    explicit ScanerManager(QObject *parent = 0);
    ~ScanerManager();

    //添加扫码设备
    bool AddScanerDevice(const int &n_ID,
                         const QString &str_IP,
                         const int &n_Port);
    //移除扫码设备
    bool RemoveScanerDevice(const int &n_ID);
    //移除所有扫码设备
    bool ClearScanerDevice();
    //请求扫码
    bool RequsetCode(const int &n_ID);

    bool GetConnectScanerList(QList<int> &list_ConnectScaner);

private:
    void WorkSleep(int n_Msec);

signals:
    void sig_ConnectDevice(int n_ID);

    void sig_DisconnectDevice(int n_ID);

    void sig_RequestCode(int n_ID);

signals:
    /*
     * 请求扫码结果
     * id false 000000000000 条码扫码错误
     * id false 000000000001 设备无返回码值
     */
    void sig_CodeData(int n_ID,
                      bool b_ScanState,
                      QString str_Code);

    //设备连接状态
    void sig_ConnectState(int n_ID,
                          bool b_ConnectState);

public slots:

private slots:
    void slot_CodeData(int n_ID,
                       bool b_ScanState,
                       QString str_Code);

    void slot_ConnectState(int n_ID,
                           bool b_ConnectState);

private:
    QMap<int, ScanerDevice *> m_mapScaner;
    QMap<int, QThread *> m_mapThread;

    QList<int> m_listNeedConnectScaner;

    QList<int> m_listConnectScaner;

    QMutex m_oQMutex;
};

#endif // SCANERMANAGER_H
