#include "sdmesmanager.h"

#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QJsonObject>

SDMesManager::SDMesManager(QThread *parent)
    : QThread(parent)
{
    SetMesUrl(TEST);

    m_bRunSwitch = true;

    m_pMESAddBackFixBox         = NULL;
    m_pMESInStationInterface    = NULL;
    m_pMESNetTest               = NULL;
    m_pMESRemoveBadBox          = NULL;
    m_pMESReqBadInfo            = NULL;
    m_pMESReqBoxIsEmpty         = NULL;

    InitSDMesManager();

    this->start();
}

SDMesManager::~SDMesManager()
{
    m_bRunSwitch = false;

    this->quit();
    this->wait();

    ExitSDMesManager();
}

void SDMesManager::InitSDMesManager()
{
    m_pMESAddBackFixBox         = new MESAddBackFixBox(m_oMesUrl);
    m_pMESInStationInterface    = new MESInStationInterface(m_oMesUrl);
    m_pMESNetTest               = new MESNetTest(m_oMesUrl);
    m_pMESRemoveBadBox          = new MESRemoveBadBox(m_oMesUrl);
    m_pMESReqBadInfo            = new MESReqBadInfo(m_oMesUrl);
    m_pMESReqBoxIsEmpty         = new MESReqBoxIsEmpty(m_oMesUrl);

    connect(m_pMESAddBackFixBox, SIGNAL(sigUploadResult(QString, QString, int)),
            this, SLOT(slot_Result_SendBadProductList(QString, QString, int)));

    connect(m_pMESInStationInterface, SIGNAL(sigInStationRespond(QString, QString, QString, int)),
            this, SLOT(slot_Result_RequestExportDirection(QString, QString, QString, int)));

    connect(m_pMESNetTest, SIGNAL(sigTestResult(int)),
            this, SLOT(slot_Result_Heartbeat(int)));

    connect(m_pMESRemoveBadBox, SIGNAL(sigUploadResult(QString, QString, int)),
            this, SLOT(slot_Result_CompleteSortingBadProduct(QString, QString, int)));

    connect(m_pMESReqBadInfo, SIGNAL(sigReqBadInfo(QString, QString, QJsonArray, int)),
            this, SLOT(slot_Result_RequestProductListInfo(QString, QString, QJsonArray, int)));

    connect(m_pMESReqBoxIsEmpty, SIGNAL(sigIsEmptyRespond(QString, QString, int, int)),
            this, SLOT(slot_Result_RequestBoxIsEmpty(QString, QString, int, int)));
}

void SDMesManager::ExitSDMesManager()
{
    if(m_pMESAddBackFixBox != NULL){
        delete m_pMESAddBackFixBox;
        m_pMESAddBackFixBox = NULL;
    }

    if(m_pMESInStationInterface != NULL){
        delete m_pMESInStationInterface;
        m_pMESInStationInterface = NULL;
    }

    if(m_pMESNetTest != NULL){
        delete m_pMESNetTest;
        m_pMESNetTest = NULL;
    }

    if(m_pMESRemoveBadBox != NULL){
        delete m_pMESRemoveBadBox;
        m_pMESRemoveBadBox = NULL;
    }

    if(m_pMESReqBadInfo != NULL){
        delete m_pMESReqBadInfo;
        m_pMESReqBadInfo = NULL;
    }

    if(m_pMESReqBoxIsEmpty != NULL){
        delete m_pMESReqBoxIsEmpty;
        m_pMESReqBoxIsEmpty = NULL;
    }
}

