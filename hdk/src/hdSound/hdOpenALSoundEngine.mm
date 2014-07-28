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

#include "hdOpenALSoundEngine.h"

#if USE_CM_OPEN_AL_SOUND == 1

static CMOpenALSoundManager *sharedCMOpenALSoundManager = nil;
typedef ALvoid	AL_APIENTRY	(*alcMacOSXMixerOutputRateProcPtr) (const ALdouble value);
#if USE_AVPLAYER_FOR_MUSIC == 1
static hdAVAudioWrapper* avAudioWrapper = nil;
#endif

void SoundEngine_Init()
{	
	static	alcMacOSXMixerOutputRateProcPtr	proc = NULL;
    
    if (proc == NULL) {
        proc = (alcMacOSXMixerOutputRateProcPtr) alcGetProcAddress(NULL, (const ALCchar*) "alcMacOSXMixerOutputRate");
    }
    
    if (proc) {
        proc(22050);
    }
	
	if (sharedCMOpenALSoundManager == nil)
	{
		sharedCMOpenALSoundManager = [[CMOpenALSoundManager alloc] init];
	}

	if (kOpenALPoolError == OpenALPool_Init())
	{
		NSLog(@"SoundEngine_Init Failed: Could not initialize OpenAL Sources pool. Bailing out...\n");
		assert(false);
	}
	
#if USE_AVPLAYER_FOR_MUSIC == 1
	UInt32	propertySize, audioIsAlreadyPlaying = 0;
	
#if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
	// do not open the track if the audio hardware is already in use (could be the iPod app playing music)
	propertySize = sizeof(UInt32);
	AudioSessionGetProperty(kAudioSessionProperty_OtherAudioIsPlaying, &propertySize, &audioIsAlreadyPlaying);
#endif
	if (audioIsAlreadyPlaying != 0)
	{
		AudioSessionSetActive(YES);
	}
	else if (avAudioWrapper == nil)
	{
		avAudioWrapper = [[hdAVAudioWrapper alloc] init];
	}
#endif
}


int SoundEngine_LoadEffect(hdSound* sound, const bool loops)
{
	assert(sharedCMOpenALSoundManager != nil);
	[sharedCMOpenALSoundManager 
		 loadSound:[NSString stringWithCString:sound->fullFilePath encoding:NSASCIIStringEncoding] 
		 doesLoop:loops 
		 withVolume:sound->volume 
		 withPitch:sound->pitchOffset];
	return 0;
}


int SoundEngine_PlayEffect(const hdSound* sound)
{
	assert(sharedCMOpenALSoundManager != nil);
	[sharedCMOpenALSoundManager playSoundWithVolumePitch:
	 [NSString stringWithCString:sound->fullFilePath encoding:NSASCIIStringEncoding] 
											withVolume:sound->volume 
											withPitch:sound->pitchOffset];
	return 0;
}


int SoundEngine_StopEffect(const hdSound* sound)
{
	assert(sharedCMOpenALSoundManager != nil);
	[sharedCMOpenALSoundManager stopSound:[NSString stringWithCString:sound->fullFilePath]];
	 return 0;
}


int SoundEngine_UnloadEffect(const hdSound* sound)
{
	assert(sharedCMOpenALSoundManager != nil);
	 return 0;
}


int SoundEngine_LoadBackgroundMusic(hdSound* sound)
{
#if USE_AVPLAYER_FOR_MUSIC == 1
	if (!avAudioWrapper) return 0;
	return [avAudioWrapper LoadSoundWithFileName:[NSString stringWithCString:sound->fullFilePath encoding:NSASCIIStringEncoding] withVolume:sound->volume];
#else
	return 0;
#endif
}



int SoundEngine_PlayBackgroundMusic(const hdSound* sound, const bool loops)
{
#if USE_AVPLAYER_FOR_MUSIC == 1
	if (!avAudioWrapper) return 0;
	return [avAudioWrapper PlaySoundWithFileName:[NSString stringWithCString:sound->fullFilePath encoding:NSASCIIStringEncoding] loops:loops];
#else
	return 0;
#endif
}


int SoundEngine_StopBackgroundMusic(const hdSound* sound)
{
#if USE_AVPLAYER_FOR_MUSIC == 1
	if (!avAudioWrapper) return 0;
	return [avAudioWrapper StopSoundWithFileName:[NSString stringWithCString:sound->fullFilePath encoding:NSASCIIStringEncoding]];
#else
	return 0;
#endif
}


