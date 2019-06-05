#ifndef LOGVIEWINSTANCEGETTER_H
#define LOGVIEWINSTANCEGETTER_H

#include "logview.h"

class LogViewInstanceGetter
{
public:
    LogViewInstanceGetter();
    ~LogViewInstanceGetter();

public:
    static LogView * GetInstance();

private:
    static LogView *m_pLogViewInstance;
    static uint m_unLogViewReference;
};

#endif // LOGVIEWINSTANCEGETTER_H
