// inih and INIReader are released under the New BSD license (see LICENSE.txt).
// Go to the project home page for more info:
//
// http://code.google.com/p/inih/

#ifndef __INIREADER_H__
#define __INIREADER_H__

#include <string>

#include "Macros.h"

// Read an INI file into easy-to-access name/value pairs. (Note that I've gone
// for simplicity here rather than speed, but it should be pretty decent.)
class INIReader
{
public:
	PTR(INIReader)

	// Create
	static Ptr create(const std::string& _filename, bool _bLowerCase = true);

	// Initialize
	bool initialize(const std::string& _filename);

	// Return the result of ini_parse(), i.e., 0 on success, line number of
	// first error on parse error, or -1 on file open error.
	int ParseError() const;

	// Get a string value from INI file, returning default_value if not found.
	const std::string& Get(const std::string& _section, const std::string& _name, const std::string& _defaultValue);

	// Set a string
	void Set(const std::string& _section, const std::string& _name, const std::string& _value);

	// Get an integer (long) value from INI file, returning default_value if
	// not found or not a valid integer (decimal "1234", "-1234", or hex "0x4d2").
	long GetInteger(const std::string& _section, const std::string& _name, long _defaultValue);
	
	// Set an integer
	void SetInteger(const std::string& _section, const std::string& _name, long _value);

	// Get a real (floating point double) value from INI file, returning
	// default_value if not found or not a valid floating point value
	// according to strtod().
	double GetReal(const std::string& _section, const std::string& _name, double _defaultValue);

	// Set real
	void SetReal(const std::string& _section, const std::string& _name, double _value);

	// Get a boolean value from INI file, returning default_value if not found or if
	// not a valid true/false value. Valid true values are "true", "yes", "on", "1",
	// and valid false values are "false", "no", "off", "0" (not case sensitive).
	bool GetBoolean(const std::string& _section, const std::string& _name, bool _defaultValue);

	// Set boolean
	void SetBoolean(const std::string& _section, const std::string& _name, bool _value);

	// Get sections
	const std::vector<std::string>& GetSections() const {return m_vecSections;}

	// Get map
	std::map<std::string, std::string>& GetMap() {return m_values;}

	// Make key
	std::string MakeKey(const std::string& _section, const std::string& _name);

	// Make key
	static std::string MakeKey(const std::string& _section, const std::string& _name, bool _bLowerCase);

private:
	int									m_error;		// Error
	std::map<std::string, std::string>	m_values;		// Values
	std::vector<std::string>			m_vecSections;	// Sections
	bool								m_lowerCase;	// User lower case sections?

	// Construct INIReader and parse given filename. See ini.h for more info about the parsing.
	INIReader(bool _bLowerCase);

	static int ValueHandler(void* _pInterface, const char* _section, const char* _name, const char* _value);
};

#endif  // __INIREADER_H__
