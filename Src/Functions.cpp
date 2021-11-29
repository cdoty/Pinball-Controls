#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <Windows.h>
#include <shellapi.h>
#include <sstream>

#include "File.h"
#include "Functions.h"
#include "Log.h"
#include "UTF8\utf8.h"

NAMESPACE(Utility)

void Functions::convertToLowerCase(std::string& _strString)
{
	if (false == _strString.empty())
	{
		std::transform(_strString.begin(), _strString.end(), _strString.begin(), ::tolower);
	}
}

void Functions::convertToLowerCase(std::wstring& _strString)
{
	if (false == _strString.empty())
	{
		std::transform(_strString.begin(), _strString.end(), _strString.begin(), ::tolower);
	}
}

void Functions::convertToUpperCase(std::string& _strString)
{
	if (false == _strString.empty())
	{
		std::transform(_strString.begin(), _strString.end(), _strString.begin(), ::toupper);
	}
}

void Functions::convertToUpperCase(std::wstring& _strString)
{
	if (false == _strString.empty())
	{
		std::transform(_strString.begin(), _strString.end(), _strString.begin(), ::toupper);
	}
}

int Functions::convertStringToInt(const std::string& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			return	std::stoi(_strString);
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertStringToInt: Error %s", _exception.what());
		}
	}

	return	0;
}

int Functions::convertStringToInt(const std::wstring& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			return	std::stoi(_strString, NULL, 10);
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertStringToInt: Error %s", _exception.what());
		}
	}

	return	0;
}

float Functions::convertStringToFloat(const std::string& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			return	(float)(std::stod(_strString, NULL));
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertStringToFloat: Error %s", _exception.what());
		}
	}

	return	0.0f;
}

float Functions::convertStringToFloat(const std::wstring& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			return	(float)(std::stod(_strString, NULL));
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertStringToFloat: Error %s", _exception.what());
		}
	}

	return	0.0f;
}

uint32_t Functions::convertHexStringToUnsigned32(const std::string& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			std::string	strTemp;
			int			iBase = 16;

			if (_strString.find("0x") == _strString.npos && _strString.find("0X") == _strString.npos)
			{
				strTemp	= "0x";
			}

			strTemp	+= _strString;

			return	std::stoul(strTemp, NULL, iBase);
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertHexStringToUnsigned32: Error %s", _exception.what());
		}
	}

	return	0;
}

uint32_t Functions::convertHexStringToUnsigned32(const std::wstring& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			std::wstring	strTemp;
			int				iBase = 16;

			if (_strString.find(L"0x") == _strString.npos && _strString.find(L"0X") == _strString.npos)
			{
				strTemp	= L"0x";
			}

			strTemp	+= _strString;

			return	std::stoul(strTemp, NULL, iBase);
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertHexStringToUnsigned32: Error %s", _exception.what());
		}
	}

	return	0;
}

uint64_t Functions::convertHexStringToUnsigned64(const std::string& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			std::string	strTemp;
			int			iBase = 16;

			if (_strString.find("0x") == _strString.npos && _strString.find("0X") == _strString.npos)
			{
				strTemp	= "0x";
			}

			strTemp	+= _strString;

			return	std::stoull(strTemp, NULL, iBase);
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertHexStringToUnsigned64: Error %s", _exception.what());
		}
	}

	return	0;
}

uint64_t Functions::convertHexStringToUnsigned64(const std::wstring& _strString)
{
	if (false == _strString.empty())
	{
		try
		{
			std::wstring	strTemp;
			int				iBase = 16;

			if (_strString.find(L"0x") == _strString.npos && _strString.find(L"0X") == _strString.npos)
			{
				strTemp	= L"0x";
			}

			strTemp	+= _strString;

			return	std::stoull(strTemp, NULL, iBase);
		}

		catch (const std::exception& _exception)
		{
			Log::instance()->logError("convertHexStringToUnsigned64: Error %s", _exception.what());
		}
	}

	return	0;
}

