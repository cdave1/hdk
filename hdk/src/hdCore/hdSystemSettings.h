/*
 *  hdSystemSettings.h
 *  TotemGame
 *
 *  Created by david on 12/09/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_SYSTEM_SETTINGS_H
#define HD_SYSTEM_SETTINGS_H
/*
class hdSystemSettingsValues
{
	
	friend void 	SystemSettings_Init();
public:
	hdSystemSettingsValues ()
	{
		m_minFrameDuration = 0.0;
		m_maxTextureBPP = 0;
	}
	
	const char * GetSystemName() const;
	
	const double GetMinFrameDuration() const;
	
	const int GetMaxTextureBPP() const;
	
private:
	
	char m_systemName[256];
	
	double m_minFrameDuration;
	
	int m_maxTextureBPP;
};


inline 	const char * hdSystemSettingsValues::GetSystemName() const
{
	return m_systemName;
}


inline const double hdSystemSettingsValues::GetMinFrameDuration() const
{
	return m_minFrameDuration;
}


inline const int hdSystemSettingsValues::GetMaxTextureBPP() const
{
	return m_maxTextureBPP;
}
*/


#ifdef __cplusplus
extern "C" {
#endif


	
	
	extern void SystemSettings_Init();
	
//	extern hdSystemSettingsValues* SystemSettings_GetValues();
	
	extern char * SystemSettings_SystemName();
	
	extern char * SystemSettings_SystemArchName();
	
	extern bool SystemSettings_HasVFPU();
	
	extern int SystemSettings_OpenGLVersion();
	
	extern void SystemSettings_TearDown();
	
	
#ifdef __cplusplus
}
#endif




#endif
