/*
 *  hdSoundEffectInterface.h
 *  TotemGame
 *
 *  Created by david on 6/08/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_SOUNDENGINEINTERFACE_H_
#define _HD_SOUNDENGINEINTERFACE_H_

#include "hdSound.h"



#ifdef __cplusplus
extern "C" {
#endif
	
	extern void SoundEngine_Init();
	
	extern int SoundEngine_LoadEffect(hdSound* effect, const bool loops);
	
	extern int SoundEngine_PlayEffect(const hdSound* effect);
	
	extern int SoundEngine_StopEffect(const hdSound* effect);
	
	extern int SoundEngine_UnloadEffect(const hdSound* effect);
	
	extern int SoundEngine_LoadBackgroundMusic(hdSound* sound);
	
	extern int SoundEngine_PlayBackgroundMusic(const hdSound* sound, 
											   const bool loops);
	
	extern int SoundEngine_StopBackgroundMusic(const hdSound* sound);
	
	extern int SoundEngine_PauseBackgroundMusic(const hdSound* sound);
	
	extern int SoundEngine_RewindBackgroundMusic(const hdSound* sound);
	
	extern int SoundEngine_UnloadBackgroundMusic(const hdSound* sound);
	
	extern int SoundEngine_PlayVibrationEffect(const hdSound *sound);
	
	extern void SoundEngine_Purge();
	extern void SoundEngine_Teardown();
	
	
	
#ifdef __cplusplus
}
#endif

#endif