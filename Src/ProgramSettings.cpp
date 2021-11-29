#include "File.h"
#include "INISettings.h"
#include "Log.h"
#include "ProgramSettings.h"

static const char*	gsc_szProgramSettingsFilename	= "ProgramSettings.ini";	// Camera settings filename

ProgramSettings::ProgramSettings()	:
	m_bUseSpace(false)
{
}

ProgramSettings::~ProgramSettings()
{
	close();
}

ProgramSettings::Ptr ProgramSettings::create()
{
	INSTANCE(pProgramSettings, ProgramSettings());
	
	if (false == pProgramSettings->initialize())
	{
		pProgramSettings.reset();
	}

	return	pProgramSettings;
}

bool ProgramSettings::initialize()
{
	if (false == readSettings())
	{
		return	false;
	}

	return	true;
}

void ProgramSettings::close()
{
}

bool ProgramSettings::saveSettings()
{
	File::Ptr	pFile	= File::create();

	if (false == pFile->create(gsc_szProgramSettingsFilename))
	{
		Log::instance()->logError("Unable to write settings file.");

		return	false;
	}

	pFile->writeString("[Settings]\n");
	
	std::string	strSetting;

	strSetting	= "UseSpace = ";
	strSetting	+= true == m_bUseSpace ? "true" : "false";
	strSetting	+= "\n";

	pFile->writeString(strSetting);

	pFile->close();

	return	true;
}

bool ProgramSettings::readSettings()
{
	Settings::Ptr	pSettings	= Settings::create(gsc_szProgramSettingsFilename, false);

	if (NULL == pSettings)
	{
		m_bUseSpace	= false;
		return	true;
	}

	// Get use space
	m_bUseSpace		= pSettings->getBoolSetting("Settings", "UseSpace", m_bUseSpace);

	pSettings.reset();

	return	true;
}
