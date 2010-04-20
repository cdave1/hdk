/*
 *  hdOpenALPool.cpp
 *  TotemGame
 *
 *  Created by david on 8/08/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdOpenALPool.h"
#include "hdCommon.h"
#include "hdCollections.h"



typedef struct hdOpenALSource;


struct hdOpenALSource
{
	hdOpenALSource() : sourceId(0), owner(NULL) {}
	
	hdOpenALSource(ALuint sourceId, void *owner) : sourceId(sourceId), owner(owner) {}
	
	ALuint sourceId;
	void *owner;
};



static ALuint			m_freeSources[kFreeSourcePoolSize];



static int				m_nextFreeSourcePos;

static ALuint			m_ownerSources[kOwnerSourcePoolSize];

static hdTypedefList<hdOpenALSource, kOwnerSourcePoolSize>* m_ownerSourcePool = NULL;

static ALuint			m_bufferPool[kBufferPoolSize];

static int				m_nextBufferPos;


static bool m_isActive = false;

static bool m_lock = false;


int OpenALPool_Init()
{
	ALenum error;
	unsigned i;
	
	// Only create the source pool once
	if (m_isActive) return 0;
	
	m_ownerSourcePool = new hdTypedefList<hdOpenALSource, kOwnerSourcePoolSize>();
	
	memset(m_ownerSources, 0, sizeof(m_ownerSources));
	memset(m_freeSources, 0, sizeof(m_freeSources));
	
	
	alGenSources(kOwnerSourcePoolSize, m_ownerSources);
	alGenSources(kFreeSourcePoolSize, m_freeSources);
	
	if((error = alGetError()) != AL_NO_ERROR) 
	{
		hdPrintf("error generating sources: %d\n", error);
		return kOpenALPoolError;
	}
	
	memset(m_bufferPool, 0, sizeof(m_bufferPool));
	alGenBuffers(kBufferPoolSize, m_bufferPool);
	
	if((error = alGetError()) != AL_NO_ERROR) 
	{
		hdPrintf("error generating buffers: %d\n", error);
		return kOpenALPoolError;
	}
	
	for (i = 0; i < kOwnerSourcePoolSize; ++i)
	{
		hdAssert(-1 != m_ownerSourcePool->Add(hdOpenALSource()));
	}
	
	m_nextFreeSourcePos = 0;
	m_nextBufferPos = 0;
	m_isActive = true;
	return 0;
}


ALuint OpenALPool_GetNextBuffer()
{
	ALuint next;
	
	ACQUIRE_LOCK;
	ASSERT_LOCKED;
	
	if (!m_isActive) OpenALPool_Init();
	
	assert(m_nextBufferPos >= 0 && m_nextBufferPos < kBufferPoolSize);
	next = m_bufferPool[m_nextBufferPos];
	
	m_nextBufferPos = (m_nextBufferPos+1) & ((kBufferPoolSize)-1);
	
#ifdef OPENAL_POOL_DEBUG
	hdPrintf("OpenALPool: Got a new buffer %d\n", next);
#endif
	
	RELEASE_LOCK;
	ASSERT_UNLOCKED;
	
	return next;
}

/*
ALuint OpenALPool_GetNextSource()
{	
	ALuint next;
	ALenum state;
	
	if (!m_isActive) OpenALPool_Init();
	
	assert(m_nextBufferPos >= 0 && m_nextBufferPos < kBufferPoolSize);
	
	next = m_bufferPool[m_nextBufferPos];
	
	m_nextBufferPos = (m_nextBufferPos+1) & ((kBufferPoolSize)-1);


	
	
#ifdef OPENAL_POOL_DEBUG	
	hdPrintf("OpenALPool: Got a new source %d\n", next);
#endif
	
	return next;
}*/


ALuint OpenALPool_GetFreeSource()
{
	ALuint next;
	ALenum state;
	
	ACQUIRE_LOCK;
	ASSERT_LOCKED;
	
	if (!m_isActive) OpenALPool_Init();
	
	assert(m_nextFreeSourcePos >= 0 && m_nextFreeSourcePos < kFreeSourcePoolSize);
	
	next = m_freeSources[m_nextFreeSourcePos];
	
	m_nextFreeSourcePos = (m_nextFreeSourcePos+1) & ((kFreeSourcePoolSize)-1);
	
	alGetSourcei(next, AL_SOURCE_STATE, &state);
	if (state == AL_PLAYING)
	{
		alSourceStop(next);
	}
	
#ifdef OPENAL_POOL_DEBUG
	hdPrintf("OpenALPool: Got a new buffer %d\n", next);
#endif
	
	RELEASE_LOCK;
	ASSERT_UNLOCKED;
	
	return next;
	
	

}


