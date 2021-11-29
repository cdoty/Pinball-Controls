#include "File.h"
#include "Functions.h"
#include "Log.h"
#include "INISettings.h"

Settings::Settings()
{
}

Settings::~Settings()
{
	close();
}

Settings::Ptr Settings::create(const std::string& _strConfigFile, bool _bForceLowerCase)
{
	INSTANCE(pSettings, Settings());
	
	if (false == pSettings->initialize(_strConfigFile, _bForceLowerCase))
	{
		pSettings.reset();
	}
	
	return	pSettings;
}

bool Settings::initialize(const std::string& _strConfigFile, bool _bForceLowerCase)
{
	m_pIniReader	= INIReader::create(_strConfigFile, _bForceLowerCase);

	if (nullptr == m_pIniReader)
	{
//		Log::instance()->logError("Unable to read config file %s", _strConfigFile.c_str());

		return	false;
	}

	m_strFilename	= _strConfigFile;

	return	true;
}

void Settings::close()
{
}

bool Settings::saveSettings()
{
	if (m_pIniReader != nullptr)
	{
		const std::map<std::string, std::string>& settingsMap	= m_pIniReader->GetMap();

		File::Ptr	pFile	= File::create();

		if (nullptr == pFile)
		{
			Log::instance()->logError("Unable to save settings file %s", m_strFilename.c_str());

			return	false;
		}

		if (false == pFile->create(m_strFilename))
		{
			Log::instance()->logError("Unable to save settings file %s", m_strFilename.c_str());

			return	false;
		}
		
		std::string	strCurrentSection	= "";

		for (auto const& ent1 : settingsMap)
		{
			std::string	strSection;
			std::string	strSetting;
			std::string	strKey		= ent1.first;
			std::string	strValue;
			
			std::size_t	found	= strKey.find('.');

			if (found != std::string::npos)
			{
				strSection	= strKey.substr(0, found);
				strSetting	= strKey.substr(found + 1);
			}

			else
			{
				strSection.clear();

				strSetting	= std::move(strKey);
			}

			if (strCurrentSection != strSection)
			{
				strCurrentSection	= strSection;
				
				if (false == strSection.empty())
				{
					strValue	= "[";
					strValue	+= strCurrentSection;
					strValue	+= "]";
					strValue	+= "\n";

					if (false == pFile->writeString(strValue))
					{
						return	false;
					}
				}
			}

			Utility::Functions::createINIString(strValue, strSetting, ent1.second);

			if (false == pFile->writeString(strValue))
			{
				return	false;
			}
		}
	
		pFile->close();
	}

	return	false;
}

bool Settings::getBoolSetting(const std::string& _strSection, const std::string& _strSetting, bool _bDefault) const
{
	if (m_pIniReader != nullptr)
	{
		return	m_pIniReader->GetBoolean(_strSection, _strSetting, _bDefault);
	}

	return	false;
}

void Settings::setBoolSetting(const std::string& _strSection, const std::string& _strSetting, bool _bValue = false)
{
	if (m_pIniReader != nullptr)
	{
		m_pIniReader->SetBoolean(_strSection, _strSetting, _bValue);
	}
}

uint32_t Settings::getHexSetting(const std::string& _strSection, const std::string& _strSetting, const std::string& _strDefault) const
{
	if (m_pIniReader != nullptr)
	{
		return	Utility::Functions::convertHexStringToUnsigned32(m_pIniReader->Get(_strSection, _strSetting, _strDefault));
	}

	return	0;
}

void Settings::setHexSetting(const std::string& _strSection, const std::string& _strSetting, uint32_t _value)
{
	if (m_pIniReader != nullptr)
	{
		char	szString[10];

		sprintf_s(szString, "%08X", _value);

		m_pIniReader->Set(_strSection, _strSetting, szString);
	}
}

int Settings::getIntSetting(const std::string& _strSection, const std::string& _strSetting, int _iDefault) const
{
	if (m_pIniReader != nullptr)
	{
		return	m_pIniReader->GetInteger(_strSection, _strSetting, _iDefault);
	}

	return	0;
}

void Settings::setIntSetting(const std::string& _strSection, const std::string& _strSetting, int _iValue)
{
	if (m_pIniReader != nullptr)
	{
		m_pIniReader->SetInteger(_strSection, _strSetting, _iValue);
	}
}

float Settings::getFloatSetting(const std::string& _strSection, const std::string& _strSetting, float _fDefault) const
{
	if (m_pIniReader != nullptr)
	{
		return	(float)m_pIniReader->GetReal(_strSection, _strSetting, _fDefault);
	}

	return	0.0f;
}

void Settings::setFloatSetting(const std::string& _strSection, const std::string& _strSetting, float _fValue)
{
	if (m_pIniReader != nullptr)
	{
		m_pIniReader->SetReal(_strSection, _strSetting, _fValue);
	}
}

std::string Settings::getStringSetting(const std::string& _strSection, const std::string& _strSetting, const std::string& _strDefault) const
{
	if (m_pIniReader != nullptr)
	{
		return	m_pIniReader->Get(_strSection, _strSetting, _strDefault);
	}

	return	"";
}

void Settings::setStringSetting(const std::string& _strSection, const std::string& _strSetting, const std::string& _strValue)
{
	if (m_pIniReader != nullptr)
	{
		m_pIniReader->Set(_strSection, _strSetting, _strValue);
	}
}

const std::vector<std::string>& Settings::getSections() const
{
	static std::vector<std::string>	tempValue;

	if (m_pIniReader != nullptr)
	{
		return	m_pIniReader->GetSections();
	}

	return	tempValue;
}
 
const std::map<std::string, std::string>& Settings::getMap() const
{
	static std::map<std::string, std::string>	tempValue;

	if (m_pIniReader != nullptr)
	{
		return	m_pIniReader->GetMap();
	}

	return	tempValue;
}
