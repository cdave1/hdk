/*
 *  pakProjectLoader.mm
 *  PakMaker
 *
 *  Created by david on 2/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
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
	
	//hdPakFileSystem::PrintFileManifest();
	
	if (0 != hdPakFileSystem::OpenFile(testFile, &pakBuf, &pakLen))
	{
		hdPrintf("Couldn't open file.\n");
	}
	
	snprintf(tmp, 256, "%s%s", baseDir, testFile);
	FileSystem_ReadFromFile((void **)&realBuf, &realLen, tmp);
	
	hdAssert(pakLen == realLen);
	
	hdAssert(strncmp(pakBuf, realBuf, realLen) == 0);
	
	//printf(buf);
	printf("PakLoader test passed for: %s\n", testFile);
	
	FileSystem_Teardown();
	[pool drain];
}


void ProjectLoader_LoadProject(const char* path)
{
	/*
	char *dest;
	int len;
	NSData *bytes;
	NSKeyedUnarchiver *archiver;
	NSAutoreleasePool *pool;
	
	FileSystem_Init();
	
	pool = [[NSAutoreleasePool alloc] init];
	
	// Read in a file	
	if (0 != (FileSystem_ReadFromFile((void**)&dest, &len, path)))
	{
		printf("Failed to read data from file at %s\n", path);
		return;
	}
	
	// Load file contents into an NSKeyedArchiver
	bytes = [NSData dataWithBytesNoCopy:(void *)dest length:len];
	archiver = [NSKeyedUnarchiver unarchiveObjectWithData:bytes];
	
	// dump contents of file.
	
	
	
	[pool drain];*/
}