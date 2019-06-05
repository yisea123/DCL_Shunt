#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QApplication>
#include <QPoint>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_pMainControl = NULL;
    m_pButtonGroup = NULL;
    m_pLogView = NULL;
    m_nClickID = 0;

    this->setWindowTitle(QStringLiteral("分容后段_分拣分流"));
    this->setFixedSize(this->size());
    this->setGeometry(8,
                      30,
                      this->width(),
                      this->height());
    this->setMouseTracking(true);

    InitMW();

    SetMesType(CURRENTUSE);
}

MainWindow::~MainWindow()
{
    ExitMW();

    delete ui;
}

void MainWindow::InitMW()
{
    m_pButtonGroup = new QButtonGroup;

    m_pButtonGroup->addButton(ui->pb_Send_1, 1);
    m_pButtonGroup->addButton(ui->pb_Send_2, 2);
    m_pButtonGroup->addButton(ui->pb_Send_3, 3);
    m_pButtonGroup->addButton(ui->pb_Send_4, 4);

    connect(m_pButtonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(slot_ButtonClick(int)));

    m_pLogView = m_oLogViewInstanceGetter.GetInstance();
    m_pLogView->setGeometry(this->x() + this->width() + 15,
                            this->y(),
                            this->width()/3,
                            this->height());

    m_pLogView->InitView();
//    m_pLogView->show();
    m_pLogView->setFixedSize(m_pLogView->size());

    m_listScanerStateLaber.append(NULL);
    m_listScanerStateLaber.append(ui->lb_ScanerState_1);
    m_listScanerStateLaber.append(ui->lb_ScanerState_2);
    m_listScanerStateLaber.append(ui->lb_ScanerState_3);
    m_listScanerStateLaber.append(ui->lb_ScanerState_4);
    m_listScanerStateLaber.append(ui->lb_ScanerState_5);
    m_listScanerStateLaber.append(ui->lb_ScanerState_6);
    m_listScanerStateLaber.append(ui->lb_ScanerState_7);
    m_listScanerStateLaber.append(ui->lb_ScanerState_8);
    m_listScanerStateLaber.append(ui->lb_ScanerState_9);
    m_listScanerStateLaber.append(ui->lb_ScanerState_10);
    m_listScanerStateLaber.append(ui->lb_ScanerState_11);
    m_listScanerStateLaber.append(ui->lb_ScanerState_12);
    m_listScanerStateLaber.append(ui->lb_ScanerState_13);

    m_listScanerInfoLaber.append(NULL);
    m_listScanerInfoLaber.append(ui->lb_ScanCode_1);
    m_listScanerInfoLaber.append(ui->lb_ScanCode_2);
    m_listScanerInfoLaber.append(ui->lb_ScanCode_3);
    m_listScanerInfoLaber.append(ui->lb_ScanCode_4);
    m_listScanerInfoLaber.append(ui->lb_ScanCode_5);
    m_listScanerInfoLaber.append(ui->lb_ScanCode_6);
    m_listScanerInfoLaber.append(ui->lb_ScanCode_7);
    m_listScanerInfoLaber.append(ui->lb_ScanCode_8);
    m_listScanerInfoLaber.append(ui->lb_ScanCode_9);
    m_listScanerInfoLaber.append(ui->lb_ScanCode_10);
    m_listScanerInfoLaber.append(ui->lb_ScanCode_11);
    m_listScanerInfoLaber.append(ui->lb_ScanCode_12);
    m_listScanerInfoLaber.append(ui->lb_ScanCode_13);

    m_listPLCStateLaber.append(NULL);
    m_listPLCStateLaber.append(ui->lb_PLCState_1);
    m_listPLCStateLaber.append(ui->lb_PLCState_2);
    m_listPLCStateLaber.append(ui->lb_PLCState_3);
    m_listPLCStateLaber.append(ui->lb_PLCState_4);
    m_listPLCStateLaber.append(ui->lb_PLCState_5);
    m_listPLCStateLaber.append(ui->lb_PLCState_6);
    m_listPLCStateLaber.append(ui->lb_PLCState_7);
    m_listPLCStateLaber.append(ui->lb_PLCState_8);
    m_listPLCStateLaber.append(ui->lb_PLCState_9);
    m_listPLCStateLaber.append(ui->lb_PLCState_10);
    m_listPLCStateLaber.append(ui->lb_PLCState_11);
    m_listPLCStateLaber.append(ui->lb_PLCState_12);
    m_listPLCStateLaber.append(ui->lb_PLCState_13);

    m_listPLCInfoLaber.append(NULL);
    m_listPLCInfoLaber.append(ui->lb_PLCCmd_1);
    m_listPLCInfoLaber.append(ui->lb_PLCCmd_2);
    m_listPLCInfoLaber.append(ui->lb_PLCCmd_3);
    m_listPLCInfoLaber.append(ui->lb_PLCCmd_4);
    m_listPLCInfoLaber.append(ui->lb_PLCCmd_5);
    m_listPLCInfoLaber.append(ui->lb_PLCCmd_6);
    m_listPLCInfoLaber.append(ui->lb_PLCCmd_7);
    m_listPLCInfoLaber.append(ui->lb_PLCCmd_8);
    m_listPLCInfoLaber.append(ui->lb_PLCCmd_9);
    m_listPLCInfoLaber.append(ui->lb_PLCCmd_10);
    m_listPLCInfoLaber.append(ui->lb_PLCCmd_11);
    m_listPLCInfoLaber.append(ui->lb_PLCCmd_12);
    m_listPLCInfoLaber.append(ui->lb_PLCCmd_13);

    m_listMESStateLaber.append(NULL);
    m_listMESStateLaber.append(ui->lb_MESState_1);
    m_listMESStateLaber.append(ui->lb_MESState_2);
    m_listMESStateLaber.append(ui->lb_MESState_3);
    m_listMESStateLaber.append(ui->lb_MESState_4);
    m_listMESStateLaber.append(ui->lb_MESState_5);
    m_listMESStateLaber.append(ui->lb_MESState_6);
    m_listMESStateLaber.append(ui->lb_MESState_7);
    m_listMESStateLaber.append(ui->lb_MESState_8);
    m_listMESStateLaber.append(ui->lb_MESState_9);
    m_listMESStateLaber.append(ui->lb_MESState_10);
    m_listMESStateLaber.append(ui->lb_MESState_11);
    m_listMESStateLaber.append(ui->lb_MESState_12);
    m_listMESStateLaber.append(ui->lb_MESState_13);

    m_listMESInfoLaber.append(NULL);
    m_listMESInfoLaber.append(ui->lb_MesWork_1);
    m_listMESInfoLaber.append(ui->lb_MesWork_2);
    m_listMESInfoLaber.append(ui->lb_MesWork_3);
    m_listMESInfoLaber.append(ui->lb_MesWork_4);
    m_listMESInfoLaber.append(ui->lb_MesWork_5);
    m_listMESInfoLaber.append(ui->lb_MesWork_6);
    m_listMESInfoLaber.append(ui->lb_MesWork_7);
    m_listMESInfoLaber.append(ui->lb_MesWork_8);
    m_listMESInfoLaber.append(ui->lb_MesWork_9);
    m_listMESInfoLaber.append(ui->lb_MesWork_10);
    m_listMESInfoLaber.append(ui->lb_MesWork_11);
    m_listMESInfoLaber.append(ui->lb_MesWork_12);
    m_listMESInfoLaber.append(ui->lb_MesWork_13);

    m_listSportCardStateLaber.append(NULL);
    m_listSportCardStateLaber.append(ui->lb_RobotState_1);
    m_listSportCardStateLaber.append(ui->lb_RobotState_2);
    m_listSportCardStateLaber.append(ui->lb_RobotState_3);
    m_listSportCardStateLaber.append(ui->lb_RobotState_4);
    m_listSportCardStateLaber.append(ui->lb_RobotState_5);
    m_listSportCardStateLaber.append(ui->lb_RobotState_6);
    m_listSportCardStateLaber.append(ui->lb_RobotState_7);
    m_listSportCardStateLaber.append(ui->lb_RobotState_8);
    m_listSportCardStateLaber.append(ui->lb_RobotState_9);
    m_listSportCardStateLaber.append(ui->lb_RobotState_10);
    m_listSportCardStateLaber.append(ui->lb_RobotState_11);
    m_listSportCardStateLaber.append(ui->lb_RobotState_12);
    m_listSportCardStateLaber.append(ui->lb_RobotState_13);

    m_listSportCardInfoLaber.append(NULL);
    m_listSportCardInfoLaber.append(ui->lb_RobotCmd_1);
    m_listSportCardInfoLaber.append(ui->lb_RobotCmd_2);
    m_listSportCardInfoLaber.append(ui->lb_RobotCmd_3);
    m_listSportCardInfoLaber.append(ui->lb_RobotCmd_4);
    m_listSportCardInfoLaber.append(ui->lb_RobotCmd_5);
    m_listSportCardInfoLaber.append(ui->lb_RobotCmd_6);
    m_listSportCardInfoLaber.append(ui->lb_RobotCmd_7);
    m_listSportCardInfoLaber.append(ui->lb_RobotCmd_8);
    m_listSportCardInfoLaber.append(ui->lb_RobotCmd_9);
    m_listSportCardInfoLaber.append(ui->lb_RobotCmd_10);
    m_listSportCardInfoLaber.append(ui->lb_RobotCmd_11);
    m_listSportCardInfoLaber.append(ui->lb_RobotCmd_12);
    m_listSportCardInfoLaber.append(ui->lb_RobotCmd_13);

    m_pMainControl = new MainControl;

    connect(m_pMainControl, &MainControl::sig_ScanerConnectState,
            this, &MainWindow::slot_ScanerConnectState);

    connect(m_pMainControl, &MainControl::sig_SportCardConnectState,
            this, &MainWindow::slot_SportCardConnectState);

    connect(m_pMainControl, &MainControl::sig_PLCConnectState,
            this, &MainWindow::slot_PLCConnectState);

    connect(m_pMainControl, &MainControl::sig_Result_Heartbeat,
            this, &MainWindow::slot_Result_Heartbeat);

    connect(m_pMainControl, &MainControl::sig_AnormalWork,
            this, &MainWindow::slot_AnormalWork);

    connect(m_pMainControl, &MainControl::sig_ReadPLCData,
            this, &MainWindow::slot_ReadPLCData);

    connect(m_pMainControl, &MainControl::sig_SportCardCmdData,
            this, &MainWindow::slot_SportCardCmdData);

    connect(m_pMainControl, &MainControl::sig_ScanCodeData,
            this, &MainWindow::slot_ScanCodeData);

    connect(m_pMainControl, &MainControl::sig_MesWork,
            this, &MainWindow::slot_MesWork);

    connect(ui->a_TEST, &QAction::triggered,
            this, [=](){SetMesType(TEST);});

    connect(ui->a_DEBUG, &QAction::triggered,
            this, [=](){SetMesType(DEBUG);});

    connect(ui->a_RUNNING, &QAction::triggered,
            this, [=](){SetMesType(RUNNING);});

    connect(ui->a_CURRENTUSE, &QAction::triggered,
            this, [=](){SetMesType(CURRENTUSE);});

    connect(ui->a_HEARTBEAT_TEST, &QAction::triggered,
            this, [=](){SetMesType(HEARTBEAT_TEST);});

    connect(ui->a_HEARTBEAT_DEBUG, &QAction::triggered,
            this, [=](){SetMesType(HEARTBEAT_DEBUG);});

    connect(ui->a_HEARTBEAT_RUNNING, &QAction::triggered,
            this, [=](){SetMesType(HEARTBEAT_RUNNING);});

    connect(ui->a_HEARTBEAT_CURRENTUSE, &QAction::triggered,
            this, [=](){SetMesType(HEARTBEAT_CURRENTUSE);});

    connect(ui->a_ScanerDebug, &QAction::triggered,
            this, [=](){ScanerDebugUI();});

    connect(ui->a_PLCDebug, &QAction::triggered,
            this, [=](){PLCDebugUI();});

    connect(ui->a_MESDebug, &QAction::triggered,
            this, [=](){MESDebugUI();});

    connect(ui->a_SportCardDebug, &QAction::triggered,
            this, [=](){SportCardDebugUI();});

    connect(ui->a_DeviceIP, &QAction::triggered,
            this, [=](){DeviceIPUI();});

    connect(ui->a_CurrentLog, &QAction::triggered,
            this, [=](){CurrentLog();});

}

