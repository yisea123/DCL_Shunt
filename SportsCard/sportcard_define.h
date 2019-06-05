#ifndef SPORTCARD_DEFINE_H
#define SPORTCARD_DEFINE_H

#include <QString>
#include "stdint.h"

enum SWAPCMD{
    REMOVEFULLBOX_COMPLETETAKE,
    REMOVEEMPTYBOX_COMPLETEPUT
};

struct PRODUCTINFO_{
    int n_SequencnNumber;
    QString str_ID;
    QString str_Quality;

    PRODUCTINFO_(){
        n_SequencnNumber = 0;
        str_ID = "";
        str_Quality = "";
    }
};

#pragma pack(push)  //将当前pack设置压栈保存
#pragma pack(1)     //必须在结构体定义之前使用

struct SPC_SWAP_CMD{
    uchar uc_RemoveFullBox_CompleteTake;
    uchar uc_RemoveEmptyBox_CompletePut;

    uchar uc_FullReady;
    uchar uc_EmptyReady;

    SPC_SWAP_CMD(){
        uc_RemoveFullBox_CompleteTake   = 0;
        uc_RemoveEmptyBox_CompletePut   = 0;
        uc_FullReady                    = 0;
        uc_EmptyReady                   = 0;
    }
};

#pragma pack(pop) // 恢复先前的pack设置

#endif // SPORTCARD_DEFINE_H
