#ifndef SPORTCARDDEBUGVIEW_H
#define SPORTCARDDEBUGVIEW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QButtonGroup>
#include <QTableWidget>

#include "ui_define.h"
#include "SportsCard/sportcardmanager.h"

namespace Ui {
class SportCardDebugView;
}

class SportCardDebugView : public QWidget
{
    Q_OBJECT

public:
    explicit SportCardDebugView(SportCardManager * &p_SportCardManager, QWidget *parent = nullptr);
    ~SportCardDebugView();

    bool SportCardCmd_GetRemoveFullBox(const int &n_ID);

    bool SportCardCmd_Cmd_GetRemoveEmptyBox(const int &n_ID);

    bool SportCardCmd_Cmd_SendFullReady(const int &n_ID);

    bool SportCardCmd_Cmd_SendEmptyReady(const int &n_ID);

    bool SportCardCmd_WriteProductInfo(const int &n_ID,
                                       QList<PRODUCTINFO_> &list_ProductInfo);

    bool SportCardCmd_GetProductInfo(const int &n_ID,
                                     QList<PRODUCTINFO_> &list_ProductInfo);


private:
    void GetTableInfo(QTableWidget *& p_QTableWidget, QList<PRODUCTINFO_> &list_Product);

    void SetTableInfo(QTableWidget *& p_QTableWidget, const QList<PRODUCTINFO_> &list_Product);

    void ErrorBox(QString str_Info);

private:
    Ui::SportCardDebugView *ui;
    SportCardManager *m_pSportCardManager;

    QButtonGroup *m_pStationButtonGroup;
    QButtonGroup *m_pSendCmdButtonGroup;
    QButtonGroup *m_pTbaleButtonGroup;

    int m_nStationClickID;
    int m_nSendCmdClickID;
    int m_nTbaleCodeClickID;
};

#endif // SPORTCARDDEBUGVIEW_H
