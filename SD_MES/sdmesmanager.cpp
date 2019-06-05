#include "sdmesmanager.h"

#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QJsonObject>
#include <QMetaType>
#include <QDebug>

SDMesManager::SDMesManager(QThread *parent)
    : QThread(parent)
{
    qRegisterMetaType<MESRESPOND>("MESRESPOND");
    qRegisterMetaType<EXPORTDIRECTION>("EXPORTDIRECTION");
    qRegisterMetaType<BOXSTATE>("BOXSTATE");

    m_pMESNetTest               = NULL;
//    m_pMESAddBackFixBox         = NULL;
//    m_pMESInStationInterface    = NULL;
//    m_pMESRemoveBadBox          = NULL;
//    m_pMESReqBadInfo            = NULL;
//    m_pMESReqBoxIsEmpty         = NULL;

    m_pQTimer = NULL;
    m_pQTimer = new QTimer;

    connect(m_pQTimer, &QTimer::timeout,
            this, &SDMesManager::slot_TimeOut);

    m_pQTimer->start(500);

//    InitSDMesManager();

    SetMesUrl(CURRENTUSE);

    m_bRunSwitch = true;

//    this->start();
}

SDMesManager::~SDMesManager()
{
    m_bRunSwitch = false;

    if(m_pQTimer != NULL){
        if(m_pQTimer->isActive())
            m_pQTimer->stop();

        delete m_pQTimer;
        m_pQTimer = NULL;
    }

    this->quit();
    this->wait();

    ExitSDMesManager();
}

void SDMesManager::InitSDMesManager()
{

//    m_pMESAddBackFixBox         = new MESAddBackFixBox(m_oMesUrl);
//    m_pMESInStationInterface    = new MESInStationInterface(m_oMesUrl);
//    m_pMESNetTest               = new MESNetTest(m_oMesUrl);
//    m_pMESRemoveBadBox          = new MESRemoveBadBox(m_oMesUrl);
//    m_pMESReqBadInfo            = new MESReqBadInfo(m_oMesUrl);
//    m_pMESReqBoxIsEmpty         = new MESReqBoxIsEmpty(m_oMesUrl);

//    connect(m_pMESAddBackFixBox, SIGNAL(sigUploadResult(QString, QString, int)),
//            this, SLOT(slot_Result_SendBadProductList(QString, QString, int)));

//    connect(m_pMESInStationInterface, SIGNAL(sigInStationRespond(QString, QString, QString, int)),
//            this, SLOT(slot_Result_RequestExportDirection(QString, QString, QString, int)));

//    connect(m_pMESNetTest, SIGNAL(sigTestResult(int)),
//            this, SLOT(slot_Result_Heartbeat(int)));

//    connect(m_pMESRemoveBadBox, SIGNAL(sigUploadResult(QString, QString, int)),
//            this, SLOT(slot_Result_CompleteSortingBadProduct(QString, QString, int)));

//    connect(m_pMESReqBadInfo, SIGNAL(sigReqBadInfo(QString, QString, QJsonArray, int)),
//            this, SLOT(slot_Result_RequestProductListInfo(QString, QString, QJsonArray, int)));

//    connect(m_pMESReqBoxIsEmpty, SIGNAL(sigIsEmptyRespond(QString, QString, int, int)),
//            this, SLOT(slot_Result_RequestBoxIsEmpty(QString, QString, int, int)));
}

