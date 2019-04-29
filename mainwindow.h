#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Scaner/scanermanager.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private slots:
    void slot_CodeData(int n_ID,
                       bool b_ScanState,
                       QString str_Code);

    void slot_ConnectState(int n_ID,
                           bool b_ConnectState);

private:
    Ui::MainWindow *ui;

    ScanerManager m_oScanerManager;
};

#endif // MAINWINDOW_H
