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
