#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <stdarg.h>

#include "File.h"
#include "Log.h"

Log::Ptr	Log::m_pInstance	= nullptr;	// Instance

static const char*	gsc_szDefaultLogFile	= "Log.html";	// Default log file
static const char*	gsc_szDefaultLogTitle	= "Log Output";	// Defauly log title

Log::Log()
{
	createLog();
}

Log::~Log()
{
	closeLog();
}

Log::Ptr Log::instance()
{
	if (nullptr == m_pInstance)
	{
		DEFINEINSTANCE(m_pInstance, Log());
	}
	
	return	m_pInstance;	
}

void Log::reset()
{
	if (m_pInstance != nullptr)
	{
		m_pInstance.reset();
	}
}

void Log::logMessage(const char* _szFormat, ...)
{
	va_list	args;
	char	szString[8192];

	va_start(args, _szFormat);

	vsprintf_s(szString, _szFormat, args);

	va_end(args);

	logString(LogMessage, szString);
}

void Log::logWarning(const char* _szFormat, ...)
{
	va_list	args;
	char	szString[8192];

	va_start(args, _szFormat);

	vsprintf_s(szString, _szFormat, args);

	va_end(args);

	logString(LogWarning, szString);
}

void Log::logError(const char* _szFormat, ...)
{
	static char	szString[8192];

	va_list	args;

	va_start(args, _szFormat);

	vsprintf_s(szString, _szFormat, args);

	va_end(args);

	logString(LogError, szString);
}

void Log::log(eLogType _eType, const char* _szFormat, ...)
{
	va_list	args;
	char	szString[8192];

	va_start(args, _szFormat);

	vsprintf_s(szString, _szFormat, args);

	va_end(args);

	logString(_eType, szString);
}

void Log::createLog()
{
	// Erase any existing log.
	File::deleteFile(gsc_szDefaultLogFile);

	writeHTMLStart();

	log(LogMessage, "Logging started");
}

void Log::closeLog()
{
	log(LogMessage, "Logging ended");
	
	writeHTMLEnd();

	// Log messages to log file.
	File::Ptr	pFile	= File::create();

	if (pFile != nullptr)
	{
		if (true == pFile->create(gsc_szDefaultLogFile))
		{
			size_t	t_c	= m_strLogStrings.size();
		
			for (size_t iLoop = 0; iLoop < t_c; ++iLoop)
			{
				pFile->writeString(m_strLogStrings[iLoop]);
			}
		}

		pFile->close();
	}
}

void Log::logString(eLogType _eType, const std::string& _strNewString)
{
//	logDateTime();

	switch (_eType)
	{
		case LogError:
			m_strLogStrings.push_back("<FONT COLOR=#FF0000>Error: ");

			break;

		case LogWarning:
			m_strLogStrings.push_back("<FONT COLOR=#FFFF00>Warning: ");

			break;

		case LogMessage:
		default:
			m_strLogStrings.push_back("<FONT COLOR=#00FF00>");

			break;
	}

	m_strLogStrings.push_back(_strNewString);
	m_strLogStrings.push_back("</FONT><BR>");

#ifdef _DEBUG
	OutputDebugStringA(_strNewString.c_str());
	OutputDebugStringA("\n");
#endif
}

void Log::logDateTime()
{
	char	szString[256];

	m_strLogStrings.push_back("<FONT COLOR=#FFFFFF>[");

#ifdef LOGDATE
	GetDateFormat(LOCALE_USER_DEFAULT, 0, NULL, "ddd',' MMM dd yy", szString, sizeof(szString));
	LogStrings.push_back(String);
	LogStrings.push_back(" ");
#endif

	GetTimeFormatA(LOCALE_USER_DEFAULT, 0, NULL, "hh':'mm':'ss tt", szString, sizeof(szString));

	m_strLogStrings.push_back(szString);
	m_strLogStrings.push_back("]</FONT> ");
}

void Log::writeHTMLStart()
{
	m_strLogStrings.push_back("<HTML><HEAD><TITLE>");
	m_strLogStrings.push_back(gsc_szDefaultLogTitle);
	m_strLogStrings.push_back("</TITLE></HEAD><BODY BGCOLOR=#00001F>");
}

void Log::writeHTMLEnd()
{
	m_strLogStrings.push_back("</BODY></HTML>\n");
}
