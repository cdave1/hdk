/*
 * Copyright (c) 2014 Hackdirt Ltd.
 * Author: David Petrie (david@davidpetrie.com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software. Permission is granted to anyone to use this software for
 * any purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 * that you wrote the original software. If you use this software in a product, an
 * acknowledgment in the product documentation would be appreciated but is not
 * required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
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
    snprintf(systemName, 256, "%s", u.machine);

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
