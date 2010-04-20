/*
 *  hdIPhoneFS.h
 *  TotemGame
 *
 *  Created by david on 30/07/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_READWRITEFS_H_
#define _HD_READWRITEFS_H_


#ifdef __cplusplus
extern "C" {
#endif
	
	extern void ReadWriteFS_Init();
	extern int ReadWriteFS_CreateFile(const char* filepath);
	extern int ReadWriteFS_WriteToFile(const char* filepath, const char* bytes, const unsigned long length);
	extern void ReadWriteFS_Teardown();
	
#ifdef __cplusplus
}
#endif

#endif