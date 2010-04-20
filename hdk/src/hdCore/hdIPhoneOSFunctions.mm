/*
 *  hdIPhoneThreadFunctions.cpp
 *  TotemGame
 *
 *  Created by david on 20/09/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdIPhoneOSFunctions.h"


void* OSFunctions_PrepareThreadResources()
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	return pool;
}

void OSFunctions_TearDownThreadResources(void *object)
{
	[((NSAutoreleasePool *)object) drain];
}