ALuint OpenALPool_RetainSource(const void *owner)
{
	int i;
	ALuint sourceId;
	
	ACQUIRE_LOCK;
	ASSERT_LOCKED;
	
	if (!m_isActive) OpenALPool_Init();
	
	hdAssert(m_ownerSourcePool != NULL);
	
	if (owner == NULL)
	{
		hdPrintf("[OpenALPool_RetainSource]: Can't retain a source with NULL owner.\n");
		return -1;		
	}
	
	// Has a source already been allocated for owner? Return it, if so.
#if 0
	for (i = 0; i < kOwnerSourcePoolSize; ++i)
	{
		if (m_ownerSourcePool->GetItems()[i].owner == owner)
		{
			return m_ownerSourcePool->GetItems()[i].sourceId;
		}
	}
#endif
	
	// Find a source not already taken
	i = -1;
	for (i = 0; i < kOwnerSourcePoolSize; ++i)
	{
		if (m_ownerSourcePool->GetItems()[i].owner == NULL)
		{
			break;
		}
	}
	
	if (-1 == i)
	{
		hdPrintf("[OpenALPool_RetainSource]: Couldn't retain a source because all available sources have been allocated.\n");
		return -1;
	}
	
	sourceId = m_ownerSources[i];
	m_ownerSourcePool->GetItems()[i].sourceId = sourceId;
	m_ownerSourcePool->GetItems()[i].owner = (void *)owner;
	
	RELEASE_LOCK;
	ASSERT_UNLOCKED;
	
	return sourceId;
}


// Just return false if the source could not be found.
extern bool OpenALPool_ReleaseSource(ALuint source)
{
	int i = -1;
	ACQUIRE_LOCK;
	ASSERT_LOCKED;
	
	if (!m_isActive) 
	{
		hdPrintf("[OpenALPool_ReleaseSource] Failed to release source %d because the OpenALPool has not been initialised.\n", source);
		return false;
	}
	
	for (i = 0; i < m_ownerSourcePool->GetItemCount(); ++i)
	{
		if (m_ownerSourcePool->GetItems()[i].sourceId == source)
		{
			break;
		}
	}
	
	if (-1 == i)
	{
		hdPrintf("[OpenALPool_ReleaseSource] Couldn't find a source to remove for %d.\n", source);
		return false;
	}
	
	m_ownerSourcePool->GetItems()[i].sourceId = -1;
	m_ownerSourcePool->GetItems()[i].owner = NULL;
	
	RELEASE_LOCK;
	ASSERT_UNLOCKED;
	
	return true;
}


int OpenALPool_Teardown()
{
	ALenum error;
	
	ACQUIRE_LOCK;
	ASSERT_LOCKED;
	
	hdPrintf("Deleting OpenAL Source Pool\n");
	
	// Delete owner sources
	for (int i = 0; i < kOwnerSourcePoolSize; ++i)
	{
		hdPrintf("Stopping owner sounds...\n");
		alSourceStop(m_ownerSources[i]);
	}
	
	alDeleteSources(kOwnerSourcePoolSize, m_ownerSources);
	
	if((error = alGetError()) != AL_NO_ERROR) 
	{
		hdPrintf("error tearing down source pool: %x\n", error);
	}
	
	
	// Delete free sources
	for (int i = 0; i < kFreeSourcePoolSize; ++i)
	{
		hdPrintf("Stopping free sounds...\n");
		alSourceStop(m_freeSources[i]);
	}
	alDeleteSources(kFreeSourcePoolSize, m_freeSources);
	
	if((error = alGetError()) != AL_NO_ERROR) 
	{
		hdPrintf("error tearing down source pool: %x\n", error);
	}
	
	hdPrintf("Deallocing OpenAL Buffer Pool\n");
	
	alDeleteBuffers(kBufferPoolSize, m_bufferPool);
	
	m_isActive = false;
	
	delete m_ownerSourcePool;
	
	RELEASE_LOCK;
	ASSERT_UNLOCKED;
	
	return 0;
}