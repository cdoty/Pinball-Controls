#include "stdafx.h"
#include "enumser.h"

#include <string.h>
#include <tchar.h>

#include "Duo.h"
#include "Log.h"

static const int	gsc_iMessageSize	= 6;
static const int	gsc_iBufferLength	= gsc_iMessageSize * 10;

DEFINE_GUID(GUID_DEVCLASS_BLUETOOTH, 0xe0cbf06cL, 0xcd8b, 0x4647, 0xbb, 0x8a, 0x26, 0x3b, 0x43, 0xf0, 0xf9, 0x74);

#pragma comment(lib, "Setupapi.lib")

Duo::Duo()	:
	m_hComPort(INVALID_HANDLE_VALUE),
	m_iWriteIndex(0),
	m_iCurrentInput(0),
	m_bLeftFlipperPressed(false),
	m_bRightFlipperPressed(false),
	m_bPlungerActive(false),
	m_bConnected(false),
	m_bUseSpace(false)
{
	CoInitialize(NULL);
	CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
}

Duo::~Duo()
{
	close();

	CoUninitialize();
}

Duo::Ptr Duo::create()
{
	INSTANCE(pDuo, Duo())

	if (false == pDuo->initialize())
	{
		pDuo.reset();
	}

	return	pDuo;
}

bool Duo::initialize()
{
	loadSettings();

	if (false == findComPort())
	{
		return	false;
	}

	if (false == connect())
	{
		return	false;
	}

	return	true;
}

void Duo::close()
{
	closePort();
	saveSettings();
}

bool Duo::run()
{
	if (true == m_bConnected)
	{
		static char	messageBuffer[gsc_iBufferLength];

		DWORD	received	= 0;

		ReadFile(m_hComPort, &messageBuffer[m_iWriteIndex], gsc_iBufferLength - m_iWriteIndex, &received, NULL);
		
		if (received > 0)
		{
#if 0
			Log::instance()->logError("Received %d bytes", received);

			std::string	strBuffer;

			for (int iLoop = 0; iLoop < (int)received; ++iLoop)
			{
				strBuffer	+= std::to_string((uint8_t)messageBuffer[iLoop + m_iWriteIndex]);
				strBuffer	+= " ";
			}

			Log::instance()->logError("%s", strBuffer.c_str());
#endif

			m_iWriteIndex	= 0;
			m_iCurrentInput	= 0;

			int	iBufferSize	= m_iWriteIndex + (int)received;

			if (iBufferSize >= gsc_iMessageSize)
			{
				int	iBufferOffset	= 0;

				while (iBufferOffset + gsc_iMessageSize <= iBufferSize)
				{
					if ('Z' == messageBuffer[iBufferOffset + 0] && '¥' == messageBuffer[iBufferOffset + 1])
					{
						switch (messageBuffer[iBufferOffset + 2])
						{
							case 1:
								handleFlippers(messageBuffer[iBufferOffset + 3]);

								break;

							case 2:
								handlePlunger(messageBuffer[iBufferOffset + 3]);

								break;
						}
					}

					iBufferOffset	+= gsc_iMessageSize;
				}

				if (iBufferOffset < iBufferSize)
				{
					int	iLeft	= iBufferSize - iBufferOffset;

					memcpy(&messageBuffer[0], &messageBuffer[iBufferOffset], iLeft);

					m_iWriteIndex	= iLeft;
				}
			}

			else
			{
				m_iWriteIndex	+= received;
			}

			if (m_iCurrentInput > 0)
			{
				SendInput(m_iCurrentInput, m_inputs, sizeof(INPUT));
			}
		}
	}

	return	true;
}

void Duo::reconnect()
{
	if (true == m_bConnected)
	{
		closePort();
	}

	connect();
}

