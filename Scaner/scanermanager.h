#ifndef SCANERMANAGER_H
#define SCANERMANAGER_H

#include <QObject>
#include <QMap>
#include <QThread>

#include "scanerdevice.h"

class ScanerManager : public QObject
{
    Q_OBJECT
public:
    explicit ScanerManager(QObject *parent = 0);
    ~ScanerManager();

    //���ɨ���豸
    bool AddScanerDevice(const int &n_ID,
                         const QString &str_IP,
                         const int &n_Port);
    //�Ƴ�ɨ���豸
    bool RemoveScanerDevice(const int &n_ID);
    //�Ƴ�����ɨ���豸
    bool ClearScanerDevice();
    //����ɨ��
    bool RequsetCode(const int &n_ID);

private:
    void WorkSleep(int n_Msec);

signals:
    /*
     * ����ɨ����
     * id false 000000000000 ����ɨ�����
     * id false 000000000001 �豸�޷�����ֵ
     */
    void sig_CodeData(int n_ID,
                      bool b_ScanState,
                      QString str_Code);

    //�豸����״̬
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
};

#endif // SCANERMANAGER_H
