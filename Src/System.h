#pragma once

#include "Duo.h"
#include "Window.h"

class System
{
	public:
		// Initialize
		static bool initialize();
		
		// Close
		static void close();
		
		// Run
		static bool run();

		// Get duo
		static Duo::Ptr getDuo() {return ms_pDuo;}

		// Get window
		static Window::Ptr getWindow() {return ms_pWindow;}

	private:
		static Duo::Ptr		ms_pDuo;	// Duo
		static Window::Ptr	ms_pWindow;	// Window
};
