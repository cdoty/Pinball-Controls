#include "System.h"

int WINAPI WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _iCmdShow)
{
	int	iRetVal	= 0;

	if (false == System::initialize())
	{
		iRetVal	= 1;
	}

	else
	{
		System::run();
	}

	System::close();

	return	iRetVal;
}