bool SDMesManager::SetMesUrl(MESURLTYPE MESUrl_Type)
{
    switch(MESUrl_Type){
    case TEST:
        m_oMesUrl.setUrl(MESURL_TEST);
        break;
    case DEBUG:
        m_oMesUrl.setUrl(MESURL_DEBUG);
        break;
    case RUNNING:
        m_oMesUrl.setUrl(MESURL_RUNNING);
        break;
    case CURRENTUSE:
        m_oMesUrl.setUrl(MESURL_CURRENTUSE);
        break;
    case HEARTBEAT_TEST:
        m_oMesUrl.setUrl(MESURL_HEARTBEAT_TEST);
        break;
    case HEARTBEAT_DEBUG:
        m_oMesUrl.setUrl(MESURL_HEARTBEAT_DEBUG);
        break;
    case HEARTBEAT_RUNNING:
        m_oMesUrl.setUrl(MESURL_HEARTBEAT_RUNNING);
        break;
    case HEARTBEAT_CURRENTUSE:
        m_oMesUrl.setUrl(MESURL_HEARTBEAT_CURRENTUSE);
        break;
    default:
        break;
    }

    if(m_pMESAddBackFixBox              == NULL ||
            m_pMESInStationInterface    == NULL ||
            m_pMESNetTest               == NULL ||
            m_pMESRemoveBadBox          == NULL ||
            m_pMESReqBadInfo            == NULL ||
            m_pMESReqBoxIsEmpty         == NULL)
        return false;

    m_pMESAddBackFixBox->setMesUrl(m_oMesUrl);
    m_pMESInStationInterface->setMesUrl(m_oMesUrl);
    m_pMESNetTest->setMesUrl(m_oMesUrl);
    m_pMESRemoveBadBox->setMesUrl(m_oMesUrl);
    m_pMESReqBadInfo->setMesUrl(m_oMesUrl);
    m_pMESReqBoxIsEmpty->setMesUrl(m_oMesUrl);

    return true;
}

bool SDMesManager::UploadBadProductList(const int &n_ScanerId,
                                        const QString &str_Code,
                                        const QList<PRODUCTINFO> &list_Product)
{
    QJsonArray json_BadProductList;

    if(!TD_QListToJson(list_Product, json_BadProductList))
        return false;

    if(m_pMESAddBackFixBox == NULL)
        return false;

    m_pMESAddBackFixBox->sendBackFixBox(QString::number(n_ScanerId),
                                        str_Code,
                                        json_BadProductList);
    return true;
}

bool SDMesManager::RequestExportDirection(const int &n_ScanerId,
                                          const QString &str_Code)
{
    if(m_pMESInStationInterface == NULL)
        return false;

    m_pMESInStationInterface->sendInStation(QString::number(n_ScanerId),
                                            str_Code);
    return true;
}

bool SDMesManager::CompleteSortingBadProduct(const int &n_ScanerId,
                                             const QString &str_Code)
{
    if(m_pMESRemoveBadBox == NULL)
        return false;

    m_pMESRemoveBadBox->sendRemoveBadInfo(QString::number(n_ScanerId),
                                          str_Code);
    return true;
}

bool SDMesManager::RequestProductListInfo(const int &n_ScanerId,
                                          const QString &str_Code)
{
    if(m_pMESReqBadInfo == NULL)
        return false;

    m_pMESReqBadInfo->sendReqBadInfo(QString::number(n_ScanerId),
                                     str_Code);
    return true;
}

bool SDMesManager::RequestBoxIsEmpty(const int &n_ScanerId,
                                     const QString &str_Code)
{
    if(m_pMESReqBoxIsEmpty == NULL)
        return false;

    m_pMESReqBoxIsEmpty->sendReqBoxIsEmpty(QString::number(n_ScanerId),
                                           str_Code);
    return true;
}

void SDMesManager::WorkSleep(int n_Msec)
{
    QTime o_DieTime = QTime::currentTime().addMSecs(n_Msec);
    while(QTime::currentTime() < o_DieTime ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        QThread::msleep(10);
    }
}

void SDMesManager::Heartbeat()
{
    if(m_pMESNetTest != NULL){
        m_pMESNetTest->sendTestMes();
    }
}

bool SDMesManager::TD_JsonToQList(const QJsonArray &json_ProductList,
                                  QList<PRODUCTINFO> &list_Product)
{
    if(json_ProductList.isEmpty())
        return false;

    PRODUCTINFO ProductInfo;
    for(int i = 0; i < json_ProductList.size(); i++){
        QJsonObject o_Json = json_ProductList.at(i).toObject();
        ProductInfo.n_SequencnNumber = i;
        ProductInfo.str_ID = o_Json.value("SFC").toString();
        ProductInfo.str_Quality = o_Json.value("IS_NG").toString();

        list_Product.append(ProductInfo);
    }

    return true;
}

