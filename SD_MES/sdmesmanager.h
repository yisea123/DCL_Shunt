#ifndef SDMESMANAGER_H
#define SDMESMANAGER_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QMap>

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
    //设置MES地址
    bool SetMesUrl(MESURLTYPE MESUrl_Type);

    //上传坏品存放处，满框后，坏品料框信息
    bool UploadBadProductList(const int &n_ScanerId,
                              const QString &str_Code,
                              const QList<PRODUCTINFO> &list_Product);

    //请求分容后端出口方向 入高温库 入反分容
    bool RequestExportDirection(const int &n_ScanerId,
                                const QString &str_Code);

    //完成分拣坏品
    bool CompleteSortingBadProduct(const int &n_ScanerId,
                                   const QString &str_Code);

    //请求产品品质信息
    bool RequestProductListInfo(const int &n_ScanerId,
                                const QString &str_Code);

    //请求是否为空框
    bool RequestBoxIsEmpty(const int &n_ScanerId,
                           const QString &str_Code);


private:
    void WorkSleep(int n_Msec);

    //网络接口心跳
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
    //MES反馈结果，上传坏品存放处，满框后，坏品料框信息
    void sig_Result_SendBadProductList(int n_ScanerId,
                                       QString str_Code,
                                       MESRESPOND RespondState);

    //MES反馈结果，分容后端出口方向
    void sig_Result_RequestExportDirection(int n_ScanerId,
                                           QString str_Code,
                                           EXPORTDIRECTION Direction,
                                           MESRESPOND RespondState);

    //MES反馈结果，心跳状态
    void sig_Result_Heartbeat(MESRESPOND RespondState);

    //MES反馈结果，完成分拣坏品
    void sig_Result_CompleteSortingBadProduct(int n_ScanerId,
                                              QString str_Code,
                                              MESRESPOND RespondState);

    //MES反馈结果，产品品质信息
    void sig_Result_RequestProductListInfo(int n_ScanerId,
                                           QString str_Code,
                                           QList<PRODUCTINFO> list_Product,
                                           MESRESPOND RespondState);

    //MES反馈结果，是否满框
    void sig_Result_RequestBoxIsEmpty(int n_ScanerId,
                                      QString str_Code,
                                      BOXSTATE BoxState,
                                      MESRESPOND RespondState);

private slots:
    void slot_TimeOut();

private slots:
    //MES反馈结果，上传坏品存放处，满框后，坏品料框信息
    void slot_Result_SendBadProductList(QString str_ScanerId,
                                        QString str_Code,
                                        int n_Result);

    //MES反馈结果，分容后端出口方向
    //str_ResultInfo FFR==反分容  IN==进高温库  n_Result
    void slot_Result_RequestExportDirection(QString str_ScanerId,
                                            QString str_Code,
                                            QString str_ResultInfo,
                                            int n_Result);

    //MES反馈结果，心跳状态
    void slot_Result_Heartbeat(int n_Type);

    //MES反馈结果，完成分拣坏品
    void slot_Result_CompleteSortingBadProduct(QString str_ScanerId,
                                               QString str_Code,
                                               int n_Result);

    //MES反馈结果，产品品质信息
    void slot_Result_RequestProductListInfo(QString str_ScanerId,
                                            QString str_Code,
                                            QJsonArray json_ProductList,
                                            int n_Result);

    //MES反馈结果，是否满框
    //信号返回为最终结果  n_BoxType 1==空框  2==满框  3==料框不存在系统中
    void slot_Result_RequestBoxIsEmpty(QString str_ScanerId,
                                       QString str_Code,
                                       int n_BoxType,
                                       int n_Result);

private:
    MESNetTest *m_pMESNetTest;
//    MESAddBackFixBox *m_pMESAddBackFixBox;
//    MESInStationInterface *m_pMESInStationInterface;
//    MESRemoveBadBox *m_pMESRemoveBadBox;
//    MESReqBadInfo *m_pMESReqBadInfo;
//    MESReqBoxIsEmpty *m_pMESReqBoxIsEmpty;

    QMap<int, MESAddBackFixBox *> m_mapAddBackFixBox;
    QMap<int, MESInStationInterface *> m_mapInStationInterface;
    QMap<int, MESRemoveBadBox *> m_mapRemoveBadBox;
    QMap<int, MESReqBadInfo *> m_mapReqBadInfo;
    QMap<int, MESReqBoxIsEmpty *> m_mapReqBoxIsEmpty;

    QUrl m_oMesUrl;

    QTimer *m_pQTimer;

    bool m_bRunSwitch;
};

#endif // SDMESMANAGER_H
