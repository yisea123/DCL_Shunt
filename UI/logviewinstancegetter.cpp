#include "logviewinstancegetter.h"

LogView *LogViewInstanceGetter::m_pLogViewInstance = NULL;
uint LogViewInstanceGetter::m_unLogViewReference = 0;

LogViewInstanceGetter::LogViewInstanceGetter()
{
    if(m_pLogViewInstance == NULL){
        m_pLogViewInstance = new LogView;
    }

    m_unLogViewReference ++;
}

LogViewInstanceGetter::~LogViewInstanceGetter()
{
    m_unLogViewReference --;
    if(m_unLogViewReference == 0){
        if(m_pLogViewInstance != NULL){
            delete m_pLogViewInstance;
            m_pLogViewInstance = NULL;
        }
    }
}

LogView *LogViewInstanceGetter::GetInstance()
{
    return m_pLogViewInstance;
}