void SDMesManager::ExitSDMesManager()
{
    if(m_pMESNetTest != NULL){
        m_pMESNetTest->deleteLater();
        m_pMESNetTest = NULL;
    }

    QMapIterator<int, MESAddBackFixBox *> map_Iterator_1(m_mapAddBackFixBox);
    while(map_Iterator_1.hasNext()){
          map_Iterator_1.next();
          MESAddBackFixBox *p_AddBackFixBox = map_Iterator_1.value();
          p_AddBackFixBox->deleteLater();
    }

    QMapIterator<int, MESInStationInterface *> map_Iterator_2(m_mapInStationInterface);
    while(map_Iterator_2.hasNext()){
          map_Iterator_2.next();
          MESInStationInterface *p_InStationInterface = map_Iterator_2.value();
          p_InStationInterface->deleteLater();
    }

    QMapIterator<int, MESRemoveBadBox *> map_Iterator_3(m_mapRemoveBadBox);
    while(map_Iterator_3.hasNext()){
          map_Iterator_3.next();
          MESRemoveBadBox *p_RemoveBadBox = map_Iterator_3.value();
          p_RemoveBadBox->deleteLater();
    }

    QMapIterator<int, MESReqBadInfo *> map_Iterator_4(m_mapReqBadInfo);
    while(map_Iterator_4.hasNext()){
          map_Iterator_4.next();
          MESReqBadInfo *p_ReqBadInfo = map_Iterator_4.value();
          p_ReqBadInfo->deleteLater();
    }

    QMapIterator<int, MESReqBoxIsEmpty *> map_Iterator_5(m_mapReqBoxIsEmpty);
    while(map_Iterator_5.hasNext()){
          map_Iterator_5.next();
          MESReqBoxIsEmpty *p_ReqBoxIsEmpty = map_Iterator_5.value();
          p_ReqBoxIsEmpty->deleteLater();
    }

    m_mapAddBackFixBox.clear();
    m_mapInStationInterface.clear();
    m_mapRemoveBadBox.clear();
    m_mapReqBadInfo.clear();
    m_mapReqBoxIsEmpty.clear();

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

//    if(m_pMESAddBackFixBox              == NULL ||
//            m_pMESInStationInterface    == NULL ||
//            m_pMESNetTest               == NULL ||
//            m_pMESRemoveBadBox          == NULL ||
//            m_pMESReqBadInfo            == NULL ||
//            m_pMESReqBoxIsEmpty         == NULL)
//        return false;

//    m_pMESAddBackFixBox->setMesUrl(m_oMesUrl);
//    m_pMESInStationInterface->setMesUrl(m_oMesUrl);
//    m_pMESNetTest->setMesUrl(m_oMesUrl);
//    m_pMESRemoveBadBox->setMesUrl(m_oMesUrl);
//    m_pMESReqBadInfo->setMesUrl(m_oMesUrl);
//    m_pMESReqBoxIsEmpty->setMesUrl(m_oMesUrl);

    return true;
}

bool SDMesManager::UploadBadProductList(const int &n_ScanerId,
                                        const QString &str_Code,
                                        const QList<PRODUCTINFO> &list_Product)
{
    QJsonArray json_BadProductList;

    if(!TD_QListToJson(list_Product, json_BadProductList))
        return false;

    if(m_mapAddBackFixBox.contains(n_ScanerId))
        return false;

    auto *p_MESAddBackFixBox = new MESAddBackFixBox(m_oMesUrl);
    connect(p_MESAddBackFixBox, SIGNAL(sigUploadResult(QString, QString, int)),
            this, SLOT(slot_Result_SendBadProductList(QString, QString, int)));

    m_mapAddBackFixBox.insert(n_ScanerId, p_MESAddBackFixBox);

    p_MESAddBackFixBox->sendBackFixBox(QString::number(n_ScanerId),
                                       str_Code,
                                       json_BadProductList);
    return true;
}

bool SDMesManager::RequestExportDirection(const int &n_ScanerId,
                                          const QString &str_Code)
{
    if(m_mapInStationInterface.contains(n_ScanerId))
        return false;

    auto p_MESInStationInterface = new MESInStationInterface(m_oMesUrl);
    connect(p_MESInStationInterface, SIGNAL(sigInStationRespond(QString, QString, QString, int)),
            this, SLOT(slot_Result_RequestExportDirection(QString, QString, QString, int)));

    m_mapInStationInterface.insert(n_ScanerId, p_MESInStationInterface);

    p_MESInStationInterface->sendInStation(QString::number(n_ScanerId),
                                           str_Code);
    return true;
}

bool SDMesManager::CompleteSortingBadProduct(const int &n_ScanerId,
                                             const QString &str_Code)
{
    if(m_mapRemoveBadBox.contains(n_ScanerId))
        return false;

    auto p_MESRemoveBadBox = new MESRemoveBadBox(m_oMesUrl);
    connect(p_MESRemoveBadBox, SIGNAL(sigUploadResult(QString, QString, int)),
            this, SLOT(slot_Result_CompleteSortingBadProduct(QString, QString, int)));

    m_mapRemoveBadBox.insert(n_ScanerId, p_MESRemoveBadBox);

    p_MESRemoveBadBox->sendRemoveBadInfo(QString::number(n_ScanerId),
                                         str_Code);
    return true;
}

bool SDMesManager::RequestProductListInfo(const int &n_ScanerId,
                                          const QString &str_Code)
{
    if(m_mapReqBadInfo.contains(n_ScanerId))
        return false;

    auto p_MESReqBadInfo = new MESReqBadInfo(m_oMesUrl);
    connect(p_MESReqBadInfo, SIGNAL(sigReqBadInfo(QString, QString, QJsonArray, int)),
            this, SLOT(slot_Result_RequestProductListInfo(QString, QString, QJsonArray, int)));

    m_mapReqBadInfo.insert(n_ScanerId, p_MESReqBadInfo);

    p_MESReqBadInfo->sendReqBadInfo(QString::number(n_ScanerId),
                                    str_Code);
    return true;
}

