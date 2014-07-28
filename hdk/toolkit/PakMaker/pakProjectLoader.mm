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

#include "pakProjectLoader.h"

#include "hdPakFileSystem.h"
#import <Foundation/Foundation.h>

void PakLoader_CreatePak(const char* baseDir, const char* pakManifestFileName, const char *destDir)
{
    NSAutoreleasePool *pool;
    pool = [[NSAutoreleasePool alloc] init];
    FileSystem_Init(baseDir);
    hdPakFileSystem::MakePak(baseDir, pakManifestFileName, destDir);
    FileSystem_Teardown();
    [pool drain];
}


void PakLoader_TestPak(const char* baseDir, const char *testFile)
{
    char *pakBuf, *realBuf;
    char tmp[256];
    int pakLen, realLen;

    NSAutoreleasePool *pool;
    pool = [[NSAutoreleasePool alloc] init];
    FileSystem_Init(baseDir);

    hdPakFileSystem::InitPakFileSystem(baseDir);

    if (0 != hdPakFileSystem::OpenFile(testFile, &pakBuf, &pakLen))
    {
        hdPrintf("Couldn't open file.\n");
    }

    snprintf(tmp, 256, "%s%s", baseDir, testFile);
    FileSystem_ReadFromFile((void **)&realBuf, &realLen, tmp);

    hdAssert(pakLen == realLen);

    hdAssert(strncmp(pakBuf, realBuf, realLen) == 0);

    printf("PakLoader test passed for: %s\n", testFile);
    
    FileSystem_Teardown();
    [pool drain];
}


void ProjectLoader_LoadProject(const char* path)
{
}
