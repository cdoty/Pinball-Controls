#include <windowsx.h>

#include "Defines.h"
#include "Log.h"
#include "System.h"
#include "Window.h"
#include "../Resources/resource.h"

#define	ID_TASKBAR	0xBA112021
#define WM_TRAYMENU	(WM_USER + 1)

Window::Window()	:
	m_hInstance(NULL),
	m_hWnd(NULL),
	m_classAtom(INVALID_ATOM),
	m_hPopupMenu(NULL)
{
	memset(&m_notifyIconData, 0, sizeof(NOTIFYICONDATA));

	m_notifyIconData.cbSize	= sizeof(NOTIFYICONDATA);
}

Window::~Window()
{
	close();
}

Window::Ptr Window::create()
{
	INSTANCE(pWindow, Window())
	
	if (false == pWindow->initialize())
	{
		pWindow.reset();
	}	
	
	return	pWindow;
}

bool Window::initialize()
{
	m_hInstance	= GetModuleHandle(NULL);
	
	WNDCLASS	wndClass;	
	
	memset(&wndClass, 0, sizeof(WNDCLASS));

	wndClass.lpfnWndProc	= messageProc;
	wndClass.hInstance		= m_hInstance;
	wndClass.hIcon			= LoadIcon(m_hInstance, MAKEINTRESOURCE(ICON_PROGRAM));
	wndClass.hCursor		= NULL;
	wndClass.hbrBackground	= NULL;
	wndClass.lpszMenuName	= NULL;
	wndClass.lpszClassName	= gsc_szClassName;

	// Register windows class
	m_classAtom	= RegisterClass(&wndClass);

	if (0 == m_classAtom)
	{
		Log::instance()->logError("Unable to register windows class (%08X)", GetLastError());
		
		return	false;
	}
	
	// Create window
	m_hWnd	= CreateWindowEx(WS_EX_TOOLWINDOW, gsc_szClassName, L"", 0, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, m_hInstance, this);
	
	if (NULL == m_hWnd)
	{
		Log::instance()->logError("Unable to create window (%08X)", GetLastError());
		
		return	false;
	}

	if (FALSE == SetPriorityClass(GetCurrentProcess (), HIGH_PRIORITY_CLASS))
	{
		Log::instance()->logError("Unable to set priority class (%08X)", GetLastError());
	}

	// Add tray icon
	if (false == addTrayIcon())
	{
		return	false;
	}

	// Load menus
	if (false == loadMenus())
	{
		return	false;
	}

	return	true;
}

void Window::close()
{
	Shell_NotifyIcon(NIM_DELETE, &m_notifyIconData);

	if (m_hPopupMenu != NULL)
	{
		DestroyMenu(m_hPopupMenu);

		m_hPopupMenu	= NULL;
	}

	if (m_hInstance != NULL)
	{
		UnregisterClass(gsc_szClassName, m_hInstance);
		
		m_hInstance	= NULL;
	}
}

LRESULT CALLBACK Window::messageProc(HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam)
{
	switch (_uMsg)
	{
		case WM_CLOSE:
			DestroyWindow(_hWnd);

			return	0;

		case WM_DESTROY:
			PostQuitMessage(0);

			break;
	
		case WM_COMMAND:
			switch (LOWORD(_wParam))
			{
				case POPUP_RECONNECT:
				{
					Duo::Ptr	pDuo	= System::getDuo();
					
					if (pDuo != nullptr)
					{
						pDuo->reconnect();
					}

					return	TRUE;
				}

				case POPUP_USESPACE:
				{
					Duo::Ptr	pDuo	= System::getDuo();

					if (pDuo != nullptr)
					{
						pDuo->setUseSpace(false == pDuo->getUseSpace());

						Window::Ptr	pWindow	= System::getWindow();

						if (pWindow != nullptr)
						{
							pWindow->updateUseSpace();
						}
					}
										
					break;
				}

				case POPUP_EXIT:
					PostQuitMessage(0);
					
					return	TRUE;
			}

			break;

		case WM_TRAYMENU:
			if (WM_LBUTTONDOWN == _lParam || WM_RBUTTONDOWN == _lParam)
			{
				POINT	ptCursor;

				if (FALSE != GetCursorPos(&ptCursor))
				{
					Window::Ptr	pWindow	= System::getWindow();

					if (pWindow != nullptr)
					{
						TrackPopupMenu(GetSubMenu(pWindow->getPopupMenu(), 0), TPM_BOTTOMALIGN | TPM_LEFTALIGN, ptCursor.x, ptCursor.y, 0, _hWnd, NULL);
					}

					return	TRUE;
				}
			}

			break;
	}

	return	DefWindowProc(_hWnd, _uMsg, _wParam, _lParam);
}

bool Window::addTrayIcon()
{
	memset(&m_notifyIconData, 0, sizeof(NOTIFYICONDATA));

	m_notifyIconData.cbSize				= sizeof(NOTIFYICONDATA);
	m_notifyIconData.hWnd				= m_hWnd;
	m_notifyIconData.uID				= ID_TASKBAR;
	m_notifyIconData.uFlags 			= NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_notifyIconData.uCallbackMessage	= WM_TRAYMENU;
	m_notifyIconData.hIcon				= (HICON)LoadImage(m_hInstance, MAKEINTRESOURCE(ICON_PROGRAM), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), 
		LR_DEFAULTCOLOR);
	
	wcsncpy(m_notifyIconData.szTip, gsc_szTaskbarName, sizeof(m_notifyIconData.szTip) / sizeof(WCHAR));

	if (FALSE == Shell_NotifyIcon(NIM_ADD, &m_notifyIconData))
	{
		Log::instance()->logError("Unable to add shell icon (%08X)", GetLastError());
		
		return	false;
	}
	
	return	true;
}

bool Window::loadMenus()
{
	m_hPopupMenu	= LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(MENU_POPUP));

	if (NULL == m_hPopupMenu)
	{
		Log::instance()->logError("Unable to load popup menu (%08X)", GetLastError());
		
		return	false;
	}
	
	updateUseSpace();

	return	true;
}

void Window::updateUseSpace()
{
	Duo::Ptr	pDuo	= System::getDuo();

	if (pDuo != nullptr)
	{
		CheckMenuItem(m_hPopupMenu, POPUP_USESPACE, pDuo->getUseSpace() ? MF_CHECKED : MF_UNCHECKED);
	}
}
