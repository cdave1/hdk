/*
 *  hdCommon.cpp
 *  AnimationEngine
 *
 *  Created by david on 9/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdCommon.h"


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
	std::cout << msg;
#else
	printf("%s", msg);
#endif
#endif
}


void hdError(int code, const char *fmt, ...)
{
	hdPrintf("### ERROR - CODE: %d\n", code);
	
	va_list		argptr;
	static char	msg[kMaxStringSize];
	
	va_start(argptr, fmt);
	(void)vsnprintf(msg, sizeof( msg ), fmt, argptr);
	va_end(argptr);
	hdPrintf(msg);
}



#include <cstdlib>

//hdVersion hd_version = {0, 1, 0};

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