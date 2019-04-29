#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&m_oScanerManager, SIGNAL(sig_CodeData(int, bool ,QString)),
            this, SLOT(slot_CodeData(int, bool ,QString)));
    connect(&m_oScanerManager, SIGNAL(sig_ConnectState(int, bool)),
            this, SLOT(slot_ConnectState(int, bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    m_oScanerManager.AddScanerDevice(ui->lineEdit->text().toInt(),
                                     ui->lineEdit_2->text(),
                                     ui->lineEdit_3->text().toInt());
}

void MainWindow::on_pushButton_2_clicked()
{
    m_oScanerManager.RemoveScanerDevice(ui->lineEdit->text().toInt());
}

void MainWindow::on_pushButton_3_clicked()
{
    m_oScanerManager.ClearScanerDevice();
}

void MainWindow::on_pushButton_4_clicked()
{
    m_oScanerManager.RequsetCode(ui->lineEdit->text().toInt());
}

void MainWindow::on_pushButton_5_clicked()
{
//    m_oScanerManager.RequsetCode_Ansy(ui->lineEdit->text().toInt());
}

void MainWindow::slot_CodeData(int n_ID, bool b_ScanState, QString str_Code)
{
    QString str_Info = "ID: " +
            QString::number(n_ID) +
            " ScanState: " +
            QString::number(b_ScanState) +
            " Code: " +
            str_Code;

    ui->textBrowser->append(str_Info);
}

void MainWindow::slot_ConnectState(int n_ID, bool b_ConnectState)
{
    QString str_Info = "ID: " +
            QString::number(n_ID) +
            " ConnectState: " +
            QString::number(b_ConnectState);

    ui->textBrowser->append(str_Info);
}
