#pragma once

#include "Macros.h"

class ProgramSettings
{
	public:
		PTR(ProgramSettings);

		// Destructor
		~ProgramSettings();

		// Create
		static Ptr create();

		// Initialize
		bool initialize();

		// Close
		void close();
	
		// Get use space
		bool getUseSpace() const {return m_bUseSpace;}

		// Set use space
		void setUseSpace(bool _bUseSpace) {m_bUseSpace = _bUseSpace;}

		// Save settings
		bool saveSettings();

	private:
		bool	m_bUseSpace;	// Use space?

		// Constructor
		ProgramSettings();

		// Read settings
		bool readSettings();
};