void MainWindow::ExitMW()
{
    if(m_pMainControl != NULL){
        delete  m_pMainControl;
        m_pMainControl = NULL;
    }
}

void MainWindow::ErrorBox(QString str_Info)
{
    qWarning()<<"MainWindow "<<str_Info;

    QMessageBox::warning(
                NULL,
                QStringLiteral("警告"),
                str_Info
                );
}

bool MainWindow::PrepareAutoWork()
{
    if(!m_pMainControl->PrepareAutoWork()){
        ErrorBox(QStringLiteral("初始化失败!"));
        return false;
    }
    return true;
}

bool MainWindow::StartAutoWork()
{
    if(!m_pMainControl->StartAutoWork()){
        ErrorBox(QStringLiteral("启动自动运行失败!"));
        return false;
    }
    return true;
}

bool MainWindow::PauseAutoWork()
{
    if(ui->pb_Send_3->text() == QStringLiteral("继续")){
        if(!m_pMainControl->ContinueAutoWork()){
            ErrorBox(QStringLiteral("继续自动运行失败!"));
            return false;
        }

        ui->pb_Send_3->setText(QStringLiteral("暂停"));

    }
    else if(ui->pb_Send_3->text() == QStringLiteral("暂停")){
        if(!m_pMainControl->PauseAutoWork()){
            ErrorBox(QStringLiteral("暂停自动运行失败!"));
            return false;
        }

        ui->pb_Send_3->setText(QStringLiteral("继续"));

    }

    return true;
}

