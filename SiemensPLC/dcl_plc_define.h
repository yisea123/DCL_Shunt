#ifndef DCL_PLC_DEFINE_H
#define DCL_PLC_DEFINE_H

#include <QString>
#include "stdint.h"

enum PC_CMD{
    PC_WARMING              = 0,
    RESTORATION             = 20,

    IN_EMPTYBOX             = 11,
    IN_FULLBOX              = 12,

    IN_HIGHWARHOUSE_PLC     = 21,
    IN_NEGATIVEDC_DC_PLC    = 22,

    IN_LINE_NOBAD           = 31,
    IN_SORTBAD              = 32,

    IN_LINE_COMPLETESORT    = 41,

    IN_LINE_LOADFULLBAD     = 51,
};

enum PLC_CMD{
    PLC_WARMING     = 0,
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
};

#pragma pack(pop) // 恢复先前的pack设置
#endif // DCL_PLC_DEFINE_H
