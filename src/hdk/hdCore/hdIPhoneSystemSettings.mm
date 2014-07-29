/*
 *  hdIPhoneSystemSettings.cpp
 *  TotemGame
 *
 *  Created by david on 12/09/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#import "hdIPhoneSystemSettings.h"

#import <Foundation/Foundation.h>

#ifdef __arm__
#include "mach-o/arch.h"
#endif

#import <sys/utsname.h>
#import <sys/types.h>
#import <sys/sysctl.h>

@implementation hdIPhoneSystemSettings

//static hdSystemSettingsValues *values = NULL;

//static struct utsname u;
static bool systemInitted = false;

static bool hasVFPU = false;

static char systemArchName[64];

static char systemName[256];

static int openGLVersion;



void SystemSettings_Init()
{
	struct utsname u;
	
	if (systemInitted) return;
	
	/*
	 * Machine name
	 */
	uname(&u);
	snprintf(systemName, 256, "%s", u.machine);
	hdPrintf("%s\n", systemName);
	/*
	 * Arch
	 */
	snprintf(systemArchName, 64, "Not Specified");
#ifdef __arm__
	NXArchInfo *archInfo = (NXArchInfo *)NXGetLocalArchInfo();
	
	if (archInfo != NULL)
	{
		hdPrintf("%s, %d, %d, %s\n", 
			   archInfo->name,
			   archInfo->cputype,
			   archInfo->cpusubtype,
			   archInfo->description);
		snprintf(systemArchName, 64, "%s", archInfo->name);
	}
	else
	{
		snprintf(systemArchName, 64, "na");
	}
	
	// HACK HACK HACK
	//
	// armv6 does not necessarily guarantee us the presence of the VFP unit!!!
	//
	// Could use sysctl with CTL_HW and HW_VECTORUNIT instead?
	hasVFPU = (strcmp(systemArchName, "armv6") == 0);
	
#else
	int mib[2];
	size_t len;
	char *p;
	
	len = 0;
	mib[0] = CTL_HW;
	mib[1] = HW_MACHINE_ARCH;
	if (-1 == sysctl(mib, 2, NULL, &len, NULL, 0))
	{
		perror("sysctl");
		hdPrintf("errno: %d\n", errno);
	}
	
	if (len > 0)
	{
		p = (char *)malloc(len);
		if (-1 == sysctl(mib, 2, p, &len, NULL, 0))
		{
			perror("sysctl");
			hdPrintf("errno: %d\n", errno);
		}
		snprintf(systemArchName, 64, "%s", p);
		free(p);
	}
#endif

	systemInitted = true;
	
	if (0 == strcmp(systemName, "iPhone1,1") || 0 == strcmp(systemName, "iPod1,1"))
	{
		openGLVersion = 11;
	}
	else if (0 == strcmp(systemName, "iPhone2,1"))
	{
		openGLVersion = 11;
		
		// TODO: Swap to 20 if we explicitly check the version.
	}
	else
	{
#if TARGET_OS_IPHONE	
		openGLVersion = 11;
#else
		openGLVersion = 11;		
#endif
	}
}


char * SystemSettings_SystemArchName()
{
	assert(systemInitted);
	return systemArchName;
}


char * SystemSettings_SystemName()
{
	assert(systemInitted);
	return systemName;
}


bool SystemSettings_HasVFPU()
{
	return hasVFPU;
}


int SystemSettings_OpenGLVersion()
{
	assert(systemInitted);
	return openGLVersion;
}


void SystemSettings_TearDown()
{
}

@end
