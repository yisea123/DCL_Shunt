#include "plcdebugview.h"
#include "ui_plcdebugview.h"

#include <QMessageBox>
#include <QDebug>


PLCDebugView::PLCDebugView(SiemensPLCDevice *&p_SiemensPLCDevice, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PLCDebugView)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(QStringLiteral("PLC调试"));
    this->setFixedSize(this->size());

    m_pWarningtButtonGroup = NULL;
    m_pResetButtonGroup = NULL;
    m_pCmdAButtonGroup = NULL;
    m_pCmdBButtonGroup = NULL;
    m_pCmdCButtonGroup = NULL;
    m_pCmdDButtonGroup = NULL;

    InitView();

    m_pSiemensPLCDevice = p_SiemensPLCDevice;

    connect(m_pSiemensPLCDevice, &SiemensPLCDevice::sig_ReadData,
            this, &PLCDebugView::slot_ReadPLCData);

    connect(m_pSiemensPLCDevice, &SiemensPLCDevice::sig_ConnectState,
            this, &PLCDebugView::slot_ConnectState);

    m_pQTimer = new QTimer;
    connect(m_pQTimer, &QTimer::timeout,
            this, &PLCDebugView::slot_TimeOut);
    m_pQTimer->start(1000);

    if(m_pSiemensPLCDevice->GetConnectState()){
        ui->lb_State->setText(QStringLiteral("连接"));
        ui->lb_State->setStyleSheet(Connect_Style);
    }
    else{
        ui->lb_State->setText(QStringLiteral("断开"));
        ui->lb_State->setStyleSheet(Disconnect_Style);
    }

}

PLCDebugView::~PLCDebugView()
{
    if(m_pQTimer != NULL){
        if(m_pQTimer->isActive())
            m_pQTimer->stop();
        delete m_pQTimer;
        m_pQTimer = NULL;
    }

    delete ui;
}

bool PLCDebugView::SendCmd(int n_Station, PC_CMD cmd)
{
    if(m_pSiemensPLCDevice == NULL)
        return false;

    if(!m_pSiemensPLCDevice->SendCmd(n_Station, cmd)){
        ErrorBox(QStringLiteral("发送命令失败"));
        return false;
    }

    return true;
}

void PLCDebugView::ResetWriteButtonUI()
{
    unsigned char ucLastData[48] = {};
    memset(ucLastData, 0, 48);
    memcpy(ucLastData, &m_LastPCData, 26);

    for(int i = 0; i < 26/2; i++) {
        if(!m_mapPB.contains(PC_CMD(ucLastData[i*2])))
            continue;

        QPushButton *p_QPushButton = m_mapPB.value(PC_CMD(ucLastData[i*2])).value(i + 1);
        if(p_QPushButton != NULL){
            p_QPushButton->setStyleSheet(NoClick_Style);
        }
    }
}

void PLCDebugView::ResetReadLabelUI()
{
    unsigned char ucLastData[48] = {};
    memset(ucLastData, 0, 48);
    memcpy(ucLastData, &m_LastPLCData, 26);

    for(int i = 0; i < 26/2; i++) {
        if(!m_mapLB.contains(PLC_CMD(ucLastData[i*2])))
            continue;

        QLabel *p_QLabel = m_mapLB.value(PLC_CMD(ucLastData[i*2])).value(i + 1);
        if(p_QLabel != NULL){
            p_QLabel->setStyleSheet(NoClick_Style);
        }
    }
}

