#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif

#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <Windows.h>
#include <direct.h>
#include <shellapi.h>
#include <shlwapi.h>
#include <tchar.h>

#include "File.h"
#include "Functions.h"
#include "Log.h"

#pragma comment(lib, "Shlwapi.lib")

File::File() :
	m_pHandle(NULL),
	m_bBinary(false)
{
}

File::~File()
{
	close();
}

File::Ptr File::create()
{
	INSTANCE(pFile, File());

	return	pFile;
}

bool File::create(const std::string& _strFilename, bool _bBinary)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	fopen_s(&m_pHandle, _strFilename.c_str(), true == _bBinary ? "wb" : "wt");
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::create(const std::wstring& _strFilename, bool _bBinary)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	_wfopen_s(&m_pHandle, _strFilename.c_str(), true == _bBinary ? L"wb" : L"wt");
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::createSharedFile(const std::string& _strFilename, int _iShareMode, bool _bBinary, bool _bAppend)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
		
	const char*	szFileMode;

	if (true == _bAppend)
	{
		szFileMode	= true == _bBinary ? "ab" : "at";
	}

	else
	{
		szFileMode	= true == _bBinary ? "wb" : "wt";
	}

	m_pHandle	= _fsopen(_strFilename.c_str(), szFileMode, _iShareMode);
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::createSharedFile(const std::wstring& _strFilename, int _iShareMode, bool _bBinary, bool _bAppend)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	const wchar_t*	szFileMode;

	if (true == _bAppend)
	{
		szFileMode	= true == _bBinary ? L"ab" : L"at";
	}

	else
	{
		szFileMode	= true == _bBinary ? L"wb" : L"wt";
	}

	m_pHandle	= _wfsopen(_strFilename.c_str(), szFileMode, _iShareMode);
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::open(const std::string& _strFilename, bool _bBinary)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	fopen_s(&m_pHandle, _strFilename.c_str(), true == _bBinary ? "rb" : "rt");
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::open(const std::wstring& _strFilename, bool _bBinary)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	_wfopen_s(&m_pHandle, _strFilename.c_str(), true == _bBinary ? L"rb" : L"rt");
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::append(const std::string& _strFilename, bool _bBinary)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	fopen_s(&m_pHandle, _strFilename.c_str(), true == _bBinary ? "ab" : "at");
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::append(const std::wstring& _strFilename, bool _bBinary)
{
	// Close a previously opened file.
	if (m_pHandle != NULL)
	{
		close();
	}
	
	_wfopen_s(&m_pHandle, _strFilename.c_str(), true == _bBinary ? L"ab" : L"at");
	
	if (NULL == m_pHandle)
	{
		return	false;
	}
	
	m_bBinary	= _bBinary;

	return	true;
}

bool File::close()
{
	if (m_pHandle != NULL)
	{
		fclose(m_pHandle);
	
		m_pHandle	= NULL;
	}
	
	return	true;
}

