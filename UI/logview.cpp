#include "logview.h"

LogView::LogView(QWidget *parent)
    :QWidget(parent)
{
    m_pQTextEdit = NULL;
}

LogView::~LogView()
{

}

void LogView::ShowLog(const QString &str_Info)
{
    if(m_pQTextEdit == NULL)
        return;

    if(m_nTextCount > 500){
        m_pQTextEdit->clear();
        m_nTextCount = 0;
    }

    m_pQTextEdit->append(str_Info);

    m_nTextCount ++;
}

void LogView::InitView()
{
//    this->setFixedSize(360, 570);
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowTitle(QStringLiteral("日记信息"));

    m_pQTextEdit = new QTextEdit(this);
    m_pQTextEdit->setFixedSize(this->width(), this->height());
    m_pQTextEdit->move(0, 0);
    m_pQTextEdit->setWordWrapMode(QTextOption::NoWrap);

    m_nTextCount = 0;
}

void LogView::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}