void PLCDebugView::InitView()
{
    QMap<int, QPushButton * > map_pb_Error;
    map_pb_Error.insert(1, ui->pb_Error_1);
    map_pb_Error.insert(2, ui->pb_Error_2);
    map_pb_Error.insert(3, ui->pb_Error_3);
    map_pb_Error.insert(4, ui->pb_Error_4);
    map_pb_Error.insert(5, ui->pb_Error_5);
    map_pb_Error.insert(6, ui->pb_Error_6);
    map_pb_Error.insert(7, ui->pb_Error_7);
    map_pb_Error.insert(8, ui->pb_Error_8);
    map_pb_Error.insert(9, ui->pb_Error_9);
    map_pb_Error.insert(10, ui->pb_Error_10);
    map_pb_Error.insert(11, ui->pb_Error_11);
    map_pb_Error.insert(12, ui->pb_Error_12);
    map_pb_Error.insert(13, ui->pb_Error_13);

    QMap<int, QPushButton * > map_pb_Reset;
    map_pb_Reset.insert(1, ui->pb_Reset_1);
    map_pb_Reset.insert(2, ui->pb_Reset_2);
    map_pb_Reset.insert(3, ui->pb_Reset_3);
    map_pb_Reset.insert(4, ui->pb_Reset_4);
    map_pb_Reset.insert(5, ui->pb_Reset_5);
    map_pb_Reset.insert(6, ui->pb_Reset_6);
    map_pb_Reset.insert(7, ui->pb_Reset_7);
    map_pb_Reset.insert(8, ui->pb_Reset_8);
    map_pb_Reset.insert(9, ui->pb_Reset_9);
    map_pb_Reset.insert(10, ui->pb_Reset_10);
    map_pb_Reset.insert(11, ui->pb_Reset_11);
    map_pb_Reset.insert(12, ui->pb_Reset_12);
    map_pb_Reset.insert(13, ui->pb_Reset_13);

    QMap<int, QPushButton * > map_pb_EMPTYBOX;
    map_pb_EMPTYBOX.insert(1, ui->pb_CmdA_1);
    map_pb_EMPTYBOX.insert(2, NULL);
    map_pb_EMPTYBOX.insert(3, NULL);
    map_pb_EMPTYBOX.insert(4, NULL);
    map_pb_EMPTYBOX.insert(5, NULL);
    map_pb_EMPTYBOX.insert(6, NULL);
    map_pb_EMPTYBOX.insert(7, NULL);
    map_pb_EMPTYBOX.insert(8, NULL);
    map_pb_EMPTYBOX.insert(9, NULL);
    map_pb_EMPTYBOX.insert(10, NULL);
    map_pb_EMPTYBOX.insert(11, NULL);
    map_pb_EMPTYBOX.insert(12, NULL);
    map_pb_EMPTYBOX.insert(13, NULL);

    QMap<int, QPushButton * > map_pb_FULLBOX;
    map_pb_FULLBOX.insert(1, ui->pb_CmdB_1);
    map_pb_FULLBOX.insert(2, NULL);
    map_pb_FULLBOX.insert(3, NULL);
    map_pb_FULLBOX.insert(4, NULL);
    map_pb_FULLBOX.insert(5, NULL);
    map_pb_FULLBOX.insert(6, NULL);
    map_pb_FULLBOX.insert(7, NULL);
    map_pb_FULLBOX.insert(8, NULL);
    map_pb_FULLBOX.insert(9, NULL);
    map_pb_FULLBOX.insert(10, NULL);
    map_pb_FULLBOX.insert(11, NULL);
    map_pb_FULLBOX.insert(12, NULL);
    map_pb_FULLBOX.insert(13, NULL);

    QMap<int, QPushButton * > map_pb_HIGHWARHOUSE_PLC;
    map_pb_HIGHWARHOUSE_PLC.insert(1, NULL);
    map_pb_HIGHWARHOUSE_PLC.insert(2, ui->pb_CmdA_2);
    map_pb_HIGHWARHOUSE_PLC.insert(3, NULL);
    map_pb_HIGHWARHOUSE_PLC.insert(4, NULL);
    map_pb_HIGHWARHOUSE_PLC.insert(5, NULL);
    map_pb_HIGHWARHOUSE_PLC.insert(6, NULL);
    map_pb_HIGHWARHOUSE_PLC.insert(7, NULL);
    map_pb_HIGHWARHOUSE_PLC.insert(8, NULL);
    map_pb_HIGHWARHOUSE_PLC.insert(9, NULL);
    map_pb_HIGHWARHOUSE_PLC.insert(10, NULL);
    map_pb_HIGHWARHOUSE_PLC.insert(11, NULL);
    map_pb_HIGHWARHOUSE_PLC.insert(12, NULL);
    map_pb_HIGHWARHOUSE_PLC.insert(13, NULL);

    QMap<int, QPushButton * > map_pb_IN_NEGATIVEDC_DC_PLC;
    map_pb_IN_NEGATIVEDC_DC_PLC.insert(1, NULL);
    map_pb_IN_NEGATIVEDC_DC_PLC.insert(2, ui->pb_CmdB_2);
    map_pb_IN_NEGATIVEDC_DC_PLC.insert(3, NULL);
    map_pb_IN_NEGATIVEDC_DC_PLC.insert(4, NULL);
    map_pb_IN_NEGATIVEDC_DC_PLC.insert(5, NULL);
    map_pb_IN_NEGATIVEDC_DC_PLC.insert(6, NULL);
    map_pb_IN_NEGATIVEDC_DC_PLC.insert(7, NULL);
    map_pb_IN_NEGATIVEDC_DC_PLC.insert(8, NULL);
    map_pb_IN_NEGATIVEDC_DC_PLC.insert(9, NULL);
    map_pb_IN_NEGATIVEDC_DC_PLC.insert(10, NULL);
    map_pb_IN_NEGATIVEDC_DC_PLC.insert(11, NULL);
    map_pb_IN_NEGATIVEDC_DC_PLC.insert(12, NULL);
    map_pb_IN_NEGATIVEDC_DC_PLC.insert(13, NULL);

    QMap<int, QPushButton * > map_pb_IN_LINE_NOBAD;
    map_pb_IN_LINE_NOBAD.insert(1, NULL);
    map_pb_IN_LINE_NOBAD.insert(2, NULL);
    map_pb_IN_LINE_NOBAD.insert(3, ui->pb_CmdA_3);
    map_pb_IN_LINE_NOBAD.insert(4, NULL);
    map_pb_IN_LINE_NOBAD.insert(5, NULL);
    map_pb_IN_LINE_NOBAD.insert(6, ui->pb_CmdA_6);
    map_pb_IN_LINE_NOBAD.insert(7, NULL);
    map_pb_IN_LINE_NOBAD.insert(8, NULL);
    map_pb_IN_LINE_NOBAD.insert(9, ui->pb_CmdA_9);
    map_pb_IN_LINE_NOBAD.insert(10, ui->pb_CmdA_10);
    map_pb_IN_LINE_NOBAD.insert(11, NULL);
    map_pb_IN_LINE_NOBAD.insert(12, NULL);
    map_pb_IN_LINE_NOBAD.insert(13, NULL);

    QMap<int, QPushButton * > map_pb_IN_SORTBAD;
    map_pb_IN_SORTBAD.insert(1, NULL);
    map_pb_IN_SORTBAD.insert(2, NULL);
    map_pb_IN_SORTBAD.insert(3, ui->pb_CmdB_3);
    map_pb_IN_SORTBAD.insert(4, NULL);
    map_pb_IN_SORTBAD.insert(5, NULL);
    map_pb_IN_SORTBAD.insert(6, ui->pb_CmdB_6);
    map_pb_IN_SORTBAD.insert(7, NULL);
    map_pb_IN_SORTBAD.insert(8, NULL);
    map_pb_IN_SORTBAD.insert(9, ui->pb_CmdB_9);
    map_pb_IN_SORTBAD.insert(10, ui->pb_CmdB_10);
    map_pb_IN_SORTBAD.insert(11, NULL);
    map_pb_IN_SORTBAD.insert(12, NULL);
    map_pb_IN_SORTBAD.insert(13, NULL);

    QMap<int, QPushButton * > map_pb_IN_LINE_COMPLETESORT;
    map_pb_IN_LINE_COMPLETESORT.insert(1, NULL);
    map_pb_IN_LINE_COMPLETESORT.insert(2, NULL);
    map_pb_IN_LINE_COMPLETESORT.insert(3, NULL);
    map_pb_IN_LINE_COMPLETESORT.insert(4, ui->pb_CmdA_4);
    map_pb_IN_LINE_COMPLETESORT.insert(5, NULL);
    map_pb_IN_LINE_COMPLETESORT.insert(6, NULL);
    map_pb_IN_LINE_COMPLETESORT.insert(7, ui->pb_CmdA_7);
    map_pb_IN_LINE_COMPLETESORT.insert(8, NULL);
    map_pb_IN_LINE_COMPLETESORT.insert(9, NULL);
    map_pb_IN_LINE_COMPLETESORT.insert(10, NULL);
    map_pb_IN_LINE_COMPLETESORT.insert(11, ui->pb_CmdA_11);
    map_pb_IN_LINE_COMPLETESORT.insert(12, NULL);
    map_pb_IN_LINE_COMPLETESORT.insert(13, NULL);

    QMap<int, QPushButton * > map_pb_IN_LINE_LOADFULLBAD;
    map_pb_IN_LINE_LOADFULLBAD.insert(1, NULL);
    map_pb_IN_LINE_LOADFULLBAD.insert(2, NULL);
    map_pb_IN_LINE_LOADFULLBAD.insert(3, NULL);
    map_pb_IN_LINE_LOADFULLBAD.insert(4, NULL);
    map_pb_IN_LINE_LOADFULLBAD.insert(5, ui->pb_CmdA_5);
    map_pb_IN_LINE_LOADFULLBAD.insert(6, NULL);
    map_pb_IN_LINE_LOADFULLBAD.insert(7, NULL);
    map_pb_IN_LINE_LOADFULLBAD.insert(8, ui->pb_CmdA_8);
    map_pb_IN_LINE_LOADFULLBAD.insert(9, NULL);
    map_pb_IN_LINE_LOADFULLBAD.insert(10, NULL);
    map_pb_IN_LINE_LOADFULLBAD.insert(11, NULL);
    map_pb_IN_LINE_LOADFULLBAD.insert(12, ui->pb_CmdA_12);
    map_pb_IN_LINE_LOADFULLBAD.insert(13, NULL);

    QMap<int, QPushButton * > map_pb_IN_EMPTYBOX_302;
    map_pb_IN_EMPTYBOX_302.insert(1, NULL);
    map_pb_IN_EMPTYBOX_302.insert(2, NULL);
    map_pb_IN_EMPTYBOX_302.insert(3, NULL);
    map_pb_IN_EMPTYBOX_302.insert(4, NULL);
    map_pb_IN_EMPTYBOX_302.insert(5, NULL);
    map_pb_IN_EMPTYBOX_302.insert(6, NULL);
    map_pb_IN_EMPTYBOX_302.insert(7, NULL);
    map_pb_IN_EMPTYBOX_302.insert(8, NULL);
    map_pb_IN_EMPTYBOX_302.insert(9, NULL);
    map_pb_IN_EMPTYBOX_302.insert(10, NULL);
    map_pb_IN_EMPTYBOX_302.insert(11, NULL);
    map_pb_IN_EMPTYBOX_302.insert(12, NULL);
    map_pb_IN_EMPTYBOX_302.insert(13, ui->pb_CmdB_13);

    QMap<int, QPushButton * > map_pb_IN_EMPTYBOX_127;
    map_pb_IN_EMPTYBOX_127.insert(1, NULL);
    map_pb_IN_EMPTYBOX_127.insert(2, NULL);
    map_pb_IN_EMPTYBOX_127.insert(3, NULL);
    map_pb_IN_EMPTYBOX_127.insert(4, NULL);
    map_pb_IN_EMPTYBOX_127.insert(5, NULL);
    map_pb_IN_EMPTYBOX_127.insert(6, NULL);
    map_pb_IN_EMPTYBOX_127.insert(7, NULL);
    map_pb_IN_EMPTYBOX_127.insert(8, NULL);
    map_pb_IN_EMPTYBOX_127.insert(9, NULL);
    map_pb_IN_EMPTYBOX_127.insert(10, NULL);
    map_pb_IN_EMPTYBOX_127.insert(11, NULL);
    map_pb_IN_EMPTYBOX_127.insert(12, NULL);
    map_pb_IN_EMPTYBOX_127.insert(13, ui->pb_CmdA_13);

    QMap<int, QPushButton * > map_pb_IN_FULLBOX_127;
    map_pb_IN_FULLBOX_127.insert(1, NULL);
    map_pb_IN_FULLBOX_127.insert(2, NULL);
    map_pb_IN_FULLBOX_127.insert(3, NULL);
    map_pb_IN_FULLBOX_127.insert(4, NULL);
    map_pb_IN_FULLBOX_127.insert(5, NULL);
    map_pb_IN_FULLBOX_127.insert(6, NULL);
    map_pb_IN_FULLBOX_127.insert(7, NULL);
    map_pb_IN_FULLBOX_127.insert(8, NULL);
    map_pb_IN_FULLBOX_127.insert(9, NULL);
    map_pb_IN_FULLBOX_127.insert(10, NULL);
    map_pb_IN_FULLBOX_127.insert(11, NULL);
    map_pb_IN_FULLBOX_127.insert(12, NULL);
    map_pb_IN_FULLBOX_127.insert(13, ui->pb_CmdC_13);

    QMap<int, QPushButton * > map_pb_IN_FULLBOX_302;
    map_pb_IN_FULLBOX_302.insert(1, NULL);
    map_pb_IN_FULLBOX_302.insert(2, NULL);
    map_pb_IN_FULLBOX_302.insert(3, NULL);
    map_pb_IN_FULLBOX_302.insert(4, NULL);
    map_pb_IN_FULLBOX_302.insert(5, NULL);
    map_pb_IN_FULLBOX_302.insert(6, NULL);
    map_pb_IN_FULLBOX_302.insert(7, NULL);
    map_pb_IN_FULLBOX_302.insert(8, NULL);
    map_pb_IN_FULLBOX_302.insert(9, NULL);
    map_pb_IN_FULLBOX_302.insert(10, NULL);
    map_pb_IN_FULLBOX_302.insert(11, NULL);
    map_pb_IN_FULLBOX_302.insert(12, NULL);
    map_pb_IN_FULLBOX_302.insert(13, ui->pb_CmdD_13);

    m_mapPB.insert(PC_WARNING, map_pb_Error);
    m_mapPB.insert(RESTORATION, map_pb_Reset);
    m_mapPB.insert(IN_EMPTYBOX, map_pb_EMPTYBOX);
    m_mapPB.insert(IN_FULLBOX, map_pb_FULLBOX);
    m_mapPB.insert(IN_HIGHWARHOUSE_PLC, map_pb_HIGHWARHOUSE_PLC);
    m_mapPB.insert(IN_NEGATIVEDC_DC_PLC, map_pb_IN_NEGATIVEDC_DC_PLC);
    m_mapPB.insert(IN_LINE_NOBAD, map_pb_IN_LINE_NOBAD);
    m_mapPB.insert(IN_SORTBAD, map_pb_IN_SORTBAD);
    m_mapPB.insert(IN_LINE_COMPLETESORT, map_pb_IN_LINE_COMPLETESORT);
    m_mapPB.insert(IN_LINE_LOADFULLBAD, map_pb_IN_LINE_LOADFULLBAD);
    m_mapPB.insert(IN_EMPTYBOX_127, map_pb_IN_EMPTYBOX_127);
    m_mapPB.insert(IN_EMPTYBOX_302, map_pb_IN_EMPTYBOX_302);
    m_mapPB.insert(IN_FULLBOX_127, map_pb_IN_FULLBOX_127);
    m_mapPB.insert(IN_FULLBOX_302, map_pb_IN_FULLBOX_302);

    QMap<int, QLabel * > map_lb_Error;
    map_lb_Error.insert(1, ui->lb_Error_1);
    map_lb_Error.insert(2, ui->lb_Error_2);
    map_lb_Error.insert(3, ui->lb_Error_3);
    map_lb_Error.insert(4, ui->lb_Error_4);
    map_lb_Error.insert(5, ui->lb_Error_5);
    map_lb_Error.insert(6, ui->lb_Error_6);
    map_lb_Error.insert(7, ui->lb_Error_7);
    map_lb_Error.insert(8, ui->lb_Error_8);
    map_lb_Error.insert(9, ui->lb_Error_9);
    map_lb_Error.insert(10, ui->lb_Error_10);
    map_lb_Error.insert(11, ui->lb_Error_11);
    map_lb_Error.insert(12, ui->lb_Error_12);
    map_lb_Error.insert(13, ui->lb_Error_13);

    QMap<int, QLabel * > map_lb_Wait;
    map_lb_Wait.insert(1, ui->lb_Wait_1);
    map_lb_Wait.insert(2, ui->lb_Wait_2);
    map_lb_Wait.insert(3, ui->lb_Wait_3);
    map_lb_Wait.insert(4, ui->lb_Wait_4);
    map_lb_Wait.insert(5, ui->lb_Wait_5);
    map_lb_Wait.insert(6, ui->lb_Wait_6);
    map_lb_Wait.insert(7, ui->lb_Wait_7);
    map_lb_Wait.insert(8, ui->lb_Wait_8);
    map_lb_Wait.insert(9, ui->lb_Wait_9);
    map_lb_Wait.insert(10, ui->lb_Wait_10);
    map_lb_Wait.insert(11, ui->lb_Wait_11);
    map_lb_Wait.insert(12, ui->lb_Wait_12);
    map_lb_Wait.insert(13, ui->lb_Wait_13);

    QMap<int, QLabel * > map_lb_RequestCode;
    map_lb_RequestCode.insert(1, ui->lb_RequestCode_1);
    map_lb_RequestCode.insert(2, ui->lb_RequestCode_2);
    map_lb_RequestCode.insert(3, ui->lb_RequestCode_3);
    map_lb_RequestCode.insert(4, ui->lb_RequestCode_4);
    map_lb_RequestCode.insert(5, ui->lb_RequestCode_5);
    map_lb_RequestCode.insert(6, ui->lb_RequestCode_6);
    map_lb_RequestCode.insert(7, ui->lb_RequestCode_7);
    map_lb_RequestCode.insert(8, ui->lb_RequestCode_8);
    map_lb_RequestCode.insert(9, ui->lb_RequestCode_9);
    map_lb_RequestCode.insert(10, ui->lb_RequestCode_10);
    map_lb_RequestCode.insert(11, ui->lb_RequestCode_11);
    map_lb_RequestCode.insert(12, ui->lb_RequestCode_12);
    map_lb_RequestCode.insert(13, ui->lb_RequestCode_13);

    m_mapLB.insert(PLC_WARNING, map_lb_Error);
    m_mapLB.insert(NOREQUESTSCAN, map_lb_Wait);
    m_mapLB.insert(REQUESTSCAN, map_lb_RequestCode);

    m_pWarningtButtonGroup = new QButtonGroup(this);
    m_pResetButtonGroup = new QButtonGroup(this);
    m_pCmdAButtonGroup = new QButtonGroup(this);
    m_pCmdBButtonGroup = new QButtonGroup(this);
    m_pCmdCButtonGroup = new QButtonGroup(this);
    m_pCmdDButtonGroup = new QButtonGroup(this);

    m_pWarningtButtonGroup->addButton(ui->pb_Error_1, 1);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_2, 2);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_3, 3);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_4, 4);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_5, 5);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_6, 6);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_7, 7);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_8, 8);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_9, 9);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_10, 10);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_11, 11);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_12, 12);
    m_pWarningtButtonGroup->addButton(ui->pb_Error_13, 13);

    m_pResetButtonGroup->addButton(ui->pb_Reset_1, 1);
    m_pResetButtonGroup->addButton(ui->pb_Reset_2, 2);
    m_pResetButtonGroup->addButton(ui->pb_Reset_3, 3);
    m_pResetButtonGroup->addButton(ui->pb_Reset_4, 4);
    m_pResetButtonGroup->addButton(ui->pb_Reset_5, 5);
    m_pResetButtonGroup->addButton(ui->pb_Reset_6, 6);
    m_pResetButtonGroup->addButton(ui->pb_Reset_7, 7);
    m_pResetButtonGroup->addButton(ui->pb_Reset_8, 8);
    m_pResetButtonGroup->addButton(ui->pb_Reset_9, 9);
    m_pResetButtonGroup->addButton(ui->pb_Reset_10, 10);
    m_pResetButtonGroup->addButton(ui->pb_Reset_11, 11);
    m_pResetButtonGroup->addButton(ui->pb_Reset_12, 12);
    m_pResetButtonGroup->addButton(ui->pb_Reset_13, 13);

    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_1, 1);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_2, 2);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_3, 3);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_4, 4);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_5, 5);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_6, 6);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_7, 7);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_8, 8);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_9, 9);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_10, 10);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_11, 11);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_12, 12);
    m_pCmdAButtonGroup->addButton(ui->pb_CmdA_13, 13);

    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_1 ,1);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_2 ,2);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_3 ,3);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_4 ,4);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_5 ,5);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_6 ,6);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_7 ,7);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_8 ,8);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_9 ,9);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_10 ,10);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_11 ,11);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_12 ,12);
    m_pCmdBButtonGroup->addButton(ui->pb_CmdB_13 ,13);

    m_pCmdCButtonGroup->addButton(ui->pb_CmdC_1 ,1);
    m_pCmdCButtonGroup->addButton(ui->pb_CmdC_13 ,13);

    m_pCmdDButtonGroup->addButton(ui->pb_CmdD_1 ,1);
    m_pCmdDButtonGroup->addButton(ui->pb_CmdD_1 ,13);

    connect(m_pWarningtButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_WarningtButtonClick(int)));

    connect(m_pResetButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_ResetButtonClick(int)));

    connect(m_pCmdAButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_CmdAButtonClick(int)));

    connect(m_pCmdBButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_CmdBButtonClick(int)));

    connect(m_pCmdCButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_CmdCButtonClick(int)));

    connect(m_pCmdDButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_CmdDButtonClick(int)));
}

