#pragma once

#include <IniReader\IniReader.h>

#include "Macros.h"

class Settings
{
	public:
		PTR(Settings)

		// Destructor
		~Settings();

		// Create
		static Ptr create(const std::string& _strConfigFile, bool _bForceLowerCase = true);
		
		// Initialize
		bool initialize(const std::string& _strConfigFile, bool _bForceLowerCase);
		
		// Close
		void close();
		
		// Save settings
		bool saveSettings();

		// Get bool setting
		bool getBoolSetting(const std::string& _strSection, const std::string& _strSetting, bool _bDefault = false) const;
		
		// Set bool setting
		void setBoolSetting(const std::string& _strSection, const std::string& _strSetting, bool _bValue);
		
		// Get hex setting
		uint32_t getHexSetting(const std::string& _strSection, const std::string& _strSetting, const std::string& _strDefault = "000000") const;
		
		// Set hex setting
		void setHexSetting(const std::string& _strSection, const std::string& _strSetting, uint32_t _value);

		// Get int setting
		int getIntSetting(const std::string& _strSection, const std::string& _strSetting, int _iDefault = 0) const;

		// Set int setting
		void setIntSetting(const std::string& _strSection, const std::string& _strSetting, int _iValue);

		// Get float setting
		float getFloatSetting(const std::string& _strSection, const std::string& _strSetting, float _fDefault = 0.0f) const;

		// Set float setting
		void setFloatSetting(const std::string& _strSection, const std::string& _strSetting, float _fValue);

		// Get string setting
		std::string getStringSetting(const std::string& _strSection, const std::string& _strSetting, const std::string& _strDefault = "") const;

		// Set string setting
		void setStringSetting(const std::string& _strSection, const std::string& _strSetting, const std::string& _strValue);

		// Get sections
		const std::vector<std::string>& getSections() const;

		// Get map
		const std::map<std::string, std::string>& getMap() const;

	private:
		INIReader::Ptr	m_pIniReader;	// INI Reader
		std::string		m_strFilename;	// Filename

		// Constructor
		Settings();
};
