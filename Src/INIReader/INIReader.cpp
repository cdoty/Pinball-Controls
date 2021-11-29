#include <algorithm>

#include "ini.h"
#include "INIReader.h"
#include "Log.h"

INIReader::INIReader(bool _bLowerCase)	:
	m_error(0),
	m_lowerCase(_bLowerCase)
{
}

INIReader::Ptr INIReader::create(const std::string& _filename, bool _bLowerCase)
{
	INSTANCE(pIniReader, INIReader(_bLowerCase));

	if (false == pIniReader->initialize(_filename))
	{
		pIniReader.reset();
	}

	return	pIniReader;
}

bool INIReader::initialize(const std::string& _filename)
{
	m_error	= ini_parse(_filename.c_str(), ValueHandler, this);

	if (m_error != 0)
	{
		return	false;
	}

	return	true;
}

int INIReader::ParseError() const
{
	return	m_error;
}

const std::string& INIReader::Get(const std::string& _section, const std::string& _name, const std::string& _defaultValue)
{
	const std::string	strKey	= MakeKey(_section, _name);

	return	m_values.count(strKey) ? m_values[strKey] : _defaultValue;
}

void INIReader::Set(const std::string& _section, const std::string& _name, const std::string& _value)
{
	const std::string strKey	= MakeKey(_section, _name);

	m_values[strKey]	= _value;
}

long INIReader::GetInteger(const std::string& _section, const std::string& _name, long _defaultValue)
{
	const std::string&	strVal	= Get(_section, _name, "");
	
	long	iValue	= _defaultValue;
	
	if (false == strVal.empty())
	{
		try
		{
			// This parses "1234" (decimal) and also "0x4D2" (hex)
			iValue	= std::stol(strVal);
		}
	
		catch (const std::exception& _exception)
		{
			Log::instance()->logError("GetInteger: Error %s", _exception.what());
		}
	}

	return	iValue;
}

void INIReader::SetInteger(const std::string& _section, const std::string& _name, long _value)
{
	std::string	strNumber	= std::to_string(_value);

	Set(_section, _name, strNumber);
}

double INIReader::GetReal(const std::string& _section, const std::string& _name, double _defaultValue)
{
	const std::string&	strVal	= Get(_section, _name, "");
	
	double	fValue	= _defaultValue;

	if (false == strVal.empty())
	{
		try
		{
			// This parses "1234" (decimal) and also "0x4D2" (hex)
			fValue	= std::stod(strVal);
		}
	
		catch (const std::exception& _exception)
		{
			Log::instance()->logError("GetReal: Error %s", _exception.what());
		}
	}

	return	fValue;
}

void INIReader::SetReal(const std::string& _section, const std::string& _name, double _value)
{
	std::string	strNumber	= std::to_string(_value);

	Set(_section, _name, strNumber);
}

bool INIReader::GetBoolean(const std::string& _section, const std::string& _name, bool _defaultValue)
{
	const std::string& strVal	= Get(_section, _name, "");
	
	if (false == strVal.empty())
	{
		std::string	lowerCase	= strVal;

		// Convert to lower case to make string comparisons case-insensitive
		std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), ::tolower);

		if ("true" == lowerCase || "yes" == lowerCase || "on" == lowerCase || "1" == lowerCase)
		{
			return	true;
		}

		else if ("false" == lowerCase || "no" == lowerCase || "off" == lowerCase || "0" == lowerCase)
		{
			return	false;
		}
	}

	return	_defaultValue;
}

void INIReader::SetBoolean(const std::string& _section, const std::string& _name, bool _value)
{
	Set(_section, _name, true == _value ? "true" : "false");
}

std::string INIReader::MakeKey(const std::string& _section, const std::string& _name)
{
	return	MakeKey(_section, _name, m_lowerCase);
}

std::string INIReader::MakeKey(const std::string& _section, const std::string& _name, bool _bLowerCase)
{
    std::string strKey;
	
	if (_section.length() > 0)
	{
		strKey	+= _section + ".";
	}

	strKey	+= _name;

    if (true == _bLowerCase)
	{
		// Convert to lower case to make section/name lookups case-insensitive
		std::transform(strKey.begin(), strKey.end(), strKey.begin(), ::tolower);
	}

    return	strKey;
}

int INIReader::ValueHandler(void* _pInterface, const char* _section, const char* _name, const char* _value)
{
	INIReader* reader	= (INIReader*)_pInterface;

	const std::string strKey	= MakeKey(_section, _name, reader->m_lowerCase);
    
	if (reader->m_values[strKey].size() > 0)
	{
		reader->m_values[strKey]	+= "\n";
	}

	size_t	t_c	= reader->m_vecSections.size();

	reader->m_values[strKey]	+= _value;

	for (size_t iLoop = 0; iLoop < t_c; ++iLoop)
	{
		if (_section == reader->m_vecSections[iLoop])
		{
			return	1;
		}
	}

	reader->m_vecSections.push_back(_section);
	    
	return	1;
}
