#include <ctime>  
#include <time.h>
#include "Logger.h"
#include "File.h"

// ------------------------------------
//
//		*** class gLogger ***
//
// ------------------------------------

gLogger::gLogger( SPPLATFORM platform, const gString& logFileName ) : 
	m_minLevel(gLogger::eLoggerLevel::GLL_MESSAGE)
{
	m_spLogger = platform->openFile(logFileName, true, false);
}

gLogger::~gLogger()
{
	finalize();
}

gLogger::eLoggerLevel gLogger::getLoggerMinLevel()
{
	return m_minLevel;
}

void gLogger::setLoggerMinLevel(eLoggerLevel level)
{
	m_minLevel = level;
}

void gLogger::logMessage(const gString& str )
{
	if (m_minLevel != gLogger::eLoggerLevel::GLL_MESSAGE)
		return;
	gString sTime;
	m_spLogger->print("<p class=\"lmessage\">%s\t", getTimeStr(sTime).c_str());
	m_spLogger->puts(str.c_str());
	m_spLogger->print("</p>\n");
}

void gLogger::logWarning(const gString& str)
{
	if ( m_minLevel <= gLogger::eLoggerLevel::GLL_MESSAGE )
		return;
	gString sTime;
	m_spLogger->print("<p class=\"lwarning\">%s\t", getTimeStr(sTime).c_str());
	m_spLogger->puts(str.c_str());
	m_spLogger->print("</p>\n");
}

void gLogger::logError(const gString& str)
{
	gString sTime;
	m_spLogger->print("<p class=\"lerror\">%s\t", getTimeStr(sTime).c_str());
	m_spLogger->puts(str.c_str());
	m_spLogger->print("</p>\n");
}

void gLogger::logMessage(const char* fmt, ...)
{
	if (m_minLevel != gLogger::eLoggerLevel::GLL_MESSAGE)
		return;
	gString sTime;
	m_spLogger->print("<p class=\"lmessage\">%s\t", getTimeStr(sTime).c_str());
	va_list argList;
	__crt_va_start(argList, fmt);
	(static_cast<gFile*>(m_spLogger.get()))->printVA(fmt, argList);
	__crt_va_end(argList);
	m_spLogger->print("</p>\n");
}

void gLogger::logWarning(const char* fmt, ...)
{
	if (m_minLevel <= gLogger::eLoggerLevel::GLL_MESSAGE)
		return;
	gString sTime;
	m_spLogger->print("<p class=\"lwarning\">%s\t", getTimeStr(sTime).c_str());
	va_list argList;
	__crt_va_start(argList, fmt);
	(static_cast<gFile*>(m_spLogger.get()))->printVA(fmt, argList);
	__crt_va_end(argList);
	m_spLogger->print("</p>\n");
}

void gLogger::logError(const char* fmt, ...)
{
	gString sTime;
	m_spLogger->print("<p class=\"lerror\">%s\t", getTimeStr(sTime).c_str());
	va_list argList;
	__crt_va_start(argList, fmt);
	(static_cast<gFile*>(m_spLogger.get()))->printVA(fmt, argList);
	__crt_va_end(argList);
	m_spLogger->print("</p>\n");
}

bool gLogger::initialize()
{
	if (!m_spLogger->isOpened())
		return false;

	m_spLogger->print("<!DOCTYPE=html>\n<html>\n<head>\n<title>3RD Game Engine Log File</title>\n<style>\n");
	m_spLogger->print(".lerror\n{\ncolor: #FF0707;\nborder:2px dotted;\nbackground:#c7c0c0;\n}\n.lwarning\n{\ncolor: #FFFF07;\nborder:2px dotted;\nbackground:#c7c7c0;\n}\n.lmessage\n{\ncolor: #07FF07;\nborder:2px dotted;\nbackground:#c0c7c0;\n}\n");
	m_spLogger->print("</style>\n</head>\n<body bgcolor=\"#c0c0c0\">\n");

	gString sTime;
	this->logMessage( gString("(") + getDateStr(sTime) + gString(")Starting 3RDE")  );

	return true;
}

bool gLogger::finalize()
{
	if (!m_spLogger->isOpened())
		return false;

	this->logMessage("Closing 3RDE");
	m_spLogger->print("</body>\n</html>");
	m_spLogger.reset(); // close log file
	return true;
}

gString& gLogger::getDateTimeStr( gString& str ) const
{
	std::time_t t = std::time(0);   // get time now
	std::tm now;
	localtime_s(&now, &t);

	str = std::to_string(now.tm_mday) +
		"." + std::to_string(now.tm_mon + 1) +
		"." + std::to_string(now.tm_year + 1900) +
		" " + std::to_string(now.tm_hour) +
		":" + std::to_string(now.tm_min) +
		":" + std::to_string(now.tm_sec);

	return str;
}

gString& gLogger::getDateStr(gString& str) const
{
	std::time_t t = std::time(0);   // get time now
	std::tm now;
	localtime_s(&now, &t);

	str = std::to_string(now.tm_mday) +
		"." + std::to_string(now.tm_mon + 1) +
		"." + std::to_string(now.tm_year + 1900);

	return str;
}

gString& gLogger::getTimeStr(gString& str) const
{
	std::time_t t = std::time(0);   // get time now
	std::tm now;
	localtime_s(&now, &t);

	str = std::to_string(now.tm_hour) +
	":" + std::to_string(now.tm_min) +
	":" + std::to_string(now.tm_sec);

	return str;
}

