#include "mesdebugview.h"
#include "ui_mesdebugview.h"
#include <QMessageBox>

#include <QDebug>

MesDebugView::MesDebugView(SDMesManager *&p_SDMesManager, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MesDebugView)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(QStringLiteral("Mes调试"));
    this->setFixedSize(this->size());

    m_pSDMesManager = p_SDMesManager;
    m_nClickID = 0;

    connect(m_pSDMesManager, &SDMesManager::sig_Result_Heartbeat,
            this, &MesDebugView::slot_Result_Heartbeat);
    connect(m_pSDMesManager, &SDMesManager::sig_Result_SendBadProductList,
            this, &MesDebugView::slot_Result_SendBadProductList);
    connect(m_pSDMesManager, &SDMesManager::sig_Result_RequestExportDirection,
            this, &MesDebugView::slot_Result_RequestExportDirection);
    connect(m_pSDMesManager, &SDMesManager::sig_Result_CompleteSortingBadProduct,
            this, &MesDebugView::slot_Result_CompleteSortingBadProduct);
    connect(m_pSDMesManager, &SDMesManager::sig_Result_RequestProductListInfo,
            this, &MesDebugView::slot_Result_RequestProductListInfo);
    connect(m_pSDMesManager, &SDMesManager::sig_Result_RequestBoxIsEmpty,
            this, &MesDebugView::slot_Result_RequestBoxIsEmpty);

    m_pButtonGroup = new QButtonGroup;
    m_pButtonGroup->addButton(ui->pb_Send_1, 1);
    m_pButtonGroup->addButton(ui->pb_Send_2, 2);
    m_pButtonGroup->addButton(ui->pb_Send_3, 3);
    m_pButtonGroup->addButton(ui->pb_Send_4, 4);
    m_pButtonGroup->addButton(ui->pb_Send_5, 5);

    connect(m_pButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_ButtonClick(int)));

}

MesDebugView::~MesDebugView()
{
    delete ui;
}

