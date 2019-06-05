#include "scanerdebugview.h"
#include "ui_scanerdebugview.h"

#include <QMessageBox>
#include <QDebug>
#include <ConfigFile/datafile.h>

ScanerDebugView::ScanerDebugView(ScanerManager *&p_ScanerManage,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScanerDebugView)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(QStringLiteral("É¨ÂëÇ¹µ÷ÊÔ"));
    this->setFixedSize(this->size());

    m_pButtonGroup = NULL;
    m_nClickID = 0;
    m_pButtonGroup = NULL;

    m_pScanerManager = p_ScanerManage;

    connect(m_pScanerManager, &ScanerManager::sig_CodeData,
            this, &ScanerDebugView::slot_ScanCodeData);
    connect(m_pScanerManager, &ScanerManager::sig_ConnectState,
            this, &ScanerDebugView::slot_ScanerConnectState);

    InitView();
}


ScanerDebugView::~ScanerDebugView()
{
    if(m_pButtonGroup != NULL){
        delete m_pButtonGroup;
        m_pButtonGroup = NULL;
    }

    delete ui;
}

bool ScanerDebugView::AddScaner(const int &n_ID)
{
    if(m_pScanerManager == NULL)
        return false;

    if(!m_pScanerManager->AddScanerDevice(n_ID, m_listScanerIP.at(n_ID - 1), 4001)){
        ErrorBox(QStringLiteral("Á¬½ÓÉ¨ÂëÊ§°Ü"));
        return false;
    }
    return true;
}

bool ScanerDebugView::RemoveSacner(const int &n_ID)
{
    if(m_pScanerManager == NULL)
        return false;

    if(!m_pScanerManager->RemoveScanerDevice(n_ID)){
        ErrorBox(QStringLiteral("¶Ï¿ªÉ¨ÂëÇ¹Ê§°Ü"));
        return false;
    }
    return true;
}

bool ScanerDebugView::RequsetCode(const int &n_ID)
{
    if(m_pScanerManager == NULL)
        return false;

    if(!m_pScanerManager->RequsetCode(n_ID)){
        ErrorBox(QStringLiteral("ÇëÇóÉ¨ÂëÊ§°Ü"));
        return false;
    }
    return true;
}