int SoundEngine_PauseBackgroundMusic(const hdSound* sound)
{
#if USE_AVPLAYER_FOR_MUSIC == 1
	if (!avAudioWrapper) return 0;
	return [avAudioWrapper PauseSoundWithFileName:[NSString stringWithCString:sound->fullFilePath encoding:NSASCIIStringEncoding]];
#else
	return 0;
#endif
}


int SoundEngine_RewindBackgroundMusic(const hdSound* sound)
{
#if USE_AVPLAYER_FOR_MUSIC == 1
	if (!avAudioWrapper) return 0;
	return [avAudioWrapper RewindSoundWithFileName:[NSString stringWithCString:sound->fullFilePath encoding:NSASCIIStringEncoding] playAfterRewind:false];
#else
	return 0;
#endif
}


int SoundEngine_UnloadBackgroundMusic(const hdSound* sound)
{
#if USE_AVPLAYER_FOR_MUSIC == 1
	if (!avAudioWrapper) return 0;
	return [avAudioWrapper UnloadSoundWithFileName:[NSString stringWithCString:sound->fullFilePath encoding:NSASCIIStringEncoding]];
#else
	return 0;
#endif
}


int SoundEngine_PlayVibrationEffect(const hdSound *sound)
{
#if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
	AudioServicesPlaySystemSound(kSystemSoundID_Vibrate);
#endif
	return 0;
}


void SoundEngine_Purge()
{
	assert(sharedCMOpenALSoundManager != nil);
	[sharedCMOpenALSoundManager purgeSounds];
}


void SoundEngine_Teardown()
{
	OpenALPool_Teardown();
	
	assert(sharedCMOpenALSoundManager != nil);
	[sharedCMOpenALSoundManager purgeSounds];
	[sharedCMOpenALSoundManager release];
	sharedCMOpenALSoundManager = nil;
#if USE_AVPLAYER_FOR_MUSIC == 1	
	if (avAudioWrapper)
	{
		[avAudioWrapper release];
		avAudioWrapper = nil;
	}
#endif
}


#elif USE_OOLONG_SOUND_ENGINE == 1

void SoundEngine_Init()
{
	SoundEngine_Initialize(44100);
	SoundEngine_SetMasterVolume(1.0f);
	SoundEngine_SetListenerPosition(0.0, 0.0, 1.0);
}


int SoundEngine_LoadEffect(hdSound* sound)
{
	return SoundEngine_LoadEffect(sound->fullFilePath, (UInt32*)&sound->soundId);
}


int SoundEngine_PlayEffect(const hdSound* sound)
{
	SoundEngine_SetEffectLevel(sound->soundId, sound->volume);
	SoundEngine_SetEffectPitch(sound->soundId, sound->pitchOffset);
	return  SoundEngine_StartEffect(sound->soundId);
}


int SoundEngine_StopEffect(const hdSound* sound)
{
	SoundEngine_StopEffect(sound->soundId, false);
	return SoundEngine_UnloadEffect(sound->soundId);
}


int SoundEngine_UnloadEffect(const hdSound* sound)
{
	return SoundEngine_UnloadEffect(sound->soundId);
}


void SoundEngine_Teardown()
{
	SoundEngine_Teardown();
}

#endif


#if NO_SOUNDS == 1

void SoundEngine_Init()
{
}


int SoundEngine_LoadEffect(hdSound* sound)
{
	return 0;
}


int SoundEngine_PlayEffect(const hdSound* sound)
{
	return 0;
}


int SoundEngine_StopEffect(const hdSound* sound)
{
	return 0;
}


int SoundEngine_UnloadEffect(const hdSound* sound)
{
	return 0;
}


int SoundEngine_LoadBackgroundMusic(hdSound* sound)
{
	return 0;
}


int SoundEngine_PlayBackgroundMusic(const hdSound* sound, const bool forcePlay)
{
	return 0;
}


int SoundEngine_StopBackgroundMusic(const hdSound* sound)
{
	return 0;
}


int SoundEngine_RewindBackgroundMusic(const hdSound* sound)
{
	return 0;
}


int SoundEngine_UnloadBackgroundMusic(const hdSound* sound)
{
	return 0;
}


void SoundEngine_Purge()
{
}


void SoundEngine_Teardown()
{
}

#endif