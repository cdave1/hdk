/*
 *  hdOpenALSoundEngine.h
 *  TotemGame
 *
 *  Created by david on 6/08/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_OPENAL_SOUND_ENGINE_H_
#define _HD_OPENAL_SOUND_ENGINE_H_

#import "hdSoundEngineInterface.h"
#import "hdOpenALPool.h"

#if TARGET_OS_MACOSX == 1
#warning "Mac OSX sounds"
	#define USE_CM_OPEN_AL_SOUND 1
	#define USE_AVPLAYER_FOR_MUSIC 0
#elif TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
	#define USE_CM_OPEN_AL_SOUND 1
	#define USE_AVPLAYER_FOR_MUSIC 1
#else
#warning "Sounds disabled in this build..."
	#define NO_SOUNDS 0
#endif


#if USE_CM_OPEN_AL_SOUND == 1
	#import "CMOpenALSoundManager.h"
#endif

#if USE_AVPLAYER_FOR_MUSIC == 1
	#import "hdAVAudioWrapper.h"
#endif

#if USE_OOLONG_SOUND_ENGINE == 1
#import "SoundEngine.h"
#endif




#endif