void PLCDebugView::ErrorBox(QString str_Info)
{
    qWarning()<<"PLCDebugView "<<str_Info;

    QMessageBox::warning(
                NULL,
                QStringLiteral("警告"),
                str_Info
                );
}

void PLCDebugView::slot_WarningtButtonClick(int n_ID)
{
    SendCmd(n_ID, PC_WARNING);
}

void PLCDebugView::slot_ResetButtonClick(int n_ID)
{
    SendCmd(n_ID, RESTORATION);
}

void PLCDebugView::slot_CmdAButtonClick(int n_ID)
{
    switch(n_ID){
    case 1:
        SendCmd(n_ID, IN_EMPTYBOX_127);
        break;
    case 2:
        SendCmd(n_ID, IN_HIGHWARHOUSE_PLC);
        break;
    case 3:
        SendCmd(n_ID, IN_LINE_NOBAD);
        break;
    case 4:
        SendCmd(n_ID, IN_LINE_COMPLETESORT);
        break;
    case 5:
        SendCmd(n_ID, IN_LINE_LOADFULLBAD);
        break;
    case 6:
        SendCmd(n_ID, IN_LINE_NOBAD);
        break;
    case 7:
        SendCmd(n_ID, IN_LINE_COMPLETESORT);
        break;
    case 8:
        SendCmd(n_ID, IN_LINE_LOADFULLBAD);
        break;
    case 9:
        SendCmd(n_ID, IN_LINE_NOBAD);
        break;
    case 10:
        SendCmd(n_ID, IN_LINE_NOBAD);
        break;
    case 11:
        SendCmd(n_ID, IN_LINE_COMPLETESORT);
        break;
    case 12:
        SendCmd(n_ID, IN_LINE_LOADFULLBAD);
        break;
    case 13:
        SendCmd(n_ID, IN_EMPTYBOX_127);
        break;
    default:
        break;
    }
}