bool MainWindow::StopAutoWork()
{
    if(!m_pMainControl->StopAutoWork()){
        ErrorBox(QStringLiteral("停止自动运行失败!"));
        return false;
    }

    return true;
}

void MainWindow::SetMesType(MESURLTYPE MESUrl_Type)
{
    if(!m_pMainControl->SetMesType(MESUrl_Type)){
        ErrorBox(QStringLiteral("设置MES服务器失败！"));
    }

    switch(MESUrl_Type) {
    case TEST:
        ui->a_CurrentMES->setText(QStringLiteral("当前：TEST"));
        break;
    case DEBUG:
        ui->a_CurrentMES->setText(QStringLiteral("当前：DEBUG"));
        break;
    case RUNNING:
        ui->a_CurrentMES->setText(QStringLiteral("当前：RUNNING"));
        break;
    case CURRENTUSE:
        ui->a_CurrentMES->setText(QStringLiteral("当前：CURRENTUSE"));
        break;
    case HEARTBEAT_TEST:
        ui->a_CurrentMES->setText(QStringLiteral("当前：HEARTBEAT_TEST"));
        break;
    case HEARTBEAT_DEBUG:
        ui->a_CurrentMES->setText(QStringLiteral("当前：HEARTBEAT_DEBUG"));
        break;
    case HEARTBEAT_RUNNING:
        ui->a_CurrentMES->setText(QStringLiteral("当前：HEARTBEAT_RUNNING"));
        break;
    case HEARTBEAT_CURRENTUSE:
        ui->a_CurrentMES->setText(QStringLiteral("当前：HEARTBEAT_CURRENTUSE"));
        break;
    default:
        break;
    }
}

