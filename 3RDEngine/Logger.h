#pragma once

#ifndef _3RDE_LOGGER_H_
#define _3RDE_LOGGER_H_

#include "Interfaces.h"

class gLogger :  public ILogger
{
public:
    gLogger( SPPLATFORM platform, const gString& logFileName );
    ~gLogger();

    eLoggerLevel getLoggerMinLevel();
    void setLoggerMinLevel(eLoggerLevel level);

    void logMessage(const gString& fmt);
    void logWarning(const gString& fmt);
    void logError(const gString& fmt);

    void logMessage(const char* fmt, ...);
    void logWarning(const char* fmt, ...);
    void logError(const char* fmt, ...);

    bool initialize();

protected:
    SPFILE m_spLogger;
    eLoggerLevel m_minLevel;

    bool finalize();
    gString& getDateTimeStr(gString& str) const;
    gString& getDateStr(gString& str) const;
    gString& getTimeStr(gString& str) const;

};

#endif

