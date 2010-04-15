/*
 *  pakProjectLoader.h
 *  PakMaker
 *
 *  Created by david on 2/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef PAK_PROJECT_LOADER_H
#define PAK_PROJECT_LOADER_H



#include <stdio.h>
#include "hdCommon.h"
#include "hdPakFileSystem.h"

extern void PakLoader_CreatePak(const char* dest, const char* pakFileListFile, const char *destDir);


extern void PakLoader_TestPak(const char* baseDir, const char *testFile);

extern void ProjectLoader_LoadProject(const char* path);



#endif