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

#include "hdCommon.h"

#define kMaxStringSize 4096

void hdPrintf(const char *fmt, ...)
{
#ifdef DEBUG
    va_list		argptr;
    static char	msg[kMaxStringSize];

    va_start(argptr, fmt);
    (void)vsnprintf(msg, sizeof( msg ), fmt, argptr);
    va_end(argptr);

    msg[sizeof(msg) - 1] = '\0';
#ifdef LEVEL_EDITOR
    printf("%s", msg);
#else
    printf("%s", msg);
#endif
#endif
}


void hdPrintfv(const char *fmt, va_list argptr) {}


void hdError(int code, const char *fmt, ...)
{
    hdPrintf("### ERROR - CODE: %d (%x)\n", code, code);

    va_list		argptr;
    static char	msg[kMaxStringSize];

    va_start(argptr, fmt);
    (void)vsnprintf(msg, sizeof( msg ), fmt, argptr);
    va_end(argptr);
    hdPrintf(msg);
}


#include <cstdlib>

int32 hd_byteCount = 0;

void* hdAlloc(int32 size)
{
    size += 4;
    hd_byteCount += size;
    char* bytes = (char*)malloc(size);
    *(int32*)bytes = size;
    return bytes + 4;
}


void hdFree(void* mem)
{
    if (mem == NULL)
    {
        return;
    }

    char* bytes = (char*)mem;
    bytes -= 4;
    int32 size = *(int32*)bytes;
    hdAssert(hd_byteCount >= size);
    hd_byteCount -= size;
    free(bytes);
}