void MainWindow::ScanerDebugUI()
{
    WORKSTATE WorkState = m_pMainControl->GetWorkState();

    if(WorkState == _WORKING && WorkState == _PAUSE){
        ErrorBox(QStringLiteral("正处于自动运行状态, 请停止运行"));
        return;
    }
    auto * p_ScanerManager = m_pMainControl->GetPoint_ScanerManager();
    auto p_View = new ScanerDebugView(p_ScanerManager);
    p_View->show();
}

void MainWindow::PLCDebugUI()
{
    WORKSTATE WorkState = m_pMainControl->GetWorkState();

    if(WorkState == _WORKING && WorkState == _PAUSE){
        ErrorBox(QStringLiteral("正处于自动运行状态, 请停止运行"));
        return;
    }
    auto * p_PLC = m_pMainControl->GetPoint_SiemensPLCDevice();
    auto p_View = new PLCDebugView(p_PLC);
    p_View->show();
}

void MainWindow::MESDebugUI()
{
    WORKSTATE WorkState = m_pMainControl->GetWorkState();

    if(WorkState == _WORKING && WorkState == _PAUSE){
        ErrorBox(QStringLiteral("正处于自动运行状态, 请停止运行"));
        return;
    }
    auto * p_MesManager = m_pMainControl->GetPoint_SDMesManager();
    auto p_View = new MesDebugView(p_MesManager);
    p_View->show();
}

