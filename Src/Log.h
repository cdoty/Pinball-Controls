// Log - Message logging
//
// © 2012 Raster Software. All rights reserved.

#pragma once

#include "Macros.h"

class Log
{
	public:
		PTR(Log)

		// Log types
		enum eLogType
		{
			LogMessage	= 0,	// Log a message
			LogWarning,			// Log a warning
			LogError			// Log an error
		};

		// Destructor
		~Log();

		// Log message
		void logMessage(const char* _szFormat, ...);

		// Log warning
		void logWarning(const char* _szFormat, ...);

		// Log error
		void logError(const char* _szFormat, ...);

		// Get instance
		static Ptr instance();
		
		// Reset
		static void reset();

	private:
		static Ptr	m_pInstance;	// Log instance

		std::vector<std::string>	m_strLogStrings;	// Log strings
																
		// Constructor
		Log();
		
		// Log - sprintf wrapper
		void log(eLogType _eType, const char* _szFormat, ...);

		// Create the log
		void createLog();

		// Close the log
		void closeLog();

		// Log string
		void logString(eLogType _eType, const std::string& _strNewString);

		// Log current date and time
		void logDateTime();

		// Write HTML start
		void writeHTMLStart();

		// Write HTML end
		void writeHTMLEnd();
};
