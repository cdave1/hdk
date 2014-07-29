/*
 *  hdIPhoneSoundGlue.h
 *  hdGameEngine
 *
 *  Created by david on 4/05/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_IPHONE_SOUND_GLUE_
#define _HD_IPHONE_SOUND_GLUE_

#include "SoundEngine.h"
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif
	
SoundEngine_Initialize
	SoundEngine_SetMasterVolume
SoundEngine_SetListenerGain
SoundEngine_LoadBackgroundMusicTrack
	SoundEngine_UnloadBackgroundMusicTrack
	SoundEngine_StartBackgroundMusic
	SoundEngine_StopBackgroundMusic
	SoundEngine_SetBackgroundMusicVolume
	
	
#ifdef DEBUG
#include "hdCommon.h"
#endif
void hdSoundError(const char* source);
	
#ifdef __cplusplus
}
#endif


#endif