void PLCDebugView::slot_CmdBButtonClick(int n_ID)
{
    switch(n_ID){
    case 1:
        SendCmd(n_ID, IN_EMPTYBOX_302);
        break;
    case 2:
        SendCmd(n_ID, IN_NEGATIVEDC_DC_PLC);
        break;
    case 3:
        SendCmd(n_ID, IN_SORTBAD);
        break;
    case 6:
        SendCmd(n_ID, IN_SORTBAD);
        break;
    case 9:
        SendCmd(n_ID, IN_SORTBAD);
        break;
    case 10:
        SendCmd(n_ID, IN_SORTBAD);
        break;
    case 13:
        SendCmd(n_ID, IN_EMPTYBOX_302);
        break;
    default:
        break;
    }
}

void PLCDebugView::slot_CmdCButtonClick(int n_ID)
{
    switch(n_ID){
    case 1:
        SendCmd(n_ID, IN_FULLBOX_127);
        break;
    case 13:
        SendCmd(n_ID, IN_FULLBOX_127);
        break;
    default:
        break;
    }
}

void PLCDebugView::slot_CmdDButtonClick(int n_ID)
{
    switch(n_ID){
    case 1:
        SendCmd(n_ID, IN_FULLBOX_302);
        break;
    case 13:
        SendCmd(n_ID, IN_FULLBOX_302);
        break;
    default:
        break;
    }
}