bool Functions::convertStringToBool(const std::string& _strString)
{
	std::string	strTemp;

	if (false == _strString.empty())
	{
		strTemp	= _strString;

		convertToLowerCase(strTemp);
	}

	return	"true" == strTemp;
}

bool Functions::convertStringToBool(const std::wstring& _strString)
{
	std::wstring	strTemp;

	if (false == _strString.empty())
	{
		strTemp	= _strString;

		convertToLowerCase(strTemp);
	}

	return	L"true" == strTemp;
}

std::string Functions::convertBoolToString(bool _bValue)
{
	return	true == _bValue ? "true" : "false";
}

std::wstring Functions::convertBoolToWideString(bool _bValue)
{
	return	true == _bValue ? L"true" : L"false";
}

void Functions::deleteString(std::string& _strSource, const std::string& _strDelete)
{
	if (false == _strSource.empty())
	{
		size_t	strPos	= _strSource.find(_strDelete);

		if (strPos != std::string::npos)
		{
			_strSource.erase(strPos, _strDelete.length());
		}
	}
}

void Functions::deleteString(std::wstring& _strSource, const std::wstring& _strDelete)
{
	if (false == _strSource.empty())
	{
		size_t	strPos	= _strSource.find(_strDelete);

		if (strPos != std::string::npos)
		{
			_strSource.erase(strPos, _strDelete.length());
		}
	}
}

std::wstring Functions::convertUTF8String(const std::string& _strString)
{
	std::wstring	strValue;

	if (false == _strString.empty())
	{
		utf8::utf8to16(_strString.begin(), _strString.end(), back_inserter(strValue));
	}

	return	strValue;
}

std::string Functions::convertToUTF8String(const std::wstring& _strString)
{
	std::string	strValue;

	if (false == _strString.empty())
	{
		utf8::utf16to8(_strString.begin(), _strString.end(), back_inserter(strValue));
	}

	return	strValue;
}

void Functions::createINIString(std::string& _strINI, const std::string& _strName, const std::string& _strValue, const std::string& _strPrefix)
{
	if (false == _strPrefix.empty())
	{
		_strINI	= _strPrefix;
		_strINI	+= _strName;
	}

	else
	{
		_strINI	= _strName;
	}

	_strINI	+= " = ";
	_strINI	+= _strValue;
	_strINI	+= "\n";
}

void Functions::createINIString(std::string& _strINI, const std::string& _strName, int _iValue, const std::string& _strPrefix)
{
	if (false == _strPrefix.empty())
	{
		_strINI	= _strPrefix;
		_strINI	+= _strName;
	}

	else
	{
		_strINI	= _strName;
	}

	_strINI	+= " = ";
	_strINI	+= std::to_string(_iValue);
	_strINI	+= "\n";
}

void Functions::createINIString(std::string& _strINI, const std::string& _strName, float _fValue, const std::string& _strPrefix, bool _bTruncate)
{
	if (false == _strPrefix.empty())
	{
		_strINI	= _strPrefix;
		_strINI	+= _strName;
	}

	else
	{
		_strINI	= _strName;
	}

	char	szString[32];

	if (true == _bTruncate)
	{
		sprintf_s(szString, "%0.2f", _fValue);
	}

	else
	{
		sprintf_s(szString, "%f", _fValue);
	}

	_strINI	+= " = ";
	_strINI	+= szString;
	_strINI	+= "\n";
}

void Functions::createINIString(std::string& _strINI, const std::string& _strName, bool _bValue, const std::string& _strPrefix)
{
	if (false == _strPrefix.empty())
	{
		_strINI	= _strPrefix;
		_strINI	+= _strName;
	}

	else
	{
		_strINI	= _strName;
	}

	_strINI	+= " = ";
	_strINI	+= (true == _bValue ? "true" : "false");
	_strINI	+= "\n";
}

ENDNAMESPACE
