/*
 *  hdZFunctions.h
 *  PakMaker
 *
 *  Created by david on 2/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_COMPRESSION_H_
#define _HD_COMPRESSION_H_

#undef Byte
//#include <zconf.h>
#include <zlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "hdCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

	/*
	 * 0 if ok, < 0 if error
	 */
	extern int hdCompression_Zip(const char* inBuffer, const long inLen, 
								 char **outBuffer, long* outLen);

	/*
	 * 0 if ok, < 0 if error
	 */
	extern int hdCompression_Unzip(const char* inBuffer, const long inLen, 
								   char **outBuffer, long* outLen);

#ifdef __cplusplus
}
#endif

#endif