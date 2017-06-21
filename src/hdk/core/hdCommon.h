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

#include <hdk/hdCore/hdTypes.h>

#define hdAssert(A) assert(A)

#define HDCALLBACK

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