/*
 *  hdTypes.h
 *  AnimationEngine
 *
 *  Created by david on 9/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_TYPES_H_
#define _HD_TYPES_H_

#ifdef _FIXED32_AS_HD_FIXED_ 
#include "hdFixed.h"
#else
typedef float fixed32;
#endif

typedef float float32;

typedef float hdFloat;

typedef signed char	int8;
typedef signed short int16;
typedef signed int int32;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

#if TARGET_RT_64_BIT == 1
//typedef long unsigned int uint64;
#endif

typedef float hdTimeInterval;


#endif