#ifndef SDMESMANAGER_H
#define SDMESMANAGER_H

#include <QObject>
#include <QThread>

#include "SD_MES/mesaddbackfixbox.h"
#include "SD_MES/mesinstationinterface.h"
#include "SD_MES/mesnettest.h"
#include "SD_MES/mesremovebadbox.h"
#include "SD_MES/mesreqbadinfo.h"
#include "SD_MES/mesreqboxisempty.h"
#include "SD_MES/sd_define.h"

class SDMesManager : public QThread
{
    Q_OBJECT
public:
    explicit SDMesManager(QThread *parent = 0);
    ~SDMesManager();

private:
    void InitSDMesManager();

    void ExitSDMesManager();

public:
    //����MES��ַ
    bool SetMesUrl(MESURLTYPE MESUrl_Type);

    //�ϴ���Ʒ��Ŵ�������󣬻�Ʒ�Ͽ���Ϣ
    bool UploadBadProductList(const int &n_ScanerId,
                              const QString &str_Code,
                              const QList<PRODUCTINFO> &list_Product);

    //������ݺ�˳��ڷ��� ����¿� �뷴����
    bool RequestExportDirection(const int &n_ScanerId,
                                const QString &str_Code);

    //��ɷּ�Ʒ
    bool CompleteSortingBadProduct(const int &n_ScanerId,
                                   const QString &str_Code);

    //�����ƷƷ����Ϣ
    bool RequestProductListInfo(const int &n_ScanerId,
                                const QString &str_Code);

    //�����Ƿ�Ϊ�տ�
    bool RequestBoxIsEmpty(const int &n_ScanerId,
                           const QString &str_Code);


private:
    void WorkSleep(int n_Msec);

    //����ӿ�����
    void Heartbeat();

    bool TD_JsonToQList(const QJsonArray &json_ProductList,
                        QList<PRODUCTINFO> &list_Product);

    bool TD_QListToJson(const QList<PRODUCTINFO> &list_Product,
                        QJsonArray &json_ProductList);

    MESRESPOND TD_MesRespondState(const int &n_Result);

    BOXSTATE TD_BoxState(const int &n_BoxType);

protected:
    void run();

signals:
    //MES����������ϴ���Ʒ��Ŵ�������󣬻�Ʒ�Ͽ���Ϣ
    void sig_Result_SendBadProductList(int n_ScanerId,
                                       QString str_Code,
                                       MESRESPOND RespondState);

    //MES������������ݺ�˳��ڷ���
    void sig_Result_RequestExportDirection(int n_ScanerId,
                                           QString str_Code,
                                           EXPORTDIRECTION Direction,
                                           MESRESPOND RespondState);

    //MES�������������״̬
    void sig_Result_Heartbeat(MESRESPOND RespondState);

    //MES�����������ɷּ�Ʒ
    void sig_Result_CompleteSortingBadProduct(int n_ScanerId,
                                              QString str_Code,
                                              MESRESPOND RespondState);

    //MES�����������ƷƷ����Ϣ
    void sig_Result_RequestProductListInfo(int n_ScanerId,
                                           QString str_Code,
                                           QList<PRODUCTINFO> list_Product,
                                           MESRESPOND RespondState);

    //MES����������Ƿ�����
    void sig_Result_RequestBoxIsEmpty(int n_ScanerId,
                                      QString str_Code,
                                      BOXSTATE BoxState,
                                      MESRESPOND RespondState);

public slots:

private slots:
    //MES����������ϴ���Ʒ��Ŵ�������󣬻�Ʒ�Ͽ���Ϣ
    void slot_Result_SendBadProductList(QString str_ScanerId,
                                        QString str_Code,
                                        int n_Result);

    //MES������������ݺ�˳��ڷ���
    //str_ResultInfo FFR==������  IN==�����¿�  n_Result
    void slot_Result_RequestExportDirection(QString str_ScanerId,
                                            QString str_Code,
                                            QString str_ResultInfo,
                                            int n_Result);

    //MES�������������״̬
    void slot_Result_Heartbeat(int n_Type);

    //MES�����������ɷּ�Ʒ
    void slot_Result_CompleteSortingBadProduct(QString str_ScanerId,
                                               QString str_Code,
                                               int n_Result);

    //MES�����������ƷƷ����Ϣ
    void slot_Result_RequestProductListInfo(QString str_ScanerId,
                                            QString str_Code,
                                            QJsonArray json_ProductList,
                                            int n_Result);

    //MES����������Ƿ�����
    //�źŷ���Ϊ���ս��  n_BoxType 1==�տ�  2==����  3==�Ͽ򲻴���ϵͳ��
    void slot_Result_RequestBoxIsEmpty(QString str_ScanerId,
                                       QString str_Code,
                                       int n_BoxType,
                                       int n_Result);

private:
    MESAddBackFixBox *m_pMESAddBackFixBox;
    MESInStationInterface *m_pMESInStationInterface;
    MESNetTest *m_pMESNetTest;
    MESRemoveBadBox *m_pMESRemoveBadBox;
    MESReqBadInfo *m_pMESReqBadInfo;
    MESReqBoxIsEmpty *m_pMESReqBoxIsEmpty;

    QUrl m_oMesUrl;

    bool m_bRunSwitch;
};

#endif // SDMESMANAGER_H
