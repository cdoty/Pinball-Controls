#pragma once

#include <string>

#include "Macros.h"

NAMESPACE(Utility)

class Functions
{
	public:
		// Convert string to lower case
		static void convertToLowerCase(std::string& _strString);
		static void convertToLowerCase(std::wstring& _strString);

		// Convert string to upper case
		static void convertToUpperCase(std::string& _strString);
		static void convertToUpperCase(std::wstring& _strString);

		// Convert int string to int
		static int convertStringToInt(const std::string& _strString);
		static int convertStringToInt(const std::wstring& _strString);

		// Convert float string to float
		static float convertStringToFloat(const std::string& _strString);
		static float convertStringToFloat(const std::wstring& _strString);

		// Convert hex string to 32 bit unsigned int
		static uint32_t convertHexStringToUnsigned32(const std::string& _strString);
		static uint32_t convertHexStringToUnsigned32(const std::wstring& _strString);

		// Convert hex string to 64 bit unsigned int
		static uint64_t convertHexStringToUnsigned64(const std::string& _strString);
		static uint64_t convertHexStringToUnsigned64(const std::wstring& _strString);

		// Convert string to bool
		static bool convertStringToBool(const std::string& _strString);
		static bool convertStringToBool(const std::wstring& _strString);

		// Convert bool to string
		static std::string convertBoolToString(bool _bValue);

		// Convert bool to wstring
		static std::wstring convertBoolToWideString(bool _bValue);

		// Delete string from string
		static void deleteString(std::string& _strSource, const std::string& _strDelete);
		static void deleteString(std::wstring& _strSource, const std::wstring& _strDelete);

		// Convert UTF8 string to wide string
		static std::wstring convertUTF8String(const std::string& _strString);

		// Convert wide string to UTF8 string
		static std::string convertToUTF8String(const std::wstring& _strString);

		// INI String
		static void createINIString(std::string& _strINI, const std::string& _strName, const std::string& _strValue, const std::string& _strPrefix = "");
		static void createINIString(std::string& _strINI, const std::string& _strName, int _iValue, const std::string& _strPrefix = "");
		static void createINIString(std::string& _strINI, const std::string& _strName, float _fValue, const std::string& _strPrefix = "", bool _bTruncate = true);
		static void createINIString(std::string& _strINI, const std::string& _strName, bool _bValue, const std::string& _strPrefix = "");
};

ENDNAMESPACE
