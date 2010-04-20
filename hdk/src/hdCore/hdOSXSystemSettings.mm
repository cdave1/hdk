/*
 *  hdOSXSystemSettings.cpp
 *  TotemGameOSX
 *
 *  Created by david on 25/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdOSXSystemSettings.h"

@implementation hdOSXSystemSettings


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
	snprintf(systemName, 256, u.machine);
	
	/*
	 * Arch
	 */
#ifdef __arm__
	NXArchInfo *archInfo = (NXArchInfo *)NXGetLocalArchInfo();
	
	if (archInfo != NULL)
	{
#ifdef DEBUG
		printf("%s, %d, %d, %s\n", 
			   archInfo->name,
			   archInfo->cputype,
			   archInfo->cpusubtype,
			   archInfo->description);
#endif
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
		printf("errno: %d\n", errno);
	}
	
	p = (char *)malloc(len);
	if (-1 == sysctl(mib, 2, p, &len, NULL, 0))
	{
		perror("sysctl");
		printf("errno: %d\n", errno);
	}
	snprintf(systemArchName, 64, "%s", p);
	free(p);
#endif
	
	systemInitted = true;
	
	openGLVersion = 11;
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