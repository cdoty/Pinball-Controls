#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <windows.h>

#include "Macros.h"
#include "ProgramSettings.h"

class Duo
{
	public:
		PTR(Duo)

		// Destructor
		~Duo();

		// Create
		static Ptr create();

		// Initialize
		bool initialize();

		// Close
		void close();

		// Run
		bool run();

		// Reconnect
		void reconnect();

		// Get use space
		bool getUseSpace() const {return m_bUseSpace;}

		// Set use space
		void setUseSpace(bool _bUseSpace) {m_bUseSpace = _bUseSpace;}

	private:
		static const int	msc_iMaxInputs	= 8;	// Maximum inputs
		
		ProgramSettings::Ptr	m_pSettings;				// Program settings
		HANDLE					m_hComPort;					// Com port handle
		std::wstring			m_strComPort;				// Com port name
		int						m_iWriteIndex;				// Write index
		INPUT					m_inputs[msc_iMaxInputs];	// Inputs
		int						m_iCurrentInput;			// Current input
		bool					m_bLeftFlipperPressed;		// Left flipper pressed?
		bool					m_bRightFlipperPressed;		// Right flipper pressed?
		bool					m_bPlungerActive;			// Plunger active?
		bool					m_bConnected;				// Connected?
		bool					m_bUseSpace;				// Use space?

		// Constructor
		Duo();

		// Connect
		bool connect();

		// Close port
		void closePort();

		// Handle flippers
		void handleFlippers(char _event);

		// Handle plunger
		void handlePlunger(char _event);

		// Find com port
		bool findComPort();

		// Load settings
		bool loadSettings();

		// Save settings
		void saveSettings();
};