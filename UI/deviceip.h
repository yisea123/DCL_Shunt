#ifndef DEVICEIP_H
#define DEVICEIP_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>

#include "ui_define.h"
#include <ConfigFile/datafile.h>

namespace Ui {
class DeviceIP;
}

class DeviceIP : public QWidget
{
    Q_OBJECT

public:
    explicit DeviceIP(QWidget *parent = nullptr);
    ~DeviceIP();

    void Write_pb();

    void Read_pb();

private:
    void InitView();

    void ReadConfig();

    void WriteConfig();

private:
    Ui::DeviceIP *ui;

    QList<QLineEdit *> m_listScanerIPLineEdit;
    QList<QLineEdit *> m_listSportCardIPLineEdit;
};

#endif // DEVICEIP_H
