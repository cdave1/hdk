/*
 *  hdFileSystem.h
 *  AnimationEngine
 *
 *  Created by david on 9/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_FILESYSTEM_
#define _HD_FILESYSTEM_

#include "hdFileSystemSupport.h"

#define HD_FS_ABSOLUTE_PATH 0x0008

#ifdef __cplusplus
extern "C" {
#endif
	
	extern void FileSystem_Init(const char* baseDir);
	
	extern int FileSystem_CreateFile(const char* fileName);
	
	extern bool FileSystem_FileExists(const char* fileName);
	
	extern int FileSystem_WriteToFile(const char* fileName, const void* bytes, const unsigned long length);
	
	extern filehandle_t *FileSystem_OpenFile( const char *filename, unsigned int FlagsAndAttributes );
	
	extern void FileSystem_CloseFile( filehandle_t *fhandle );
	
	extern unsigned int FileSystem_ReadFile( void *buffer, unsigned int size, unsigned int count, filehandle_t *fhandle );
	
	extern unsigned int FileSystem_FileTell( filehandle_t *fhandle );
	
	extern unsigned int FileSystem_FileSeek( filehandle_t *fhandle, unsigned int offset, unsigned int origin );
	
	extern unsigned int FileSystem_GetFileSize( filehandle_t *fhandle );
	
	extern void FileSystem_InitFileSystem(const char* p);
	
	extern char *FileSystem_BaseDir( void );
    
	/*
	 * Returns 0 on success, -1 on fail.
	 */
	extern int FileSystem_ReadFromFile(void** dest, int* len, const char* fileName);
	
	extern void FileSystem_Teardown();
	
#ifdef __cplusplus
}
#endif

#endif