bool SDMesManager::RequestBoxIsEmpty(const int &n_ScanerId,
                                     const QString &str_Code)
{
    if(m_mapReqBoxIsEmpty.contains(n_ScanerId))
        return false;

    auto p_MESReqBoxIsEmpty = new MESReqBoxIsEmpty(m_oMesUrl);
    connect(p_MESReqBoxIsEmpty, SIGNAL(sigIsEmptyRespond(QString, QString, int, int)),
            this, SLOT(slot_Result_RequestBoxIsEmpty(QString, QString, int, int)));

    m_mapReqBoxIsEmpty.insert(n_ScanerId, p_MESReqBoxIsEmpty);

    p_MESReqBoxIsEmpty->sendReqBoxIsEmpty(QString::number(n_ScanerId),
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
    if(m_pMESNetTest == NULL){
        m_pMESNetTest = new MESNetTest(m_oMesUrl);
        connect(m_pMESNetTest, SIGNAL(sigTestResult(int)),
                this, SLOT(slot_Result_Heartbeat(int)));

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
        WorkSleep(3000);
        Heartbeat();
    }
}

void SDMesManager::slot_TimeOut()
{
    Heartbeat();
}

void SDMesManager::slot_Result_SendBadProductList(QString str_ScanerId,
                                                  QString str_Code,
                                                  int n_Result)
{
    int n_ScanerID = str_ScanerId.toInt();

    if(!m_mapAddBackFixBox.contains(n_ScanerID))
        return;

    auto p_MESAddBackFixBox = m_mapAddBackFixBox.value(n_ScanerID);

    p_MESAddBackFixBox->deleteLater();

    m_mapAddBackFixBox.remove(n_ScanerID);

    emit sig_Result_SendBadProductList(n_ScanerID,
                                       str_Code,
                                       TD_MesRespondState(n_Result));
}

void SDMesManager::slot_Result_RequestExportDirection(QString str_ScanerId,
                                                      QString str_Code,
                                                      QString str_ResultInfo,
                                                      int n_Result)
{
    int n_ScanerID = str_ScanerId.toInt();

    if(!m_mapInStationInterface.contains(n_ScanerID))
        return;

    auto p_MESInStationInterface = m_mapInStationInterface.value(n_ScanerID);

    p_MESInStationInterface->deleteLater();

    m_mapInStationInterface.remove(n_ScanerID);

    EXPORTDIRECTION Direction = IN_HIGHWAREHOUSE;

    if(str_ResultInfo == "FFR")
        Direction = IN_NEGATIVEDC_DC;
    else if(str_ResultInfo == "IN")
        Direction = IN_HIGHWAREHOUSE;
    else
        emit sig_Result_RequestExportDirection(n_ScanerID,
                                               str_Code,
                                               Direction,
                                               DATAERROR);

    emit sig_Result_RequestExportDirection(n_ScanerID,
                                           str_Code,
                                           Direction,
                                           TD_MesRespondState(n_Result));
}

void SDMesManager::slot_Result_Heartbeat(int n_Type)
{
    m_pMESNetTest->deleteLater();

//    delete m_pMESNetTest;
    m_pMESNetTest = NULL;

    emit sig_Result_Heartbeat(TD_MesRespondState(n_Type));
}

void SDMesManager::slot_Result_CompleteSortingBadProduct(QString str_ScanerId,
                                                         QString str_Code,
                                                         int n_Result)
{
    int n_ScanerID = str_ScanerId.toInt();

    if(!m_mapRemoveBadBox.contains(n_ScanerID))
        return;

    auto p_MESRemoveBadBox = m_mapRemoveBadBox.value(n_ScanerID);

    p_MESRemoveBadBox->deleteLater();

    m_mapRemoveBadBox.remove(n_ScanerID);

    emit sig_Result_CompleteSortingBadProduct(n_ScanerID,
                                              str_Code,
                                              TD_MesRespondState(n_Result));
}

void SDMesManager::slot_Result_RequestProductListInfo(QString str_ScanerId,
                                                      QString str_Code,
                                                      QJsonArray json_ProductList,
                                                      int n_Result)
{
    int n_ScanerID = str_ScanerId.toInt();

    if(!m_mapReqBadInfo.contains(n_ScanerID))
        return;

    auto p_MESReqBadInfo = m_mapReqBadInfo.value(n_ScanerID);

    p_MESReqBadInfo->deleteLater();

    m_mapReqBadInfo.remove(n_ScanerID);

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
    int n_ScanerID = str_ScanerId.toInt();

    if(!m_mapReqBoxIsEmpty.contains(n_ScanerID))
        return;

    auto p_MESReqBoxIsEmpty = m_mapReqBoxIsEmpty.value(n_ScanerID);

    p_MESReqBoxIsEmpty->deleteLater();

    m_mapReqBoxIsEmpty.remove(n_ScanerID);

    emit sig_Result_RequestBoxIsEmpty(str_ScanerId.toInt(),
                                      str_Code,
                                      TD_BoxState(n_BoxType),
                                      TD_MesRespondState(n_Result));
}