void MesDebugView::GetTableInfo(QTableWidget *&p_QTableWidget,
                                QList<PRODUCTINFO> &list_Product)
{
    PRODUCTINFO ProductInfo;

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

void MesDebugView::SetTableInfo(QTableWidget *&p_QTableWidget,
                                const QList<PRODUCTINFO> &list_Product)
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

void MesDebugView::ErrorBox(QString str_Info)
{
    qWarning()<<"MesDebugView "<<str_Info;

    QMessageBox::warning(
                NULL,
                QStringLiteral("警告"),
                str_Info
                );
}

bool MesDebugView::MES_UploadBadProductList(const int &n_ScanerId,
                                            const QString &str_Code,
                                            const QList<PRODUCTINFO> &list_Product)
{
    if(m_pSDMesManager == NULL)
        return false;

    if(!m_pSDMesManager->UploadBadProductList(n_ScanerId, str_Code, list_Product)){
        ErrorBox(QStringLiteral("上传料框信息失败!"));
        return false;
    }

    return true;
}

bool MesDebugView::MES_RequestExportDirection(const int &n_ScanerId,
                                              const QString &str_Code)
{
    if(m_pSDMesManager == NULL)
        return false;

    if(!m_pSDMesManager->RequestExportDirection(n_ScanerId, str_Code)){
        ErrorBox(QStringLiteral("请求后端出口方向失败!"));
        return false;
    }

    return true;
}

bool MesDebugView::MES_CompleteSortingBadProduct(const int &n_ScanerId,
                                                 const QString &str_Code)
{
    if(m_pSDMesManager == NULL)
        return false;

    if(!m_pSDMesManager->CompleteSortingBadProduct(n_ScanerId, str_Code)){
        ErrorBox(QStringLiteral("发送完成分拣失败!"));
        return false;
    }

    return true;
}

bool MesDebugView::MES_RequestProductListInfo(const int &n_ScanerId,
                                              const QString &str_Code)
{
    if(m_pSDMesManager == NULL)
        return false;

    if(!m_pSDMesManager->RequestProductListInfo(n_ScanerId, str_Code)){
        ErrorBox(QStringLiteral("请求料框信息失败!"));
        return false;
    }

    return true;
}

bool MesDebugView::MES_RequestBoxIsEmpty(const int &n_ScanerId,
                                         const QString &str_Code)
{
    if(m_pSDMesManager == NULL)
        return false;

    if(!m_pSDMesManager->RequestBoxIsEmpty(n_ScanerId, str_Code)){
        ErrorBox(QStringLiteral("识别空框或满框失败!"));
        return false;
    }

    return true;
}

void MesDebugView::slot_ButtonClick(int n_ID)
{
    QString str_Code = ui->le_Code->text();
    QString str_ID = ui->le_ID->text();

    if(str_Code == "" || str_ID == ""){
        ErrorBox(QStringLiteral("ID，条形码数据不能为空!"));
        return;
    }

    switch (n_ID) {
    case 1:{
        if(MES_RequestBoxIsEmpty(str_ID.toInt(), str_Code))
            return;
        break;
    }
    case 2:{
        if(MES_RequestExportDirection(str_ID.toInt(), str_Code))
            return;
        break;
    }
    case 3:{
        if(MES_RequestProductListInfo(str_ID.toInt(), str_Code))
            return;
        break;
    }
    case 4:{
        QList<PRODUCTINFO> list_Product;
        GetTableInfo(ui->tb_Send, list_Product);

        if(MES_UploadBadProductList(str_ID.toInt(), str_Code, list_Product))
            return;
        break;
    }
    case 5:{
        if(MES_CompleteSortingBadProduct(str_ID.toInt(), str_Code))
            return;
        break;
    }
    default:
        break;
    }

    QPushButton *p_PushButton;

    if(m_nClickID == 0){
        p_PushButton = qobject_cast<QPushButton*>(m_pButtonGroup->button(n_ID));
        p_PushButton->setStyleSheet(Click_Style);
    }
    else{
        p_PushButton = qobject_cast<QPushButton*>(m_pButtonGroup->button(m_nClickID));
        p_PushButton->setStyleSheet(NoClick_Style);

        p_PushButton = qobject_cast<QPushButton*>(m_pButtonGroup->button(n_ID));
        p_PushButton->setStyleSheet(Click_Style);
    }

    m_nClickID = n_ID;
}

void MesDebugView::slot_Result_SendBadProductList(int n_ScanerId,
                                                  QString str_Code,
                                                  MESRESPOND RespondState)
{
    Q_UNUSED(n_ScanerId);

    ui->lb_State_Succeed_4->setStyleSheet(ShowNoState);
    ui->lb_State_NetError_4->setStyleSheet(ShowNoState);
    ui->lb_State_StateError_4->setStyleSheet(ShowNoState);
    ui->lb_State_Data_Error_4->setStyleSheet(ShowNoState);

    if(RespondState == RESPONDSUCCEED){
        ui->lb_State_Succeed_4->setStyleSheet(ShowState);
    }
    else if(RespondState == NETWORKERROR){
        ui->lb_State_NetError_4->setStyleSheet(ShowState);
    }
    else if(RespondState == STATUSERROR){
        ui->lb_State_StateError_4->setStyleSheet(ShowState);
    }
    else if(RespondState == DATAERROR){
        ui->lb_State_Data_Error_4->setStyleSheet(ShowState);
    }
}

void MesDebugView::slot_Result_RequestExportDirection(int n_ScanerId,
                                                      QString str_Code,
                                                      EXPORTDIRECTION Direction,
                                                      MESRESPOND RespondState)
{
    Q_UNUSED(n_ScanerId);
    Q_UNUSED(Direction);

    ui->lb_State_Succeed_1->setStyleSheet(ShowNoState);
    ui->lb_State_NetError_1->setStyleSheet(ShowNoState);
    ui->lb_State_StateError_1->setStyleSheet(ShowNoState);
    ui->lb_State_Data_Error_1->setStyleSheet(ShowNoState);

    if(RespondState == RESPONDSUCCEED){
        ui->lb_State_Succeed_1->setStyleSheet(ShowState);
    }
    else if(RespondState == NETWORKERROR){
        ui->lb_State_NetError_1->setStyleSheet(ShowState);
    }
    else if(RespondState == STATUSERROR){
        ui->lb_State_StateError_1->setStyleSheet(ShowState);
    }
    else if(RespondState == DATAERROR){
        ui->lb_State_Data_Error_1->setStyleSheet(ShowState);
    }
}

void MesDebugView::slot_Result_Heartbeat(MESRESPOND RespondState)
{
    if(RespondState == RESPONDSUCCEED){
        ui->le_State->setText(QStringLiteral("已连接"));
    }
    else if(RespondState == NETWORKERROR){
        ui->le_State->setText(QStringLiteral("断开"));
    }
    else if(RespondState == STATUSERROR){
        ui->le_State->setText(QStringLiteral("状态异常"));
    }
    else if(RespondState == DATAERROR){
        ui->le_State->setText(QStringLiteral("数据错误"));
    }
}

void MesDebugView::slot_Result_CompleteSortingBadProduct(int n_ScanerId,
                                                         QString str_Code,
                                                         MESRESPOND RespondState)
{
    Q_UNUSED(n_ScanerId);

    ui->lb_State_Succeed_5->setStyleSheet(ShowNoState);
    ui->lb_State_NetError_5->setStyleSheet(ShowNoState);
    ui->lb_State_StateError_5->setStyleSheet(ShowNoState);
    ui->lb_State_Data_Error_5->setStyleSheet(ShowNoState);

    if(RespondState == RESPONDSUCCEED){
        ui->lb_State_Succeed_5->setStyleSheet(ShowState);
    }
    else if(RespondState == NETWORKERROR){
        ui->lb_State_NetError_5->setStyleSheet(ShowState);
    }
    else if(RespondState == STATUSERROR){
        ui->lb_State_StateError_5->setStyleSheet(ShowState);
    }
    else if(RespondState == DATAERROR){
        ui->lb_State_Data_Error_5->setStyleSheet(ShowState);
    }
}

void MesDebugView::slot_Result_RequestProductListInfo(int n_ScanerId,
                                                      QString str_Code,
                                                      QList<PRODUCTINFO> list_Product, MESRESPOND RespondState)
{
    Q_UNUSED(n_ScanerId);

    SetTableInfo(ui->tb_Get, list_Product);

    ui->lb_State_Succeed_3->setStyleSheet(ShowNoState);
    ui->lb_State_NetError_3->setStyleSheet(ShowNoState);
    ui->lb_State_StateError_3->setStyleSheet(ShowNoState);
    ui->lb_State_Data_Error_3->setStyleSheet(ShowNoState);

    if(RespondState == RESPONDSUCCEED){
        ui->lb_State_Succeed_3->setStyleSheet(ShowState);
    }
    else if(RespondState == NETWORKERROR){
        ui->lb_State_NetError_3->setStyleSheet(ShowState);
    }
    else if(RespondState == STATUSERROR){
        ui->lb_State_StateError_3->setStyleSheet(ShowState);
    }
    else if(RespondState == DATAERROR){
        ui->lb_State_Data_Error_3->setStyleSheet(ShowState);
    }
}

void MesDebugView::slot_Result_RequestBoxIsEmpty(int n_ScanerId,
                                                 QString str_Code,
                                                 BOXSTATE BoxState,
                                                 MESRESPOND RespondState)
{
    Q_UNUSED(n_ScanerId);
    Q_UNUSED(BoxState);

    ui->lb_State_Succeed_2->setStyleSheet(ShowNoState);
    ui->lb_State_NetError_2->setStyleSheet(ShowNoState);
    ui->lb_State_StateError_2->setStyleSheet(ShowNoState);
    ui->lb_State_Data_Error_2->setStyleSheet(ShowNoState);

    if(RespondState == RESPONDSUCCEED){
        ui->lb_State_Succeed_2->setStyleSheet(ShowState);
    }
    else if(RespondState == NETWORKERROR){
        ui->lb_State_NetError_2->setStyleSheet(ShowState);
    }
    else if(RespondState == STATUSERROR){
        ui->lb_State_StateError_2->setStyleSheet(ShowState);
    }
    else if(RespondState == DATAERROR){
        ui->lb_State_Data_Error_2->setStyleSheet(ShowState);
    }
}
