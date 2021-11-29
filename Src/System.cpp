#include "Duo.h"
#include "Log.h"
#include "System.h"

Duo::Ptr	System::ms_pDuo;	// Duo
Window::Ptr	System::ms_pWindow;	// Window

bool System::initialize()
{
	ms_pDuo	= Duo::create();

	if (nullptr == ms_pDuo)
	{
		Log::instance()->logError("Unable to create Duo interface");

		return	false;
	}

	ms_pWindow	= Window::create();
	
	if (nullptr == ms_pWindow)
	{
		Log::instance()->logError("Unable to create window");

		return	false;
	}

	return	true;
}

void System::close()
{
	// Delete window
	ms_pWindow.reset();

	// Delete Duo
	ms_pDuo.reset();
}

bool System::run()
{
	MSG		msg;

	memset(&msg, 0, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{ 
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (ms_pDuo != nullptr)
		{
			if (false == ms_pDuo->run())			{				return	false;			}		}
	}

	return	true;
}
