#ifndef GLOBAL_H_H
#define GLOBAL_H_H

#include <QString>

enum MESRespondResult{
    MESRespondSuccess = 0,
//    EmptyBoxResult = 1,         //结果为空框
//    FullBoxResult = 2,          //结果为满框
//    BoxNotExsit = 13,            //料框在mes系统不存在，停机检查
    NetWorkError = 1,           //网络错误，停机检查
    MesStatusError = 2          //mes回复status错误，停机检查
};

const int NetworkErrorResendTimes = 3;
const QString ACTIONR = "R";

#endif // GLOBAL_H_H
