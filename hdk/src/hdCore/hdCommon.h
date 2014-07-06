/*
 *  hdCommon.h
 *  AnimationEngine
 *
 *  Created by david on 9/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 * Common functions
 */

#ifndef _HD_COMMON_H_
#define _HD_COMMON_H_

#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <assert.h>

#include "hdTypes.h"

#define hdAssert(A) assert(A)
#define kMaxStringSize 4096

#define HDCALLBACK

/*
 /// Version numbering scheme.
 /// See http://en.wikipedia.org/wiki/Software_versioning
 struct hdVersion
 {
 int32 major;		///< significant changes
 int32 minor;		///< incremental changes
 int32 revision;		///< bug fixes
 };
 
 /// Current version.
 extern hdVersion hd_version;
 */

#ifdef __cplusplus
extern "C" {
#endif

	// Memory Allocation
	//
	/// The current number of bytes allocated through b2Alloc.
	extern int32 hd_byteCount;
	
	void* hdAlloc(int32 size);
	
	void hdFree(void* mem);
		
    extern void hdPrintf( const char *fmt, ... );
	
	extern void hdPrintfv(const char *fmt, va_list list);
    
    extern void hdError( int code, const char *fmt, ... );
	
#ifdef __cplusplus
}
#endif

#ifdef DEBUG
#define hdLog(xx, ...) hdPrintf(xx, ##__VA_ARGS__)
#else
#define hdLog(xx, ...) ((void)0)
#endif

#endif