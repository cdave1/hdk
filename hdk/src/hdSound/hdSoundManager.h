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

#ifndef _HD_SOUND_MANAGER_
#define _HD_SOUND_MANAGER_

#include "hdMath.h"
#include "hdCore.h"
#include "hdCollections.h"
#include "hdSound/hdSoundEngineInterface.h"
#include "hdSound/hdSound.h"
#include "hdSound/hdSoundSystemEnums.h"

/*
 * A set of static methods, wrapped around an engine that deals with sound.
 */
class hdSoundManager
{
public:

    static void CheckSoundStatus(const char *location);

    static void TearDown();

    static void InitSoundManager();

    static hdSound* FindSound(const char* filename, e_soundType type);

    static hdSound* LoadSound(const char* filepath, e_soundType type);

    static hdSound* LoadLoopingSound(const char* filepath, const char* attackFilepath, const char* decayFilePath);

    static hdSound* GetSound(const int location);

    static bool PlaySound(hdSound* sound);

    static bool StopSound(hdSound* sound);

    static bool UnloadSound(hdSound *sound);

    static hdSound* GetNullSound();

    static void DestroySounds();

    static void SetMusicMaxVolume(const float vol);

    static void SetSoundMaxVolume(const float vol);
    
    static void SetVibrationsOn();
    
    static void SetVibrationsOff();

};


#endif