void ScanerDebugView::InitView()
{
    m_pButtonGroup = new QButtonGroup;

    m_pButtonGroup->addButton(ui->pb_Connect_1, 1);
    m_pButtonGroup->addButton(ui->pb_Connect_2, 2);
    m_pButtonGroup->addButton(ui->pb_Connect_3, 3);
    m_pButtonGroup->addButton(ui->pb_Connect_4, 4);
    m_pButtonGroup->addButton(ui->pb_Connect_5, 5);
    m_pButtonGroup->addButton(ui->pb_Connect_6, 6);
    m_pButtonGroup->addButton(ui->pb_Connect_7, 7);
    m_pButtonGroup->addButton(ui->pb_Connect_8, 8);
    m_pButtonGroup->addButton(ui->pb_Connect_9, 9);
    m_pButtonGroup->addButton(ui->pb_Connect_10, 10);
    m_pButtonGroup->addButton(ui->pb_Connect_11, 11);
    m_pButtonGroup->addButton(ui->pb_Connect_12, 12);
    m_pButtonGroup->addButton(ui->pb_Connect_13, 13);

    m_pButtonGroup->addButton(ui->pb_Disconnect_1, 21);
    m_pButtonGroup->addButton(ui->pb_Disconnect_2, 22);
    m_pButtonGroup->addButton(ui->pb_Disconnect_3, 23);
    m_pButtonGroup->addButton(ui->pb_Disconnect_4, 24);
    m_pButtonGroup->addButton(ui->pb_Disconnect_5, 25);
    m_pButtonGroup->addButton(ui->pb_Disconnect_6, 26);
    m_pButtonGroup->addButton(ui->pb_Disconnect_7, 27);
    m_pButtonGroup->addButton(ui->pb_Disconnect_8, 28);
    m_pButtonGroup->addButton(ui->pb_Disconnect_9, 29);
    m_pButtonGroup->addButton(ui->pb_Disconnect_10, 30);
    m_pButtonGroup->addButton(ui->pb_Disconnect_11, 31);
    m_pButtonGroup->addButton(ui->pb_Disconnect_12, 32);
    m_pButtonGroup->addButton(ui->pb_Disconnect_13, 33);

    m_pButtonGroup->addButton(ui->pb_Scan_1, 41);
    m_pButtonGroup->addButton(ui->pb_Scan_2, 42);
    m_pButtonGroup->addButton(ui->pb_Scan_3, 43);
    m_pButtonGroup->addButton(ui->pb_Scan_4, 44);
    m_pButtonGroup->addButton(ui->pb_Scan_5, 45);
    m_pButtonGroup->addButton(ui->pb_Scan_6, 46);
    m_pButtonGroup->addButton(ui->pb_Scan_7, 47);
    m_pButtonGroup->addButton(ui->pb_Scan_8, 48);
    m_pButtonGroup->addButton(ui->pb_Scan_9, 49);
    m_pButtonGroup->addButton(ui->pb_Scan_10, 50);
    m_pButtonGroup->addButton(ui->pb_Scan_11, 51);
    m_pButtonGroup->addButton(ui->pb_Scan_12, 52);
    m_pButtonGroup->addButton(ui->pb_Scan_13, 53);

    connect(m_pButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_ButtonClick(int)));

    m_listState.append(NULL);
    m_listState.append(ui->lb_State_1);
    m_listState.append(ui->lb_State_2);
    m_listState.append(ui->lb_State_3);
    m_listState.append(ui->lb_State_4);
    m_listState.append(ui->lb_State_5);
    m_listState.append(ui->lb_State_6);
    m_listState.append(ui->lb_State_7);
    m_listState.append(ui->lb_State_8);
    m_listState.append(ui->lb_State_9);
    m_listState.append(ui->lb_State_10);
    m_listState.append(ui->lb_State_11);
    m_listState.append(ui->lb_State_12);
    m_listState.append(ui->lb_State_13);

    for(int i = 1; i < m_listState.size(); i ++){
        m_listState.at(i)->setText(QStringLiteral("¶Ï¿ª"));
        m_listState.at(i)->setStyleSheet(Disconnect_Style);
    }

    m_listCode.append(NULL);
    m_listCode.append(ui->le_CodeData_1);
    m_listCode.append(ui->le_CodeData_2);
    m_listCode.append(ui->le_CodeData_3);
    m_listCode.append(ui->le_CodeData_4);
    m_listCode.append(ui->le_CodeData_5);
    m_listCode.append(ui->le_CodeData_6);
    m_listCode.append(ui->le_CodeData_7);
    m_listCode.append(ui->le_CodeData_8);
    m_listCode.append(ui->le_CodeData_9);
    m_listCode.append(ui->le_CodeData_10);
    m_listCode.append(ui->le_CodeData_11);
    m_listCode.append(ui->le_CodeData_12);
    m_listCode.append(ui->le_CodeData_13);

    DataFile o_DataFile;
    o_DataFile.GetScanerIPList(m_listScanerIP);
}

void ScanerDebugView::ErrorBox(QString str_Info)
{
    qWarning()<<"ScanerDebugView "<<str_Info;

    QMessageBox::warning(
                NULL,
                QStringLiteral("¾¯¸æ"),
                str_Info
                );
}

void ScanerDebugView::slot_ButtonClick(int n_ID)
{
    if(n_ID >= 1 && n_ID <= 13){
        if(!AddScaner(n_ID))
            return;
    }
    else if(n_ID >= 21 && n_ID <= 33){
        if(!RemoveSacner(n_ID - 20))
            return;
    }
    else if(n_ID >= 41 && n_ID <= 53){
        if(!RequsetCode(n_ID - 40))
            return;
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

void ScanerDebugView::slot_ScanCodeData(int n_ID,
                                        bool b_ScanState,
                                        QString str_Code)
{
    m_listCode.at(n_ID)->setText(str_Code);

    if(!b_ScanState){
        m_listState.at(n_ID)->setText(QStringLiteral("É¨ÂëÊ§°Ü"));
        m_listState.at(n_ID)->setStyleSheet(StateWarning);
    }
}

void ScanerDebugView::slot_ScanerConnectState(int n_ID,
                                              bool b_ConnectState)
{
    if(b_ConnectState){
        m_listState.at(n_ID)->setText(QStringLiteral("Á¬½Ó"));
        m_listState.at(n_ID)->setStyleSheet(Connect_Style);
    }
    else{
        m_listState.at(n_ID)->setText(QStringLiteral("¶Ï¿ª"));
        m_listState.at(n_ID)->setStyleSheet(Disconnect_Style);
    }
}
