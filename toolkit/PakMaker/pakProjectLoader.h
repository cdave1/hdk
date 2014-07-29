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

#ifndef PAK_PROJECT_LOADER_H
#define PAK_PROJECT_LOADER_H

#include <stdio.h>
#include "hdCommon.h"
#include "hdPakFileSystem.h"

extern void PakLoader_CreatePak(const char* dest, const char* pakFileListFile, const char *destDir);

extern void PakLoader_TestPak(const char* baseDir, const char *testFile);

extern void ProjectLoader_LoadProject(const char* path);

#endif