#ifndef MESDEBUGVIEW_H
#define MESDEBUGVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QButtonGroup>

#include "ui_define.h"
#include <SD_MES/sdmesmanager.h>

namespace Ui {
class MesDebugView;
}

class MesDebugView : public QWidget
{
    Q_OBJECT
public:
    explicit MesDebugView(SDMesManager * &p_SDMesManager, QWidget *parent = nullptr);
    ~MesDebugView();

private:
    void GetTableInfo(QTableWidget *& p_QTableWidget, QList<PRODUCTINFO> &list_Product);

    void SetTableInfo(QTableWidget *& p_QTableWidget, const QList<PRODUCTINFO> &list_Product);

    void ErrorBox(QString str_Info);

private:
    //上传坏品存放处，满框后，坏品料框信息
    bool MES_UploadBadProductList(const int &n_ScanerId,
                                  const QString &str_Code,
                                  const QList<PRODUCTINFO> &list_Product);

    //请求分容后端出口方向 入高温库 入反分容
    bool MES_RequestExportDirection(const int &n_ScanerId,
                                    const QString &str_Code);

    //完成分拣坏品
    bool MES_CompleteSortingBadProduct(const int &n_ScanerId,
                                       const QString &str_Code);

    //请求产品品质信息
    bool MES_RequestProductListInfo(const int &n_ScanerId,
                                    const QString &str_Code);

    //请求是否为空框
    bool MES_RequestBoxIsEmpty(const int &n_ScanerId,
                               const QString &str_Code);

private slots:
    void slot_ButtonClick(int n_ID);

private slots:
    //MES反馈结果，上传坏品存放处，满框后，坏品料框信息
    void slot_Result_SendBadProductList(int n_ScanerId,
                                        QString str_Code,
                                        MESRESPOND RespondState);

    //MES反馈结果，分容后端出口方向
    void slot_Result_RequestExportDirection(int n_ScanerId,
                                            QString str_Code,
                                            EXPORTDIRECTION Direction,
                                            MESRESPOND RespondState);

    //MES反馈结果，心跳状态
    void slot_Result_Heartbeat(MESRESPOND RespondState);

    //MES反馈结果，完成分拣坏品
    void slot_Result_CompleteSortingBadProduct(int n_ScanerId,
                                               QString str_Code,
                                               MESRESPOND RespondState);

    //MES反馈结果，产品品质信息
    void slot_Result_RequestProductListInfo(int n_ScanerId,
                                            QString str_Code,
                                            QList<PRODUCTINFO> list_Product,
                                            MESRESPOND RespondState);

    //MES反馈结果，是否满框
    void slot_Result_RequestBoxIsEmpty(int n_ScanerId,
                                       QString str_Code,
                                       BOXSTATE BoxState,
                                       MESRESPOND RespondState);


private:
    Ui::MesDebugView *ui;

    SDMesManager *m_pSDMesManager;

    QButtonGroup *m_pButtonGroup;
    int m_nClickID;
};

#endif // MESDEBUGVIEW_H
