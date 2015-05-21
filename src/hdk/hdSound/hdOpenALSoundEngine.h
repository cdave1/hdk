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

#ifndef _HD_OPENAL_SOUND_ENGINE_H_
#define _HD_OPENAL_SOUND_ENGINE_H_

#import "hdSoundEngineInterface.h"
#import "hdOpenALPool.h"

#if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
#define USE_CM_OPEN_AL_SOUND 1
#define USE_AVPLAYER_FOR_MUSIC 1
#elif TARGET_OS_MAC == 1
#warning "Mac OSX sounds"
#define USE_CM_OPEN_AL_SOUND 1
#define USE_AVPLAYER_FOR_MUSIC 0
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