#include "sportcarddebugview.h"
#include "ui_sportcarddebugview.h"

#include <QMessageBox>
#include <QDebug>

SportCardDebugView::SportCardDebugView(SportCardManager * &p_SportCardManager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SportCardDebugView)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(QStringLiteral("机器手调试"));
    this->setFixedSize(this->size());

    m_pSportCardManager = p_SportCardManager;
}

SportCardDebugView::~SportCardDebugView()
{
    delete ui;
}

bool SportCardDebugView::SportCardCmd_GetRemoveFullBox(const int &n_ID)
{
    if(m_pSportCardManager == NULL)
        return false;

    if(!m_pSportCardManager->Cmd_GetRemoveFullBox(n_ID)){
        return false;
    }
    return true;
}

bool SportCardDebugView::SportCardCmd_Cmd_GetRemoveEmptyBox(const int &n_ID)
{
    if(m_pSportCardManager == NULL)
        return false;

    if(!m_pSportCardManager->Cmd_GetRemoveEmptyBox(n_ID)){
        return false;
    }
    return true;
}

bool SportCardDebugView::SportCardCmd_Cmd_SendFullReady(const int &n_ID)
{
    if(m_pSportCardManager == NULL)
        return false;

    if(!m_pSportCardManager->Cmd_SendFullReady(n_ID)){
        return false;
    }
    return true;
}

bool SportCardDebugView::SportCardCmd_Cmd_SendEmptyReady(const int &n_ID)
{
    if(m_pSportCardManager == NULL)
        return false;

    if(!m_pSportCardManager->Cmd_SendEmptyReady(n_ID)){
        return false;
    }
    return true;
}

bool SportCardDebugView::SportCardCmd_WriteProductInfo(const int &n_ID, QList<PRODUCTINFO_> &list_ProductInfo)
{
    if(m_pSportCardManager == NULL)
        return false;

    if(!m_pSportCardManager->WriteProductInfo(n_ID, list_ProductInfo)){
        return false;
    }
    return true;
}

bool SportCardDebugView::SportCardCmd_GetProductInfo(const int &n_ID, QList<PRODUCTINFO_> &list_ProductInfo)
{
    if(m_pSportCardManager == NULL)
        return false;

    if(!m_pSportCardManager->GetProductInfo(n_ID, list_ProductInfo)){
        return false;
    }
    return true;
}

void SportCardDebugView::GetTableInfo(QTableWidget *&p_QTableWidget,
                                      QList<PRODUCTINFO_> &list_Product)
{
    PRODUCTINFO_ ProductInfo;

    for(int i = 0; i < p_QTableWidget->rowCount(); i++){
        if(p_QTableWidget->item(i,0) != NULL){
           ProductInfo.str_ID =  p_QTableWidget->item(0,i)->text();
        }

        if(p_QTableWidget->item(i,1) != NULL){
           ProductInfo.str_Quality =  p_QTableWidget->item(0,i)->text();
        }

        ProductInfo.n_SequencnNumber = i;

        list_Product.append(ProductInfo);
    }
}

void SportCardDebugView::SetTableInfo(QTableWidget *&p_QTableWidget,
                                      const QList<PRODUCTINFO_> &list_Product)
{
    if(list_Product.size() > 48)
        return;

    for(int i = 0; i < list_Product.size(); i++){
        if(list_Product.at(i).n_SequencnNumber != i)
            continue;

        if(p_QTableWidget->item(i,0) != NULL){
           p_QTableWidget->item(0,i)->setText(list_Product.at(i).str_ID);
        }

        if(p_QTableWidget->item(i,1) != NULL){
           p_QTableWidget->item(0,i)->setText(list_Product.at(i).str_Quality);
        }
    }
}

void SportCardDebugView::ErrorBox(QString str_Info)
{
    qWarning()<<"SportCardDebugView "<<str_Info;

    QMessageBox::warning(
                NULL,
                QStringLiteral("警告"),
                str_Info
                );
}