void MainWindow::SportCardDebugUI()
{
    WORKSTATE WorkState = m_pMainControl->GetWorkState();

    if(WorkState == _WORKING && WorkState == _PAUSE){
        ErrorBox(QStringLiteral("正处于自动运行状态, 请停止运行"));
        return;
    }
    auto * p_SportCardManager = m_pMainControl->GetPoint_SportCardManager();
    auto p_View = new SportCardDebugView(p_SportCardManager);
    p_View->show();
}

void MainWindow::DeviceIPUI()
{
    WORKSTATE WorkState = m_pMainControl->GetWorkState();

    if(WorkState == _WORKING && WorkState == _PAUSE){
        ErrorBox(QStringLiteral("正处于自动运行状态, 请停止运行"));
        return;
    }

    auto p_View = new DeviceIP;
    p_View->show();
}

void MainWindow::CurrentLog()
{
    if(m_pLogView == NULL)
        return;

    if(m_pLogView->isHidden()){
        m_pLogView->setGeometry(this->x() + this->width() + 24,
                                this->y() + 30,
                                this->width()/3,
                                this->height());

        m_pLogView->show();
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    QApplication* app;
    app->quit();
}

//void MainWindow::mouseMoveEvent(QMouseEvent *event)
//{
//    if(m_pLogView == NULL)
//        return;

//    m_pLogView->setGeometry(this->x() + this->width() + 15,
//                            this->y(),
//                            this->width()/3,
//                            this->height());
//}

void MainWindow::slot_ButtonClick(int n_ID)
{
    if(n_ID == 1){
        if(!PrepareAutoWork())
            return;
    }
    else if(n_ID == 2){
        if(!StartAutoWork())
            return;
    }
    else if(n_ID == 3){
        if(!PauseAutoWork())
            return;
    }
    else if(n_ID == 4){
        if(!StopAutoWork())
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

void MainWindow::slot_ScanerConnectState(int n_ID, bool b_ConnectState)
{
    if(n_ID == 0){
        for(int i = 1; i < m_listScanerStateLaber.size(); i++){
            if(!b_ConnectState){
                m_listScanerStateLaber.at(i)->setText(QStringLiteral("断开"));
                m_listScanerStateLaber.at(i)->setStyleSheet(Disconnect_Style);
            }
        }
        return;
    }

    if(b_ConnectState){
        m_listScanerStateLaber.at(n_ID)->setText(QStringLiteral("连接"));
        m_listScanerStateLaber.at(n_ID)->setStyleSheet(Connect_Style);
    }
    else{
        m_listScanerStateLaber.at(n_ID)->setText(QStringLiteral("断开"));
        m_listScanerStateLaber.at(n_ID)->setStyleSheet(Disconnect_Style);
    }
}

void MainWindow::slot_SportCardConnectState(int n_ID, bool b_State)
{
    switch (n_ID) {
    case 1:{
        if(b_State){
            m_listSportCardStateLaber.at(4)->setText(QStringLiteral("连接"));
            m_listSportCardStateLaber.at(4)->setStyleSheet(Connect_Style);
            m_listSportCardStateLaber.at(5)->setText(QStringLiteral("连接"));
            m_listSportCardStateLaber.at(5)->setStyleSheet(Connect_Style);
        }
        else{
            m_listSportCardStateLaber.at(4)->setText(QStringLiteral("断开"));
            m_listSportCardStateLaber.at(4)->setStyleSheet(Disconnect_Style);
            m_listSportCardStateLaber.at(5)->setText(QStringLiteral("断开"));
            m_listSportCardStateLaber.at(5)->setStyleSheet(Disconnect_Style);
        }
        break;
    }
    case 2:{
        if(b_State){
            m_listSportCardStateLaber.at(7)->setText(QStringLiteral("连接"));
            m_listSportCardStateLaber.at(7)->setStyleSheet(Connect_Style);
            m_listSportCardStateLaber.at(8)->setText(QStringLiteral("连接"));
            m_listSportCardStateLaber.at(8)->setStyleSheet(Connect_Style);
        }
        else{
            m_listSportCardStateLaber.at(7)->setText(QStringLiteral("断开"));
            m_listSportCardStateLaber.at(7)->setStyleSheet(Disconnect_Style);
            m_listSportCardStateLaber.at(8)->setText(QStringLiteral("断开"));
            m_listSportCardStateLaber.at(8)->setStyleSheet(Disconnect_Style);
        }
        break;
    }
    case 3:{
        if(b_State){
            m_listSportCardStateLaber.at(11)->setText(QStringLiteral("连接"));
            m_listSportCardStateLaber.at(11)->setStyleSheet(Disconnect_Style);
            m_listSportCardStateLaber.at(12)->setText(QStringLiteral("连接"));
            m_listSportCardStateLaber.at(12)->setStyleSheet(Disconnect_Style);
        }
        else{
            m_listSportCardStateLaber.at(11)->setText(QStringLiteral("断开"));
            m_listSportCardStateLaber.at(11)->setStyleSheet(Disconnect_Style);
            m_listSportCardStateLaber.at(12)->setText(QStringLiteral("断开"));
            m_listSportCardStateLaber.at(12)->setStyleSheet(Disconnect_Style);
        }
        break;
    }
    default:
        break;
    }
}

void MainWindow::slot_PLCConnectState(bool b_Connect)
{
    for(int i = 1; i < m_listPLCStateLaber.size(); i++){
        if(b_Connect){
            m_listPLCStateLaber.at(i)->setText(QStringLiteral("连接"));
            m_listPLCStateLaber.at(i)->setStyleSheet(Disconnect_Style);
        }
        else{
            m_listPLCStateLaber.at(i)->setText(QStringLiteral("断开"));
            m_listPLCStateLaber.at(i)->setStyleSheet(Disconnect_Style);
        }
    }
}

void MainWindow::slot_Result_Heartbeat(MESRESPOND RespondState)
{
    for(int i = 1; i < m_listMESStateLaber.size(); i++){
        if(RespondState == RESPONDSUCCEED){
            m_listMESStateLaber.at(i)->setText(QStringLiteral("连接"));
            m_listMESStateLaber.at(i)->setStyleSheet(Disconnect_Style);
        }
        else if(RespondState == NETWORKERROR){
            m_listMESStateLaber.at(i)->setText(QStringLiteral("断开"));
            m_listMESStateLaber.at(i)->setStyleSheet(Disconnect_Style);
        }
        else if(RespondState == STATUSERROR){
            m_listMESStateLaber.at(i)->setText(QStringLiteral("异常"));
            m_listMESStateLaber.at(i)->setStyleSheet(Disconnect_Style);
        }
        else if(RespondState == DATAERROR){
            m_listMESStateLaber.at(i)->setText(QStringLiteral("异常"));
            m_listMESStateLaber.at(i)->setStyleSheet(Disconnect_Style);
        }
    }
}

void MainWindow::slot_AnormalWork(int n_Station, ANORMALWORK ERROR)
{
    QString str_Info = QStringLiteral("位置") + QString::number(n_Station);

    switch (ERROR) {
    case SACNERCONNECTFAILE:
        str_Info += QStringLiteral(" :扫码枪连接错误!");

        slot_ScanerConnectState(n_Station, false);
        return;
        break;
    case MESCONNECTFAILE:
        str_Info += QStringLiteral(" :MES连接错误!");

        slot_MesWork(n_Station, false);
        return;
        break;
    case PLCCONNECTFAILE:
        str_Info += QStringLiteral(" :PLC连接错误!");

        slot_PLCConnectState(false);
        return;
        break;
    case SPORTSCARDCONNECTFAILE:
        str_Info += QStringLiteral(" :机器手连接错误!");

        slot_SportCardConnectState(n_Station, false);
        return;
        break;
    case SACNERDATAERROR:
        str_Info += QStringLiteral(" :扫码数据错误!");
        break;
    case MESDATAERROR:
        str_Info += QStringLiteral(" :MES数据错误!");
        break;
    case PLCDATAERROR:
        str_Info += QStringLiteral(" :PLC错误错误!");
        break;
    case SPORTSCARDDATAERROR:
        str_Info += QStringLiteral(" :机器手数据错误!");
        break;
    case SACNERWARING:
        str_Info += QStringLiteral(" :扫码警告!");
        break;
    case MESWARING:
        str_Info += QStringLiteral(" :MES警告!");
        break;
    case PLCWARING:
        str_Info += QStringLiteral(" :PLC警告!");
        break;
    case SPORTSCARDWARING:
        str_Info += QStringLiteral(" :机器手警告!");
        break;
    case BOXERROR:
        str_Info += QStringLiteral(" :料框编号异常!");
        break;
    default:
        break;
    }

    ErrorBox(str_Info);
}

void MainWindow::slot_ReadPLCData(PLC_DATAAREA PLCData)
{
    unsigned char ucCurrentData[48] = {};

    memset(ucCurrentData, 0, 48);
    memcpy(ucCurrentData, &PLCData, 26);

    for (int i = 1; i <= 26/2; i++) {
        if(ucCurrentData[i*2] == 0){
            m_listPLCInfoLaber.at(i)->setText(QStringLiteral("报警"));
            m_listPLCInfoLaber.at(i)->setStyleSheet(StateWarning);
        }
        else if(ucCurrentData[i*2] == 1){
            m_listPLCInfoLaber.at(i)->setText(QStringLiteral("工作中"));
            m_listPLCInfoLaber.at(i)->setStyleSheet(StateWorking);
        }
        else if(ucCurrentData[i*2] == 2){
            m_listPLCInfoLaber.at(i)->setText(QStringLiteral("等待"));
            m_listPLCInfoLaber.at(i)->setStyleSheet(StateWaiting);
        }
    }
}

void MainWindow::slot_SportCardCmdData(int n_ID, SPC_SWAP_CMD Cmd)
{
    switch(n_ID) {
    case 1:{
        if(Cmd.uc_RemoveFullBox_CompleteTake == 0 &&
                Cmd.uc_FullReady == 0){
            m_listSportCardInfoLaber.at(4)->setText(QStringLiteral("等待"));
            m_listSportCardInfoLaber.at(4)->setStyleSheet(StateWaiting);
        }
        else{
            m_listSportCardInfoLaber.at(4)->setText(QStringLiteral("工作中"));
            m_listSportCardInfoLaber.at(4)->setStyleSheet(StateWorking);
        }

        if(Cmd.uc_RemoveEmptyBox_CompletePut == 0 &&
                Cmd.uc_EmptyReady == 0){
            m_listSportCardInfoLaber.at(5)->setText(QStringLiteral("等待"));
            m_listSportCardInfoLaber.at(5)->setStyleSheet(StateWaiting);
        }
        else{
            m_listSportCardInfoLaber.at(5)->setText(QStringLiteral("工作中"));
            m_listSportCardInfoLaber.at(5)->setStyleSheet(StateWorking);
        }

        break;
    }
    case 2:{
        if(Cmd.uc_RemoveFullBox_CompleteTake == 0 &&
                Cmd.uc_FullReady == 0){
            m_listSportCardInfoLaber.at(7)->setText(QStringLiteral("等待"));
            m_listSportCardInfoLaber.at(7)->setStyleSheet(StateWaiting);
        }
        else{
            m_listSportCardInfoLaber.at(7)->setText(QStringLiteral("工作中"));
            m_listSportCardInfoLaber.at(7)->setStyleSheet(StateWorking);
        }

        if(Cmd.uc_RemoveEmptyBox_CompletePut == 0 &&
                Cmd.uc_EmptyReady == 0){
            m_listSportCardInfoLaber.at(8)->setText(QStringLiteral("等待"));
            m_listSportCardInfoLaber.at(8)->setStyleSheet(StateWaiting);
        }
        else{
            m_listSportCardInfoLaber.at(8)->setText(QStringLiteral("工作中"));
            m_listSportCardInfoLaber.at(8)->setStyleSheet(StateWorking);
        }

        break;
    }
    case 3:{
        if(Cmd.uc_RemoveFullBox_CompleteTake == 0 &&
                Cmd.uc_FullReady == 0){
            m_listSportCardInfoLaber.at(11)->setText(QStringLiteral("等待"));
            m_listSportCardInfoLaber.at(11)->setStyleSheet(StateWaiting);
        }
        else{
            m_listSportCardInfoLaber.at(11)->setText(QStringLiteral("工作中"));
            m_listSportCardInfoLaber.at(11)->setStyleSheet(StateWorking);
        }

        if(Cmd.uc_RemoveEmptyBox_CompletePut == 0 &&
                Cmd.uc_EmptyReady == 0){
            m_listSportCardInfoLaber.at(12)->setText(QStringLiteral("等待"));
            m_listSportCardInfoLaber.at(12)->setStyleSheet(StateWaiting);
        }
        else{
            m_listSportCardInfoLaber.at(12)->setText(QStringLiteral("工作中"));
            m_listSportCardInfoLaber.at(12)->setStyleSheet(StateWorking);
        }
        break;
    }
    default:
        break;
    }
}

void MainWindow::slot_ScanCodeData(int n_ID, QString str_Code)
{
    m_listScanerInfoLaber.value(n_ID)->setText(str_Code);
}

void MainWindow::slot_MesWork(int n_ID, bool b_State)
{
    Q_UNUSED(n_ID);

    for(int i = 1; i < m_listMESStateLaber.size(); i++){
        if(b_State){
            m_listMESInfoLaber.at(i)->setText("工作");
            m_listMESInfoLaber.at(i)->setStyleSheet(StateWorking);
        }
        else{
            m_listMESInfoLaber.at(i)->setText(QStringLiteral("等待"));
            m_listMESInfoLaber.at(i)->setStyleSheet(StateWaiting);
        }

    }
}