bool File::read8Bit(char& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(char), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::readUnsigned8Bit(uint8_t& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(uint8_t), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::read16Bit(short& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(short), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::readUnsigned16Bit(uint16_t& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(uint16_t), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::read32Bit(int& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(int), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::readUnsigned32Bit(uint32_t& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(uint32_t), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::read64Bit(int64_t& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(int64_t), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::readUnsigned64Bit(uint64_t& _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(&_iValue, sizeof(uint64_t), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::readString(std::string& _strValue)
{
	_strValue.clear();

	while (true)
	{
		char	value;

		if (false == read8Bit(value))
		{
			return	false;
		}
		
		if (0 == value)
		{
			break;
		}

		_strValue	+= value;
	}

	return	true;
}

bool File::readString(std::wstring& _strValue)
{
	_strValue.clear();

	while (true)
	{
		char	value;

		if (false == read8Bit(value))
		{
			return	false;
		}
		
		if (0 == value)
		{
			break;
		}

		_strValue	+= value;
	}

	return	true;
}

bool File::readLine(std::string& _strLine, int _iMaxSize) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	_strLine.clear();

	DEFINESHAREDBUFFER(pBuffer, char, _iMaxSize);
							
	if (nullptr == pBuffer || NULL == fgets(pBuffer.get(), _iMaxSize, m_pHandle))
	{
		return	false;
	}

	_strLine	= pBuffer.get();

	size_t	found	= _strLine.find('\n');
	
	if (found != std::string::npos)
	{
		_strLine.erase(found, 1);
	}

	return	true;
}

bool File::readLine(std::wstring& _strLine, int _iMaxSize) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	_strLine.clear();

	DEFINESHAREDBUFFER(pBuffer, wchar_t, _iMaxSize);
							
	if (nullptr == pBuffer || NULL == fgetws(pBuffer.get(), _iMaxSize, m_pHandle))
	{
		return	false;
	}

	_strLine	= pBuffer.get();

	size_t	found	= _strLine.find(L'\n');
	
	if (found != std::wstring::npos)
	{
		_strLine.erase(found, 1);
	}

	return	true;
}

bool File::readLineUTF8(std::string& _strLine, int _iMaxSize) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	_strLine.clear();

	DEFINESHAREDBUFFER(pBuffer, wchar_t, _iMaxSize);
							
	if (nullptr == pBuffer || NULL == fgetws(pBuffer.get(), _iMaxSize, m_pHandle))
	{
		return	false;
	}

	std::wstring	strBuffer	= pBuffer.get();
	std::string		strValue	= Utility::Functions::convertToUTF8String(strBuffer);

	_strLine	= strValue;

	size_t	found	= _strLine.find('\n');
	
	if (found != std::string::npos)
	{
		_strLine.erase(found, 1);
	}

	return	true;
}

bool File::readBuffer(void* _pBuffer, int _iBufferSize, int _iElementSize) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fread(_pBuffer, (size_t)_iElementSize, (size_t)_iBufferSize, m_pHandle) != (size_t)_iBufferSize)
	{
		return	false;
	}
	
	return	true;
}

bool File::readBuffer(uint8_t** _pBuffer, int _iBufferSize, int _iElementSize) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (NULL == *_pBuffer)
	{
		*_pBuffer	= new (std::nothrow) uint8_t[(size_t)_iBufferSize];

		if (nullptr == *_pBuffer)
		{
			return	false;
		}
	}
	
	return	readBuffer(*_pBuffer, _iBufferSize, _iElementSize);
}

bool File::write8Bit(uint8_t _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fwrite(&_iValue, sizeof(uint8_t), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::write32Bit(int _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fwrite(&_iValue, sizeof(int), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::write64Bit(int64_t _iValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fwrite(&_iValue, sizeof(int64_t), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::writeFloat(float _fValue) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fwrite(&_fValue, sizeof(float), 1, m_pHandle) != 1)
	{
		return	false;
	}
	
	return	true;
}

bool File::writeBuffer(const uint8_t* _pBuffer, int iBufferSize) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (fwrite(_pBuffer, 1, (size_t)iBufferSize, m_pHandle) != (size_t)iBufferSize)
	{
		return	false;
	}
	
	return	true;
}

bool File::writeLine(const std::string& _strLine) const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	if (false == writeString(_strLine))
	{
		return	false;
	}

	if (false == writeString("\n"))
	{
		return	false;
	}

	return	true;
}

bool File::writeString(const std::string& _strString) const
{
	if (false == _strString.empty())
	{
		size_t	iBufferSize;
		
		if (true == m_bBinary)
		{
			iBufferSize	= _strString.length() + 1;
		}
		
		else
		{
			iBufferSize	= _strString.length();
		}
		
		return	writeBuffer((const uint8_t*)_strString.c_str(), (int)iBufferSize);
	}
	
	return	true;
}

bool File::writeString(const std::wstring& _strString) const
{
	if (false == _strString.empty())
	{
		size_t	iBufferSize;
		
		if (true == m_bBinary)
		{
			iBufferSize	= _strString.length() + 1;
		}
		
		else
		{
			iBufferSize	= _strString.length();
		}
		
		iBufferSize	*= sizeof(wchar_t);

		return	writeBuffer((const uint8_t*)_strString.c_str(), (int)iBufferSize);
	}
	
	return	true;
}

bool File::writeStringUTF8(const std::wstring& _strString) const
{
	if (false == _strString.empty())
	{
		std::string	strValue	= Utility::Functions::convertToUTF8String(_strString);

		size_t	iBufferSize;
		
		if (true == m_bBinary)
		{
			iBufferSize	= strValue.length() + 1;
		}
		
		else
		{
			iBufferSize	= strValue.length();
		}
		
		return	writeBuffer((const uint8_t*)strValue.c_str(), (int)iBufferSize);
	}
	
	return	true;
}

bool File::seek(int _iOffset, SeekPoint _eSeekPoint) const
{
	int	iOrigin;
	
	switch (_eSeekPoint)
	{
		case SeekFromStart:
			iOrigin	= SEEK_SET;
			
			break;
			
		case SeekFromCurrent:
			iOrigin	= SEEK_CUR;
			
			break;
			
		case SeekFromEnd:
			iOrigin	= SEEK_END;
			
			break;
	
		default:
			return	false;
	}
	
	if (fseek(m_pHandle, _iOffset, iOrigin) != 0)
	{
		return	false;
	}
	
	return	true;
}

int File::getPosition() const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	0;
	}

	return	ftell(m_pHandle);
}

int File::getLength() const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	0;
	}

	int	iCurrentPosition	= ftell(m_pHandle);
	
	if (-1 == iCurrentPosition || fseek(m_pHandle, 0, SEEK_END) != 0)
	{
		return	-1;
	}
	
	int	iLength	= ftell(m_pHandle);
	
	fseek(m_pHandle, iCurrentPosition, SEEK_SET);
	
	return	iLength;
}

bool File::endOfFile() const
{
	if (NULL == m_pHandle)
	{
		Log::instance()->logError("FILE handle is invalid");

		return	false;
	}

	return	feof(m_pHandle) != 0;
}

bool File::fileExists(const std::string& _strFilename, bool _bIgnoreDirectories)
{
	DWORD	uFileAttributes	= GetFileAttributesA(_strFilename.c_str());
	
	if (uFileAttributes != INVALID_FILE_ATTRIBUTES)
	{
		if (FILE_ATTRIBUTE_DIRECTORY == (uFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && true == _bIgnoreDirectories)
		{
			return	false;
		}
		
		return	true;
	}
	
	return	false;
}

bool File::fileExists(const std::wstring& _strFilename, bool _bIgnoreDirectories)
{
	DWORD	uFileAttributes	= GetFileAttributesW(_strFilename.c_str());
	
	if (uFileAttributes != INVALID_FILE_ATTRIBUTES)
	{
		if (FILE_ATTRIBUTE_DIRECTORY == (uFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && true == _bIgnoreDirectories)
		{
			return	false;
		}
		
		return	true;
	}
	
	return	false;
}

bool File::directoryExists(const std::string& _strFilename)
{
	DWORD	uFileAttributes	= GetFileAttributesA(_strFilename.c_str());
	
	if (uFileAttributes != INVALID_FILE_ATTRIBUTES && FILE_ATTRIBUTE_DIRECTORY == (uFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		return	true;
	}
	
	return	false;
}

bool File::directoryExists(const std::wstring& _strFilename)
{
	DWORD	uFileAttributes	= GetFileAttributesW(_strFilename.c_str());
	
	if (uFileAttributes != INVALID_FILE_ATTRIBUTES && FILE_ATTRIBUTE_DIRECTORY == (uFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		return	true;
	}
	
	return	false;
}

bool File::copyFile(const std::string& _strSrc, const std::string& _strDest)
{
	if (FALSE == CopyFileA(_strSrc.c_str(), _strDest.c_str(), FALSE))
	{
		return	false;
	}

	return	true;
}

bool File::copyFile(const std::wstring& _strSrc, const std::wstring& _strDest)
{
	if (FALSE == CopyFileW(_strSrc.c_str(), _strDest.c_str(), FALSE))
	{
		return	false;
	}

	return	true;
}

bool File::deleteFile(const std::string& _strFilename)
{
	if (FALSE == DeleteFileA(_strFilename.c_str()))
	{
		return	false;
	}
	
	return	true;
}

bool File::deleteFile(const std::wstring& _strFilename)
{
	if (FALSE == DeleteFileW(_strFilename.c_str()))
	{
		return	false;
	}
	
	return	true;
}

bool File::deleteMultipleFiles(const std::string& _strFilename)
{
	WIN32_FIND_DATAA	findData;
	DWORD				uFileFlags	= FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_REPARSE_POINT | FILE_ATTRIBUTE_OFFLINE; 

	std::string	strDirectory	= getDirectoryFromPath(_strFilename);

	HANDLE	hFindFile	= FindFirstFileA(_strFilename.c_str(), &findData);
	
	if (INVALID_HANDLE_VALUE == hFindFile)
	{
		return	false;
	}
	
	do 
	{
		if (0 == (uFileFlags & findData.dwFileAttributes))
		{
			char	szFilename[FILENAME_MAX];

			_snprintf_s(szFilename, FILENAME_MAX, "%s\\%s", strDirectory.c_str(), findData.cFileName);

			if (false == deleteFile(szFilename))
			{
				FindClose(hFindFile);
		
				return	false;
			}
		}
	} while (FindNextFileA(hFindFile, &findData) != FALSE);
	
	FindClose(hFindFile);
		
	return	true;
}

bool File::deleteMultipleFiles(const std::wstring& _strFilename)
{
	WIN32_FIND_DATAW	findData;
	DWORD				uFileFlags	= FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_REPARSE_POINT | FILE_ATTRIBUTE_OFFLINE; 

	std::wstring	strDirectory	= getDirectoryFromPath(_strFilename);

	HANDLE	hFindFile	= FindFirstFileW(_strFilename.c_str(), &findData);
	
	if (INVALID_HANDLE_VALUE == hFindFile)
	{
		return	false;
	}
	
	do 
	{
		if (0 == (uFileFlags & findData.dwFileAttributes))
		{
			wchar_t	szFilename[FILENAME_MAX];

			_snwprintf_s(szFilename, FILENAME_MAX, L"%s\\%s", strDirectory.c_str(), findData.cFileName);

			if (false == deleteFile(szFilename))
			{
				FindClose(hFindFile);
		
				return	false;
			}
		}
	} while (FindNextFileW(hFindFile, &findData) != FALSE);
	
	FindClose(hFindFile);
		
	return	true;
}

bool File::deleteDirectory(const std::string& _strDirectory)
{
	if (0 == RemoveDirectoryA(_strDirectory.c_str()))
	{
		return	false;
	}

	return	true;
}

bool File::deleteDirectory(const std::wstring& _strDirectory)
{
	if (0 == RemoveDirectoryW(_strDirectory.c_str()))
	{
		return	false;
	}

	return	true;
}

bool File::recursivelyDeleteDirectory(const std::string& _strDirectory)
{
	SHFILEOPSTRUCTA	fileOps	= {0};

	memset(&fileOps, 0, sizeof(SHFILEOPSTRUCTA));

	std::string	strFrom;

	strFrom	= _strDirectory;

	strFrom.push_back('\0');
	strFrom.push_back('\0');

	std::string	strTo;

	strTo.push_back('\0');
	strTo.push_back('\0');

	fileOps.pFrom	= strFrom.c_str();
	fileOps.pTo		= strTo.c_str();
	fileOps.hwnd	= NULL;
	fileOps.fFlags	= FOF_NO_UI;
	fileOps.wFunc	= FO_DELETE;

	return	0 == SHFileOperationA(&fileOps);
}

bool File::recursivelyDeleteDirectory(const std::wstring& _strDirectory)
{
	SHFILEOPSTRUCTW	fileOps	= {0};

	memset(&fileOps, 0, sizeof(SHFILEOPSTRUCTW));

	std::wstring	strTo;

	strTo	= _strDirectory;

	strTo.push_back('\0');
	strTo.push_back('\0');

	std::wstring	strFrom;

	strFrom.push_back(L'\0');
	strFrom.push_back(L'\0');

	fileOps.pFrom	= strFrom.c_str();
	fileOps.pTo		= strTo.c_str();
	fileOps.hwnd	= NULL;
	fileOps.fFlags	= FOF_NO_UI;
	fileOps.wFunc	= FO_DELETE;

	return	0 == SHFileOperationW(&fileOps);
}

bool File::createDirectory(const std::string& _strFilename)
{
	if (0 == CreateDirectoryA(_strFilename.c_str(), NULL))
	{
		return	false;
	}

	return	true;
}

bool File::createDirectory(const std::wstring& _strFilename)
{
	if (0 == CreateDirectoryW(_strFilename.c_str(), NULL))
	{
		return	false;
	}

	return	true;
}

std::string File::getCurrentDirectory()
{
	char	szCurrentDirectory[FILENAME_MAX];

	memset(szCurrentDirectory, 0, FILENAME_MAX);

	GetCurrentDirectoryA(FILENAME_MAX, szCurrentDirectory);
	
	std::string	strDirectory	= szCurrentDirectory;

	addDirectorySeparator(strDirectory);

	return	strDirectory;
}

std::wstring File::getCurrentDirectoryWideString()
{
	wchar_t	szCurrentDirectory[FILENAME_MAX];

	memset(szCurrentDirectory, 0, FILENAME_MAX * sizeof(wchar_t));

	GetCurrentDirectoryW(FILENAME_MAX, szCurrentDirectory);
	
	std::wstring	strDirectory	= szCurrentDirectory;

	addDirectorySeparator(strDirectory);

	return	strDirectory;
}

bool File::setCurrentDirectory(const std::string& _strDirectory)
{
	return	FALSE != SetCurrentDirectoryA(_strDirectory.c_str());
}

bool File::setCurrentDirectory(const std::wstring& _strDirectory)
{
	return	FALSE != SetCurrentDirectoryW(_strDirectory.c_str());
}

bool File::setWorkingDirectory()
{
	TCHAR	szFilename[FILENAME_MAX];
	TCHAR	szDrive[FILENAME_MAX];
	TCHAR	szDir[FILENAME_MAX];
	
	if (FALSE == GetModuleFileName(NULL, szFilename, FILENAME_MAX))
	{
		Log::instance()->logError("Unable to get module filename");

		return	false;
	}
	
	_tsplitpath_s(szFilename, szDrive, FILENAME_MAX, szDir, FILENAME_MAX, NULL, 0, NULL, 0);
	
	std::basic_string<TCHAR>	strDirectory;
	
	strDirectory	= szDrive;
	strDirectory	+= szDir;

	if (FALSE == SetCurrentDirectory(strDirectory.c_str()))
	{
		Log::instance()->logError("Unable to set working directory");

		return	false;
	}

	return	true;
}

std::string File::getProgramDirectory()
{
	char	szDirectory[FILENAME_MAX];

	GetModuleFileNameA(GetModuleHandle(NULL), szDirectory, FILENAME_MAX);

	return	getDirectoryFromPath(szDirectory);
}

std::wstring File::getProgramDirectoryWideString()
{
	wchar_t	szDirectory[FILENAME_MAX];

	GetModuleFileNameW(GetModuleHandle(NULL), szDirectory, FILENAME_MAX);

	return	getDirectoryFromPath(szDirectory);
}

std::string File::getFilenameFromPath(const std::string& _strPath)
{
	char	szFilename[FILENAME_MAX];

	_splitpath_s(_strPath.c_str(), NULL, 0, NULL, 0, szFilename, FILENAME_MAX, NULL, 0);

	return	szFilename;
}

std::wstring File::getFilenameFromPath(const std::wstring& _strPath)
{
	wchar_t	szFilename[FILENAME_MAX];

	_wsplitpath_s(_strPath.c_str(), NULL, 0, NULL, 0, szFilename, FILENAME_MAX, NULL, 0);

	return	szFilename;
}

std::string File::getExtensionFromPath(const std::string& _strPath)
{
	char	szExtension[FILENAME_MAX];

	_splitpath_s(_strPath.c_str(), NULL, 0, NULL, 0, NULL, 0, szExtension, FILENAME_MAX);

	return	szExtension;
}

std::wstring File::getExtensionFromPath(const std::wstring& _strPath)
{
	wchar_t	szExtension[FILENAME_MAX];

	_wsplitpath_s(_strPath.c_str(), NULL, 0, NULL, 0, NULL, 0, szExtension, FILENAME_MAX);

	return	szExtension;
}

std::string File::getFullFilenameFromPath(const std::string& _strPath)
{
	char	szFilename[FILENAME_MAX];
	char	szExtension[FILENAME_MAX];

	_splitpath_s(_strPath.c_str(), NULL, 0, NULL, 0, szFilename, FILENAME_MAX, szExtension, FILENAME_MAX);

	std::string	strFilename;

	strFilename	= szFilename;
	strFilename	+= szExtension;
	
	return	strFilename;
}

std::string File::getDirectoryFromPath(const std::string& _strPath)
{
	char	szDrive[FILENAME_MAX];
	char	szDir[FILENAME_MAX];

	_splitpath_s(_strPath.c_str(), szDrive, FILENAME_MAX, szDir, FILENAME_MAX, NULL, 0, NULL, 0);

	std::string	strDirectory;

	strDirectory	= szDrive;
	strDirectory	+= szDir;
	
	return	strDirectory;
}

std::wstring File::getDirectoryFromPath(const std::wstring& _strPath)
{
	wchar_t	szDrive[FILENAME_MAX];
	wchar_t	szDir[FILENAME_MAX];

	_wsplitpath_s(_strPath.c_str(), szDrive, FILENAME_MAX, szDir, FILENAME_MAX, NULL, 0, NULL, 0);

	std::wstring	strDirectory;

	strDirectory	= szDrive;
	strDirectory	+= szDir;
	
	return	strDirectory;
}

std::string File::getDateTime(const std::string& _strPath)
{
	HANDLE	hFile	= CreateFileA(_strPath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		FILETIME	lastWriteTime;

		if (GetFileTime(hFile, NULL, NULL, &lastWriteTime) != FALSE)
		{
			SYSTEMTIME	systemTime;

			if (FileTimeToSystemTime(&lastWriteTime, &systemTime) != FALSE)
			{
				char	szString[1024]	= {0};

				int	iCount	= GetDateFormatA(LOCALE_INVARIANT, 0, &systemTime, "yyMMdd", szString, sizeof(szString));

				if (iCount > 0)
				{
					std::string	strTemp	= szString;

					iCount	= GetTimeFormatA(LOCALE_INVARIANT, TIME_FORCE24HOURFORMAT, &systemTime, "hhmmss", szString, sizeof(szString));

					if (iCount > 0)
					{
						strTemp	+= szString;

						return	strTemp;	
					}
				}
			}
		}
	}

	return	"";
}

std::wstring File::getDateTime(const std::wstring& _strPath)
{
	HANDLE	hFile	= CreateFileW((const LPWSTR)_strPath.c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		FILETIME	lastWriteTime;

		if (GetFileTime(hFile, NULL, NULL, &lastWriteTime) != FALSE)
		{
			SYSTEMTIME	systemTime;

			if (FileTimeToSystemTime(&lastWriteTime, &systemTime) != FALSE)
			{
				wchar_t	szString[1024]	= {0};

				int	iCount	= GetDateFormatW(LOCALE_USER_DEFAULT, 0, &systemTime, L"yyMMdd", szString, sizeof(szString) / sizeof(wchar_t));

				if (iCount > 0)
				{
					std::wstring	strTemp	= szString;

					iCount	= GetTimeFormatW(LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &systemTime, L"hhmmss", szString, sizeof(szString) / sizeof(wchar_t));

					if (iCount > 0)
					{
						strTemp	+= szString;

						return	strTemp;	
					}
				}
			}
		}
	}

	return	L"";
}

bool File::findFiles(const std::string& _strPath, std::vector<std::string>& _vecFiles)
{
	_vecFiles.clear();

	WIN32_FIND_DATAA	findData;

	HANDLE	hFindFile	= FindFirstFileA(_strPath.c_str(), &findData);

	if (INVALID_HANDLE_VALUE == hFindFile)
	{
		return	false;
	}
		
	if (0 == (FILE_ATTRIBUTE_DIRECTORY & findData.dwFileAttributes))
	{
		std::string	strFound	= findData.cFileName;
		
		_vecFiles.push_back(strFound);
	}

	while (FindNextFileA(hFindFile, &findData) != FALSE)
	{
		if (0 == (FILE_ATTRIBUTE_DIRECTORY & findData.dwFileAttributes))
		{
			std::string	strFound	= findData.cFileName;
		
			_vecFiles.push_back(strFound);
		}
	}
	
	FindClose(hFindFile);

	return	true;
}

bool File::findFiles(const std::wstring& _strPath, std::vector<std::wstring>& _vecFiles)
{
	_vecFiles.clear();

	WIN32_FIND_DATAW	findData;

	HANDLE	hFindFile	= FindFirstFileW(_strPath.c_str(), &findData);

	if (INVALID_HANDLE_VALUE == hFindFile)
	{
		return	false;
	}
		
	if (0 == (FILE_ATTRIBUTE_DIRECTORY & findData.dwFileAttributes))
	{
		std::wstring	strFound	= findData.cFileName;
		
		_vecFiles.push_back(strFound);
	}

	while (FindNextFileW(hFindFile, &findData) != FALSE)
	{
		if (0 == (FILE_ATTRIBUTE_DIRECTORY & findData.dwFileAttributes))
		{
			std::wstring	strFound	= findData.cFileName;
		
			_vecFiles.push_back(strFound);
		}
	}
	
	FindClose(hFindFile);

	return	true;
}

bool File::findDirectories(const std::string& _strPath, std::vector<std::string>& _vecDirectories)
{
	std::string	strSearch;

	strSearch	= _strPath;
	
	addDirectorySeparator(strSearch);

	strSearch	+= "*.*";
	
	WIN32_FIND_DATAA	findData;

	HANDLE	hFindFile	= FindFirstFileA(strSearch.c_str(), &findData);
	
	if (INVALID_HANDLE_VALUE == hFindFile)
	{
		return	false;
	}

	if ((FILE_ATTRIBUTE_DIRECTORY & findData.dwFileAttributes) != 0)
	{
		std::string	strFound	= findData.cFileName;
		
		if (strFound != "." && strFound != "..")
		{
			_vecDirectories.push_back(strFound);
		}
	}

	while (FindNextFileA(hFindFile, &findData) != FALSE)
	{
		if ((FILE_ATTRIBUTE_DIRECTORY & findData.dwFileAttributes) != 0)
		{
			std::string	strFound	= findData.cFileName;
		
			if (strFound != "." && strFound != "..")
			{
				_vecDirectories.push_back(strFound);
			}
		}
	}
	
	FindClose(hFindFile);

	return	true;
}

bool File::findDirectories(const std::wstring& _strPath, std::vector<std::wstring>& _vecDirectories)
{
	std::wstring	strSearch;

	strSearch	= _strPath;
	
	addDirectorySeparator(strSearch);

	strSearch	+= L"*.*";
	
	WIN32_FIND_DATAW	findData;

	HANDLE	hFindFile	= FindFirstFileW(strSearch.c_str(), &findData);
	
	if (INVALID_HANDLE_VALUE == hFindFile)
	{
		return	false;
	}

	if ((FILE_ATTRIBUTE_DIRECTORY & findData.dwFileAttributes) != 0)
	{
		std::wstring	strFound	= findData.cFileName;
		
		if (strFound != L"." && strFound != L"..")
		{
			_vecDirectories.push_back(strFound);
		}
	}

	while (FindNextFileW(hFindFile, &findData) != FALSE)
	{
		if ((FILE_ATTRIBUTE_DIRECTORY & findData.dwFileAttributes) != 0)
		{
			std::wstring	strFound	= findData.cFileName;
		
			if (strFound != L"." && strFound != L"..")
			{
				_vecDirectories.push_back(strFound);
			}
		}
	}
	
	FindClose(hFindFile);

	return	true;
}

void File::addDirectorySeparator(std::string& _strPath)
{
	char	separator;

	if (_strPath.find('\\') != std::string::npos)
	{
		separator	= '\\';
	}

	else
	{
		separator	= '/';
	}

	int	iPos	= (int)_strPath.rfind(separator);

	if (-1 == iPos || iPos < (int)_strPath.length() - 1)
	{
		_strPath	+= separator;
	}
}

void File::removeDirectorySeparator(std::wstring& _strPath)
{
	wchar_t	separator;

	if (_strPath.find(L'\\') != std::wstring::npos)
	{
		separator	= L'\\';
	}

	else
	{
		separator	= L'/';
	}

	int	iPos	= (int)_strPath.rfind(separator);

	if (iPos == (int)_strPath.length() - 1)
	{
		_strPath.erase(iPos, 1);
	}
}

void File::removeDirectorySeparator(std::string& _strPath)
{
	char	separator;

	if (_strPath.find('\\') != std::string::npos)
	{
		separator	= '\\';
	}

	else
	{
		separator	= '/';
	}

	int	iPos	= (int)_strPath.rfind(separator);

	if (iPos == (int)_strPath.length() - 1)
	{
		_strPath.erase(iPos, 1);
	}
}

void File::addDirectorySeparator(std::wstring& _strPath)
{
	wchar_t	separator;

	if (_strPath.find(L'\\') != std::wstring::npos)
	{
		separator	= L'\\';
	}

	else
	{
		separator	= L'/';
	}

	int	iPos	= (int)_strPath.rfind(separator);

	if (-1 == iPos || iPos < (int)_strPath.length() - 1)
	{
		_strPath	+= separator;
	}
}

void File::convertToFileURL(std::string& _strPath)
{
	if (_strPath.find("file:///") != std::string::npos)
	{
		_strPath.insert(0, "file:///");
	}

	size_t	t_c	= _strPath.length();

	for (size_t iLoop = 0; iLoop < t_c; ++iLoop)
	{
		if ('\\' == _strPath[iLoop])
		{
			_strPath[iLoop]	= '/';
		}
	}
}

void File::convertToFileURL(std::wstring& _strPath)
{
	if (_strPath.find(L"file:///") != std::wstring::npos)
	{
		_strPath.insert(0, L"file:///");
	}

	size_t	t_c	= _strPath.length();

	for (size_t iLoop = 0; iLoop < t_c; ++iLoop)
	{
		if (L'\\' == _strPath[iLoop])
		{
			_strPath[iLoop]	= L'/';
		}
	}
}

bool File::getSafeFilename(std::string& _strPath)
{
	int	iSize	= (int)_strPath.size();

	if (0 == iSize)
	{
		Log::instance()->logError("Filename is empty");

		return	false;
	}

	std::string	strOriginalPath	= _strPath;

	_strPath.clear();

	for (int iLoop = 0; iLoop < iSize; ++iLoop)
	{
		int	iCharacter	= strOriginalPath[iLoop];

		UINT	type	= PathGetCharTypeA(iCharacter);

		if (GCT_INVALID == type || GCT_SEPARATOR == type)
		{
			continue;
		}

		_strPath.push_back(iCharacter);
	}

	return	true;
}

bool File::getSafeFilename(std::wstring& _strPath)
{
	int	iSize	= (int)_strPath.size();

	if (0 == iSize)
	{
		Log::instance()->logError("Filename is empty");

		return	false;
	}

	std::wstring	strOriginalPath	= _strPath;

	_strPath.clear();

	for (int iLoop = 0; iLoop < iSize; ++iLoop)
	{
		int	iCharacter	= strOriginalPath[iLoop];

		UINT	type	= PathGetCharType(iCharacter);

		if (GCT_INVALID == type || GCT_SEPARATOR == type)
		{
			continue;
		}

		_strPath.push_back(iCharacter);
	}

	return	true;
}
