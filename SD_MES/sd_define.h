#ifndef SD_DEFINE_H
#define SD_DEFINE_H

#include <QString>

const QString MESURL_TEST = "http://10.10.156.11:50000/sapdevwebservice/ExecutingServiceService?wsdl&mode=ws_policy";
const QString MESURL_DEBUG = "http://10.10.156.11:51000/sapdevwebservice/ExecutingServiceService?wsdl&mode=ws_policy";
const QString MESURL_RUNNING = "http://10.10.180.13:50000/sapdevwebservice/ExecutingServiceService?wsdl";
const QString MESURL_CURRENTUSE = "http://10.10.156.11:50000/sapdevwebservice/ExecutingServiceService?wsdl&mode=ws_policy";
const QString MESURL_HEARTBEAT_TEST= "http://10.10.156.11:50000/sapdevwebservice/MachineAccessTestServiceService?wsdl";
const QString MESURL_HEARTBEAT_DEBUG = "http://10.10.156.11:51000/sapdevwebservice/MachineAccessTestServiceService?wsdl";
const QString MESURL_HEARTBEAT_RUNNING = "http://10.10.180.13:50000/sapdevwebservice/MachineAccessTestServiceService?wsdl";
const QString MESURL_HEARTBEAT_CURRENTUSE = "http://10.10.156.11:50000/sapdevwebservice/MachineAccessTestServiceService?wsdl";

enum MESURLTYPE{
    TEST,
    DEBUG,
    RUNNING,
    CURRENTUSE,
    HEARTBEAT_TEST,
    HEARTBEAT_DEBUG,
    HEARTBEAT_RUNNING,
    HEARTBEAT_CURRENTUSE
};

enum BOXSTATE{
    EMPTYBOX = 1,
    FULLBOX,
    INEXISTENCE,
};

enum EXPORTDIRECTION{
    IN_HIGHWAREHOUSE,
    IN_NEGATIVEDC_DC
};

enum MESRESPOND{
    RESPONDSUCCEED,
    NETWORKERROR,
    STATUSERROR,
    DATAERROR,
};

struct PRODUCTINFO{
    int n_SequencnNumber;
    QString str_ID;
    QString str_Quality;

    PRODUCTINFO(){
        n_SequencnNumber = 0;
        str_ID = "";
        str_Quality = "";
    }
};

#endif // SD_DEFINE_H
