#ifndef DCL_PLC_DEFINE_H
#define DCL_PLC_DEFINE_H

#include <QString>
#include "stdint.h"


enum PC_CMD{
    PC_WARNING              = 0,
    RESTORATION             = 20,

    IN_EMPTYBOX             = 11,
    IN_FULLBOX              = 12,

    IN_HIGHWARHOUSE_PLC     = 21,
    IN_NEGATIVEDC_DC_PLC    = 22,

    IN_LINE_NOBAD           = 31,
    IN_SORTBAD              = 32,

    IN_LINE_COMPLETESORT    = 41,

    IN_LINE_LOADFULLBAD     = 51,

    IN_EMPTYBOX_127         = 61,
    IN_EMPTYBOX_302         = 62,
    IN_FULLBOX_127          = 63,
    IN_FULLBOX_302          = 64,
};

enum PLC_CMD{
    PLC_WARNING     = 0,
    NOREQUESTSCAN   = 1,
    REQUESTSCAN     = 2,
};

#pragma pack(push)  //将当前pack设置压栈保存
#pragma pack(1)     //必须在结构体定义之前使用
struct PC_DATAAREA{
    signed char PC_Entryance_CMD;
    signed char PC_Entryance_Standby;

    signed char PC_Exit_CMD;
    signed char PC_Exit_Standby;

    signed char PC_1_QualityCheck_CMD;
    signed char PC_1_QualityCheck_Standby;

    signed char PC_1_SortingStation_CMD;
    signed char PC_1_SortingStation_Standby;

    signed char PC_1_BadStore_CMD;
    signed char PC_1_BadStore_Standby;

    signed char PC_2_QualityCheck_CMD;
    signed char PC_2_QualityCheck_Standby;

    signed char PC_2_SortingStation_CMD;
    signed char PC_2_SortingStation_Standby;

    signed char PC_2_BadStore_CMD;
    signed char PC_2_BadStore_Standby;

    signed char PC_3L_QualityCheck_CMD;
    signed char PC_3L_QualityCheck_Standby;

    signed char PC_3R_QualityCheck_CMD;
    signed char PC_3R_QualityCheck_Standby;

    signed char PC_3_SortingStation_CMD;
    signed char PC_3_SortingStation_Standby;

    signed char PC_3_BadStore_CMD;
    signed char PC_3_BadStore_Standby;

    signed char PC_HeadDCL_CMD;
    signed char PC_HeadDCL_Standby;

    PC_DATAAREA(){
        PC_Entryance_CMD            = 20;
        PC_Entryance_Standby        = 0x00;
        PC_Exit_CMD                 = 20;
        PC_Exit_Standby             = 0x00;

        PC_1_QualityCheck_CMD       = 20;
        PC_1_QualityCheck_Standby   = 0x00;
        PC_1_SortingStation_CMD     = 20;
        PC_1_SortingStation_Standby = 0x00;
        PC_1_BadStore_CMD           = 20;
        PC_1_BadStore_Standby       = 0x00;

        PC_2_QualityCheck_CMD       = 20;
        PC_2_QualityCheck_Standby   = 0x00;
        PC_2_SortingStation_CMD     = 20;
        PC_2_SortingStation_Standby = 0x00;
        PC_2_BadStore_CMD           = 20;
        PC_2_BadStore_Standby       = 0x00;

        PC_3L_QualityCheck_CMD      = 20;
        PC_3L_QualityCheck_Standby  = 0x00;
        PC_3R_QualityCheck_CMD      = 20;
        PC_3R_QualityCheck_Standby  = 0x00;
        PC_3_SortingStation_CMD     = 20;
        PC_3_SortingStation_Standby = 0x00;
        PC_3_BadStore_CMD           = 20;
        PC_3_BadStore_Standby       = 0x00;

        PC_HeadDCL_CMD              = 20;
        PC_HeadDCL_Standby          = 0x00;
    }
};

struct PLC_DATAAREA{
    signed char PLC_Entryance_CMD;
    signed char PLC_Entryance_Standby;

    signed char PLC_Exit_CMD;
    signed char PLC_Exit_Standby;

    signed char PLC_1_QualityCheck_CMD;
    signed char PLC_1_QualityCheck_Standby;

    signed char PLC_1_SortingStation_CMD;
    signed char PLC_1_SortingStation_Standby;

    signed char PLC_1_BadStore_CMD;
    signed char PLC_1_BadStore_Standby;

    signed char PLC_2_QualityCheck_CMD;
    signed char PLC_2_QualityCheck_Standby;

    signed char PLC_2_SortingStation_CMD;
    signed char PLC_2_SortingStation_Standby;

    signed char PLC_2_BadStore_CMD;
    signed char PLC_2_BadStore_Standby;

    signed char PLC_3L_QualityCheck_CMD;
    signed char PLC_3L_QualityCheck_Standby;

    signed char PLC_3R_QualityCheck_CMD;
    signed char PLC_3R_QualityCheck_Standby;

    signed char PLC_3_SortingStation_CMD;
    signed char PLC_3_SortingStation_Standby;

    signed char PLC_3_BadStore_CMD;
    signed char PLC_3_BadStore_Standby;

    signed char PLC_HeadDCL_CMD;
    signed char PLC_HeadDCL_Standby;


    PLC_DATAAREA(){
        PLC_Entryance_CMD            = 0x01;
        PLC_Entryance_Standby        = 0x00;
        PLC_Exit_CMD                 = 0x01;
        PLC_Exit_Standby             = 0x00;

        PLC_1_QualityCheck_CMD       = 0x01;
        PLC_1_QualityCheck_Standby   = 0x00;
        PLC_1_SortingStation_CMD     = 0x01;
        PLC_1_SortingStation_Standby = 0x00;
        PLC_1_BadStore_CMD           = 0x01;
        PLC_1_BadStore_Standby       = 0x00;

        PLC_2_QualityCheck_CMD       = 0x01;
        PLC_2_QualityCheck_Standby   = 0x00;
        PLC_2_SortingStation_CMD     = 0x01;
        PLC_2_SortingStation_Standby = 0x00;
        PLC_2_BadStore_CMD           = 0x01;
        PLC_2_BadStore_Standby       = 0x00;

        PLC_3L_QualityCheck_CMD      = 0x01;
        PLC_3L_QualityCheck_Standby  = 0x00;
        PLC_3R_QualityCheck_CMD      = 0x01;
        PLC_3R_QualityCheck_Standby  = 0x00;
        PLC_3_SortingStation_CMD     = 0x01;
        PLC_3_SortingStation_Standby = 0x00;
        PLC_3_BadStore_CMD           = 0x01;
        PLC_3_BadStore_Standby       = 0x00;

        PLC_HeadDCL_CMD              = 0x01;
        PLC_HeadDCL_Standby          = 0x00;
    }
};

#pragma pack(pop) // 恢复先前的pack设置
#endif // DCL_PLC_DEFINE_H
