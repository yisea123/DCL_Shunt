#ifndef SPORTCARDMANAGER_H
#define SPORTCARDMANAGER_H

#include <QObject>
#include <QMap>
#include <QList>
#include <sportcarddevice.h>

class SportCardManager : public QObject
{
    Q_OBJECT
public:
    explicit SportCardManager(QObject *parent = 0);
    ~SportCardManager();

    bool AddSportCard(int n_ID,
                      QString str_IP);

    bool RemoveSportCard(int n_ID);

    bool ClearSportCard();

    bool Cmd_GetRemoveFullBox(int n_ID);

    bool Cmd_GetRemoveEmptyBox(int n_ID);

    bool Cmd_SendFullReady(int n_ID);

    bool Cmd_SendEmptyReady(int n_ID);

    bool WriteProductInfo(int n_ID, QList<PRODUCTINFO_> &list_ProductInfo);

    bool GetProductInfo(int n_ID, QList<PRODUCTINFO_> &list_ProductInfo);

    void UploadCmdSwitch(const bool &b_Switch);

private:
    void WorkSleep(int n_Msec);

signals:
    void sig_ConnectState(int n_ID, bool b_State);

    void sig_CmdData(int n_ID, SPC_SWAP_CMD Cmd);

    void sig_SportCardCmd(int n_ID, SWAPCMD cmd);

private slots:
    void slot_ConnectState(int n_ID, bool b_State);

    void slot_CmdData(int n_ID, SPC_SWAP_CMD Cmd);

    void slot_Cmd(int n_ID, SWAPCMD Cmd);

private:
    QMap<int, SportCardDevice *> m_mapSportCard;

    QList<int> m_listConnectSportCard;

};

#endif // SPORTCARDMANAGER_H
