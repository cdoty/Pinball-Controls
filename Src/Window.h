#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <windows.h>
#include <shellapi.h>

#include "Macros.h"

class Window
{
	public:
		PTR(Window)

		// Destructor
		virtual ~Window();

		// Create
		static Ptr create();
		
		// Initialize
		virtual bool initialize();
		
		// Close
		void close();

		// Get popup menu
		HMENU getPopupMenu() const {return m_hPopupMenu;}

		// Message proc
		static LRESULT CALLBACK messageProc(HWND _hWnd, UINT _uMsg, WPARAM _wParam, LPARAM _lParam);

	protected:
		HINSTANCE			m_hInstance;		// App instance
		HWND				m_hWnd;				// Hidden window handle
		ATOM				m_classAtom;		// Window class atom
		NOTIFYICONDATA		m_notifyIconData;	// Notify icon data
		HMENU				m_hPopupMenu;		// Popup menu

		// Constructor
		Window();

		// Add tray icon
		bool addTrayIcon();

		// Load menus
		bool loadMenus();

		// Update use space
		void updateUseSpace();
};