bool Duo::connect()
{
	m_hComPort	= CreateFile(m_strComPort.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	
	if (INVALID_HANDLE_VALUE == m_hComPort)
	{
		Log::instance()->logError("Unable to open com port %s (%08X)", m_strComPort.c_str(), GetLastError());

		return	false;
	}

	DCB	config;

	if (0 == GetCommState(m_hComPort, &config))
	{
		Log::instance()->logError("Unable to get comm state (%08X)", GetLastError());

		return	false;
	}
	
	config.BaudRate				= 9600;
	config.StopBits				= ONESTOPBIT;
	config.Parity				= 0;
	config.ByteSize				= 8;
	config.fOutxCtsFlow			= FALSE;
	config.fOutxDsrFlow			= FALSE;
	config.fDtrControl			= DTR_CONTROL_DISABLE;
	config.fDsrSensitivity		= FALSE;
	config.fTXContinueOnXoff	= FALSE;
	config.fOutX				= FALSE;
	config.fInX					= FALSE;
	config.fErrorChar			= FALSE;
	config.EvtChar				= 'Z';
	config.fNull				= FALSE;
	config.fAbortOnError		= FALSE;

	if (0 == SetCommState(m_hComPort, &config))
	{
		Log::instance()->logError("Unable to set comm state (%08X)", GetLastError());

		return	false;
	}

	COMMTIMEOUTS	timeOuts;

	timeOuts.ReadIntervalTimeout			= MAXDWORD;
	timeOuts.ReadTotalTimeoutMultiplier		= 0;
	timeOuts.ReadTotalTimeoutConstant		= 0;
	timeOuts.WriteTotalTimeoutMultiplier	= 0;
	timeOuts.WriteTotalTimeoutConstant		= 0;

	if (FALSE == SetCommTimeouts(m_hComPort, &timeOuts))
	{
		Log::instance()->logError("Unable to set comm timeouts (%08X)", GetLastError());

		return	false;
	}

	m_bConnected	= true;

	return	true;
}

void Duo::closePort()
{
	if (m_hComPort != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hComPort);

		m_hComPort	= INVALID_HANDLE_VALUE;
	}

	m_bConnected	= false;
}

void Duo::handleFlippers(char _event)
{
	switch (_event)
	{
		case 0:
			if (true == m_bLeftFlipperPressed)
			{
				m_bLeftFlipperPressed	= false;

				m_inputs[m_iCurrentInput].type				= INPUT_KEYBOARD;
				m_inputs[m_iCurrentInput].ki.dwFlags		= KEYEVENTF_KEYUP;
				m_inputs[m_iCurrentInput].ki.wVk			= VK_LSHIFT;
				m_inputs[m_iCurrentInput].ki.time			= 0;
				m_inputs[m_iCurrentInput].ki.wScan			= 0;
				m_inputs[m_iCurrentInput].ki.dwExtraInfo	= NULL;

				m_iCurrentInput++;
			}

			if (true == m_bRightFlipperPressed)
			{
				m_bRightFlipperPressed	= false;

				m_inputs[m_iCurrentInput].type				= INPUT_KEYBOARD;
				m_inputs[m_iCurrentInput].ki.dwFlags		= KEYEVENTF_KEYUP;
				m_inputs[m_iCurrentInput].ki.wVk			= VK_RSHIFT;
				m_inputs[m_iCurrentInput].ki.time			= 0;
				m_inputs[m_iCurrentInput].ki.wScan			= 0;
				m_inputs[m_iCurrentInput].ki.dwExtraInfo	= NULL;

				m_iCurrentInput++;
			}

			break;

		case 1:
			if (false == m_bLeftFlipperPressed)
			{
				m_bLeftFlipperPressed	= true;

				m_inputs[m_iCurrentInput].type				= INPUT_KEYBOARD;
				m_inputs[m_iCurrentInput].ki.dwFlags		= 0;
				m_inputs[m_iCurrentInput].ki.wVk			= VK_LSHIFT;
				m_inputs[m_iCurrentInput].ki.time			= 0;
				m_inputs[m_iCurrentInput].ki.wScan			= 0;
				m_inputs[m_iCurrentInput].ki.dwExtraInfo	= NULL;

				m_iCurrentInput++;
			}

			if (true == m_bRightFlipperPressed)
			{
				m_bRightFlipperPressed	= false;

				m_inputs[m_iCurrentInput].type				= INPUT_KEYBOARD;
				m_inputs[m_iCurrentInput].ki.dwFlags		= KEYEVENTF_KEYUP;
				m_inputs[m_iCurrentInput].ki.wVk			= VK_RSHIFT;
				m_inputs[m_iCurrentInput].ki.time			= 0;
				m_inputs[m_iCurrentInput].ki.wScan			= 0;
				m_inputs[m_iCurrentInput].ki.dwExtraInfo	= NULL;

				m_iCurrentInput++;
			}

			break;

		case 2:
			if (false == m_bRightFlipperPressed)
			{
				m_bRightFlipperPressed	= true;

				m_inputs[m_iCurrentInput].type				= INPUT_KEYBOARD;
				m_inputs[m_iCurrentInput].ki.dwFlags		= 0;
				m_inputs[m_iCurrentInput].ki.wVk			= VK_RSHIFT;
				m_inputs[m_iCurrentInput].ki.time			= 0;
				m_inputs[m_iCurrentInput].ki.wScan			= 0;
				m_inputs[m_iCurrentInput].ki.dwExtraInfo	= NULL;

				m_iCurrentInput++;
			}

			if (true == m_bLeftFlipperPressed)
			{
				m_bLeftFlipperPressed	= false;

				m_inputs[m_iCurrentInput].type				= INPUT_KEYBOARD;
				m_inputs[m_iCurrentInput].ki.dwFlags		= KEYEVENTF_KEYUP;
				m_inputs[m_iCurrentInput].ki.wVk			= VK_LSHIFT;
				m_inputs[m_iCurrentInput].ki.time			= 0;
				m_inputs[m_iCurrentInput].ki.wScan			= 0;
				m_inputs[m_iCurrentInput].ki.dwExtraInfo	= NULL;

				m_iCurrentInput++;
			}

			break;

		case 3:
			if (false == m_bLeftFlipperPressed)
			{
				m_bLeftFlipperPressed	= true;

				m_inputs[m_iCurrentInput].type				= INPUT_KEYBOARD;
				m_inputs[m_iCurrentInput].ki.dwFlags		= 0;
				m_inputs[m_iCurrentInput].ki.wVk			= VK_LSHIFT;
				m_inputs[m_iCurrentInput].ki.time			= 0;
				m_inputs[m_iCurrentInput].ki.wScan			= 0;
				m_inputs[m_iCurrentInput].ki.dwExtraInfo	= NULL;

				m_iCurrentInput++;
			}

			if (false == m_bRightFlipperPressed)
			{
				m_bRightFlipperPressed	= true;

				m_inputs[m_iCurrentInput].type				= INPUT_KEYBOARD;
				m_inputs[m_iCurrentInput].ki.dwFlags		= 0;
				m_inputs[m_iCurrentInput].ki.wVk			= VK_RSHIFT;
				m_inputs[m_iCurrentInput].ki.time			= 0;
				m_inputs[m_iCurrentInput].ki.wScan			= 0;
				m_inputs[m_iCurrentInput].ki.dwExtraInfo	= NULL;

				m_iCurrentInput++;
			}

			break;
	}
}

