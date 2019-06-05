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
    //�ϴ���Ʒ��Ŵ�������󣬻�Ʒ�Ͽ���Ϣ
    bool MES_UploadBadProductList(const int &n_ScanerId,
                                  const QString &str_Code,
                                  const QList<PRODUCTINFO> &list_Product);

    //������ݺ�˳��ڷ��� ����¿� �뷴����
    bool MES_RequestExportDirection(const int &n_ScanerId,
                                    const QString &str_Code);

    //��ɷּ�Ʒ
    bool MES_CompleteSortingBadProduct(const int &n_ScanerId,
                                       const QString &str_Code);

    //�����ƷƷ����Ϣ
    bool MES_RequestProductListInfo(const int &n_ScanerId,
                                    const QString &str_Code);

    //�����Ƿ�Ϊ�տ�
    bool MES_RequestBoxIsEmpty(const int &n_ScanerId,
                               const QString &str_Code);

private slots:
    void slot_ButtonClick(int n_ID);

private slots:
    //MES����������ϴ���Ʒ��Ŵ�������󣬻�Ʒ�Ͽ���Ϣ
    void slot_Result_SendBadProductList(int n_ScanerId,
                                        QString str_Code,
                                        MESRESPOND RespondState);

    //MES������������ݺ�˳��ڷ���
    void slot_Result_RequestExportDirection(int n_ScanerId,
                                            QString str_Code,
                                            EXPORTDIRECTION Direction,
                                            MESRESPOND RespondState);

    //MES�������������״̬
    void slot_Result_Heartbeat(MESRESPOND RespondState);

    //MES�����������ɷּ�Ʒ
    void slot_Result_CompleteSortingBadProduct(int n_ScanerId,
                                               QString str_Code,
                                               MESRESPOND RespondState);

    //MES�����������ƷƷ����Ϣ
    void slot_Result_RequestProductListInfo(int n_ScanerId,
                                            QString str_Code,
                                            QList<PRODUCTINFO> list_Product,
                                            MESRESPOND RespondState);

    //MES����������Ƿ�����
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
