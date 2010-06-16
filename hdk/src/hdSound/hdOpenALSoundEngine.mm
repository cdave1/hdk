/*
 *  hdOpenALSoundEngine.cpp
 *  TotemGame
 *
 *  Created by david on 6/08/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
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
    
    if (proc)
        proc(22050);
	
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


/*
 - (void) loadSound:(NSString *)file withVolume:(float)volume withPitch:(float)pitch;
 - (void) playSound:(NSString *)file;
 - (void) playSoundWithVolumePitch:(NSString *)file withVolume:(float)volume withPitch:(float)pitch;
 - (void) stopSound:(NSString *)file;
 - (void) pauseSound:(NSString *)file;
 - (void) rewindSound:(NSString *)file;
 - (BOOL) isPlayingSound:(NSString *)file;
 */
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
	 return 0; //SoundEngine_UnloadEffect(sound->soundId);
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
#if USE_AVPLAYER_FOR_MUSIC == 1
	//assert(avAudioWrapper != nil);
#endif
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
	//	 [[CMOpenALSoundManager sharedCMOpenALSoundManager] shutdownOpenAL];
}





/*
 - (void) playBackgroundMusic:(NSString *)file;
 - (void) playBackgroundMusic:(NSString *)file forcePlay:(BOOL)forcePlay; //if forcePlay is YES, iPod audio will be stopped.
 - (void) stopBackgroundMusic;
 - (void) pauseBackgroundMusic;
 - (void) resumeBackgroundMusic;
 */
/*
int SoundEngine_LoadBackgroundMusic(hdSound* sound)
{
	assert(sharedCMOpenALSoundManager != nil);
	// Don't do anything.
	return 0;
}


int SoundEngine_PlayBackgroundMusic(const hdSound* sound, const bool forcePlay)
{
	assert(sharedCMOpenALSoundManager != nil);
	[sharedCMOpenALSoundManager playBackgroundMusic:[NSString stringWithCString:sound->fullFilePath] 
										  forcePlay:NO withVolume:sound->volume];
	return 0;
}


int SoundEngine_StopBackgroundMusic(const hdSound* sound)
{
	assert(sharedCMOpenALSoundManager != nil);
	[sharedCMOpenALSoundManager stopBackgroundMusic];
	return 0;
}


int SoundEngine_RewindBackgroundMusic(const hdSound* effect)
{
	assert(sharedCMOpenALSoundManager != nil);
	
	return 0;
}


int SoundEngine_UnloadBackgroundMusic(const hdSound* effect)
{
	assert(sharedCMOpenALSoundManager != nil);
	return 0;
}


void SoundEngine_Purge()
{
	assert(sharedCMOpenALSoundManager != nil);
	[sharedCMOpenALSoundManager purgeSounds];
}


void SoundEngine_Teardown()
{
	assert(sharedCMOpenALSoundManager != nil);
	[sharedCMOpenALSoundManager purgeSounds];
	[sharedCMOpenALSoundManager release];
	sharedCMOpenALSoundManager = nil;
//	 [[CMOpenALSoundManager sharedCMOpenALSoundManager] shutdownOpenAL];
}
	*/ 
	 
	 

#elif USE_OOLONG_SOUND_ENGINE == 1

void SoundEngine_Init()
{
	/*
	 AudioSessionInitialize(NULL,NULL,NULL,NULL);
	 unsigned int sessionCategory = kAudioSessionCategory_MediaPlayback;
	 int audio_ret = AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
	 audio_ret |= AudioSessionSetActive(true);
	 */
	
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

/*
 - (void) loadSound:(NSString *)file withVolume:(float)volume withPitch:(float)pitch;
 - (void) playSound:(NSString *)file;
 - (void) playSoundWithVolumePitch:(NSString *)file withVolume:(float)volume withPitch:(float)pitch;
 - (void) stopSound:(NSString *)file;
 - (void) pauseSound:(NSString *)file;
 - (void) rewindSound:(NSString *)file;
 - (BOOL) isPlayingSound:(NSString *)file;
 */
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
	return 0; //SoundEngine_UnloadEffect(sound->soundId);
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