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

/*
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

    extern ALuint OpenALPool_GetFreeSource();

    extern ALuint OpenALPool_RetainSource(const void *owner);

    extern bool OpenALPool_ReleaseSource(ALuint source);

    extern int OpenALPool_Teardown();
    
#ifdef __cplusplus
}
#endif

#endif
