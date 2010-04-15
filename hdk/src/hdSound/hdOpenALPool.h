/*
 *  hdOpenALPool.h
 *  TotemGame
 *
 *  Created by david on 8/08/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 * Free sources - a cycling pool of sources.
 * Retained sources - a set of sources that are "owned" by a 
 * particular sound. Intended to be used by frequently played sounds,
 * or looping sounds.
 *
 * This class needs to be carefully uncoupled from the rest of the 
 * sound application - other stuff should be fail safe.
 */

#ifndef _HD_OPENAL_SOURCE_POOL_
#define _HD_OPENAL_SOURCE_POOL_

#include <AudioToolbox/AudioToolbox.h>
#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#define kFreeSourcePoolSize 8
#define kOwnerSourcePoolSize 24
#define kBufferPoolSize 128

#define kOpenALPoolError -99

#define ACQUIRE_LOCK if (m_lock) return -1; m_lock = true;
#define RELEASE_LOCK m_lock = false;

#define ASSERT_LOCKED assert(m_lock == true);
#define ASSERT_UNLOCKED assert(m_lock == false);




#ifdef __cplusplus
extern "C" {
#endif
	
	extern int OpenALPool_Init();
	
	extern int OpenALPool_IsActive();
	
	extern ALuint OpenALPool_GetNextBuffer();
	
	//ALuint OpenALPool_GetNextSource();
	
	extern ALuint OpenALPool_GetFreeSource();
	
	extern ALuint OpenALPool_RetainSource(const void *owner);
	
	extern bool OpenALPool_ReleaseSource(ALuint source);
	
	extern int OpenALPool_Teardown();

	
#ifdef __cplusplus
}
#endif

#endif