void Duo::handlePlunger(char _event)
{
	if (0 == _event)
	{
		if (true == m_bPlungerActive)
		{
			m_bPlungerActive	= false;

			m_inputs[m_iCurrentInput].type				= INPUT_KEYBOARD;
			m_inputs[m_iCurrentInput].ki.dwFlags		= KEYEVENTF_KEYUP;
			m_inputs[m_iCurrentInput].ki.wVk			= true == m_bUseSpace ? VK_SPACE : VK_RETURN;
			m_inputs[m_iCurrentInput].ki.time			= 0;
			m_inputs[m_iCurrentInput].ki.wScan			= 0;
			m_inputs[m_iCurrentInput].ki.dwExtraInfo	= NULL;

			m_iCurrentInput++;
		}
	}

	else
	{
		if (false == m_bPlungerActive)
		{
			m_bPlungerActive	= true;

			m_inputs[m_iCurrentInput].type				= INPUT_KEYBOARD;
			m_inputs[m_iCurrentInput].ki.dwFlags		= 0;
			m_inputs[m_iCurrentInput].ki.wVk			= true == m_bUseSpace ? VK_SPACE : VK_RETURN;
			m_inputs[m_iCurrentInput].ki.time			= 0;
			m_inputs[m_iCurrentInput].ki.wScan			= 0;
			m_inputs[m_iCurrentInput].ki.dwExtraInfo	= NULL;

			m_iCurrentInput++;
		}
	}
}

bool Duo::findComPort()
{
	CEnumerateSerial::CPortAndNamesArray portAndNames;

	HRESULT	hResult	= CEnumerateSerial::UsingSetupAPI1(portAndNames);

	if (FAILED(hResult))
	{
		Log::instance()->logError("Unable to get a list of com ports");

		return	false;
	}
	
	int	iComPort	= -1;

	int	t_c	= (int)portAndNames.size();

	for (int iLoop = 0; iLoop < t_c; ++iLoop)
	{
		if (L"Standard Serial over Bluetooth link" == portAndNames[iLoop].second)
		{
			iComPort	= iLoop;
		}
	}

	if (-1 == iComPort)
	{
		Log::instance()->logError("No com ports match the Pinball Duo.");

		return	false;
	}

	m_strComPort	= L"COM";
	m_strComPort	+= std::to_wstring(portAndNames[iComPort].first);

	return	true;
}

bool Duo::loadSettings()
{
	m_pSettings	= ProgramSettings::create();

	if (nullptr == m_pSettings)
	{
		return	false;
	}

	m_bUseSpace	= m_pSettings->getUseSpace();

	return	true;
}

void Duo::saveSettings()
{
	if (m_pSettings != nullptr)
	{
		m_pSettings->setUseSpace(m_bUseSpace);
		m_pSettings->saveSettings();
	}
}