bool SDMesManager::TD_QListToJson(const QList<PRODUCTINFO> &list_Product,
                                  QJsonArray &json_ProductList)
{
    if(list_Product.isEmpty())
        return false;

    QJsonObject o_Json;
    for(int i = 0; i < list_Product.size(); i++){

        if(list_Product.at(i).n_SequencnNumber != i)
            return false;

        QJsonObject o_Json;
        o_Json.insert("SFC", list_Product.at(i).str_ID);
        o_Json.insert("IS_NG", list_Product.at(i).str_Quality);

        json_ProductList.insert(i, o_Json);
   }

    return true;
}

MESRESPOND SDMesManager::TD_MesRespondState(const int &n_Result)
{
    switch (n_Result) {
    case 0:
        return RESPONDSUCCEED;
    case 1:
        return NETWORKERROR;
    case 2:
        return STATUSERROR;
    default:
        break;
    }

    return STATUSERROR;
}

BOXSTATE SDMesManager::TD_BoxState(const int &n_BoxType)
{
    switch (n_BoxType) {
    case 1:
        return EMPTYBOX;
    case 2:
        return FULLBOX;
    case 3:
        return INEXISTENCE;
    default:
        break;
    }

    return INEXISTENCE;
}

void SDMesManager::run()
{
    while(m_bRunSwitch){
        Heartbeat();

        WorkSleep(500);
    }
}

void SDMesManager::slot_Result_SendBadProductList(QString str_ScanerId,
                                                  QString str_Code,
                                                  int n_Result)
{
    emit sig_Result_SendBadProductList(str_ScanerId.toInt(),
                                       str_Code,
                                       TD_MesRespondState(n_Result));
}

void SDMesManager::slot_Result_RequestExportDirection(QString str_ScanerId,
                                                      QString str_Code,
                                                      QString str_ResultInfo,
                                                      int n_Result)
{
    EXPORTDIRECTION Direction;

    if(str_ResultInfo == "FFR")
        Direction = IN_NEGATIVEDC_DC;
    else if(str_ResultInfo == "IN")
        Direction = IN_HIGHWAREHOUSE;
    else
        emit sig_Result_RequestExportDirection(str_ScanerId.toInt(),
                                               str_Code,
                                               Direction,
                                               DATAERROR);

    emit sig_Result_RequestExportDirection(str_ScanerId.toInt(),
                                           str_Code,
                                           Direction,
                                           TD_MesRespondState(n_Result));
}

void SDMesManager::slot_Result_Heartbeat(int n_Type)
{
    emit sig_Result_Heartbeat(TD_MesRespondState(n_Type));
}

void SDMesManager::slot_Result_CompleteSortingBadProduct(QString str_ScanerId,
                                                         QString str_Code,
                                                         int n_Result)
{
    emit sig_Result_CompleteSortingBadProduct(str_ScanerId.toInt(),
                                              str_Code,
                                              TD_MesRespondState(n_Result));
}

void SDMesManager::slot_Result_RequestProductListInfo(QString str_ScanerId,
                                                      QString str_Code,
                                                      QJsonArray json_ProductList,
                                                      int n_Result)
{
    QList<PRODUCTINFO> list_Product;

    if(TD_JsonToQList(json_ProductList, list_Product))
        emit sig_Result_RequestProductListInfo(str_ScanerId.toInt(),
                                               str_Code,
                                               list_Product,
                                               DATAERROR);

    emit sig_Result_RequestProductListInfo(str_ScanerId.toInt(),
                                           str_Code,
                                           list_Product,
                                           TD_MesRespondState(n_Result));
}

void SDMesManager::slot_Result_RequestBoxIsEmpty(QString str_ScanerId,
                                                 QString str_Code,
                                                 int n_BoxType,
                                                 int n_Result)
{
    emit sig_Result_RequestBoxIsEmpty(str_ScanerId.toInt(),
                                      str_Code,
                                      TD_BoxState(n_BoxType),
                                      TD_MesRespondState(n_Result));
}
