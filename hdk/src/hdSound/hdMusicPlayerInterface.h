/*
 *  hdAvAudioInterface.h
 *  TotemGame
 *
 *  Created by david on 27/07/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_AVAUDIOINTERFACE_H_
#define _HD_AVAUDIOINTERFACE_H_

//#import "hdAVAudioWrapper.h"

#ifdef __cplusplus
extern "C" {
#endif

	extern void MusicPlayer_Init();
	extern int MusicPlayer_LoadSound(const char* fileName, const float volume);
	extern int MusicPlayer_PlaySound(const char* fileName);
	extern int MusicPlayer_StopSound(const char* fileName);
	extern int MusicPlayer_RewindSound(const char* fileName);
	extern int MusicPlayer_UnloadSound(const char* fileName);
	extern void MusicPlayer_Teardown();
	
#ifdef __cplusplus
}
#endif

#endif