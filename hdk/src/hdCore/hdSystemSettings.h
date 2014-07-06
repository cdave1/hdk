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

#ifdef __cplusplus
extern "C" {
#endif
    
	extern void SystemSettings_Init();
	
	extern char * SystemSettings_SystemName();
	
	extern char * SystemSettings_SystemArchName();
	
	extern bool SystemSettings_HasVFPU();
	
	extern int SystemSettings_OpenGLVersion();
	
	extern void SystemSettings_TearDown();
	
	
#ifdef __cplusplus
}
#endif

#endif