void PLCDebugView::slot_TimeOut()
{
    PC_DATAAREA PCData;
    m_pSiemensPLCDevice->GetWritePCData(PCData);

    unsigned char ucLastData[48] = {};
    memset(ucLastData, 0, 48);
    memcpy(ucLastData, &PCData, 26);

    for(int i = 0; i < 26/2; i++) {
        if(!m_mapPB.contains(PC_CMD(ucLastData[i*2])))
            continue;

        QPushButton *p_QPushButton = m_mapPB.value(PC_CMD(ucLastData[i*2])).value(i + 1);
        if(p_QPushButton != NULL){
            p_QPushButton->setStyleSheet(NoClick_Style);
        }
    }

    m_LastPCData = PCData;
}

void PLCDebugView::slot_ReadPLCData(PLC_DATAAREA PLCData)
{
    unsigned char ucLastData[48] = {};
    memset(ucLastData, 0, 48);
    memcpy(ucLastData, &PLCData, 26);

    for(int i = 0; i < 26/2; i++) {
        if(!m_mapLB.contains(PLC_CMD(ucLastData[i*2])))
            continue;

        QLabel *p_QLabel = m_mapLB.value(PLC_CMD(ucLastData[i*2])).value(i + 1);
        if(p_QLabel != NULL){
            p_QLabel->setStyleSheet(NoClick_Style);
        }
    }

    m_LastPLCData = PLCData;
}

void PLCDebugView::slot_ConnectState(bool b_Connect)
{
    if(b_Connect){
        ui->lb_State->setText(QStringLiteral("连接"));
        ui->lb_State->setStyleSheet(Connect_Style);
    }
    else{
        ui->lb_State->setText(QStringLiteral("断开"));
        ui->lb_State->setStyleSheet(Disconnect_Style);
    }
}
