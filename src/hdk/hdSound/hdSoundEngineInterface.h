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

#ifndef _HD_SOUNDENGINEINTERFACE_H_
#define _HD_SOUNDENGINEINTERFACE_H_

#include <hdk/hdSound/hdSound.h>

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