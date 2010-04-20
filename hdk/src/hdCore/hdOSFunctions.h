/*
 *  hdOSFunctions.h
 *  TotemGame
 *
 *  Created by david on 20/09/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_OS_FUNCTIONS_H
#define HD_OS_FUNCTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

	extern void * OSFunctions_PrepareThreadResources();
	
	extern void OSFunctions_TearDownThreadResources(void *object);
	
#ifdef __cplusplus
}
#endif

#endif