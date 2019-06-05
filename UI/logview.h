#ifndef LOGVIEW_H
#define LOGVIEW_H

#include <QWidget>
#include <QTextEdit>
#include <QCloseEvent>

#include "ui_define.h"

class LogView : public QWidget
{
    Q_OBJECT
public:
    explicit LogView(QWidget *parent = 0);
    ~LogView();

    void ShowLog(const QString &str_Info);

//private:
    void InitView();

protected:
    void closeEvent(QCloseEvent *event);

private:
    QTextEdit *m_pQTextEdit;

    int m_nTextCount;
};

#endif // LOGVIEW_H
