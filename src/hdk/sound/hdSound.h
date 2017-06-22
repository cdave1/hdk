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

#ifndef _HDK_SOUND_H_
#define _HDK_SOUND_H_

#include <hdk/sound/hdSoundSystemEnums.h>

class hdSound
{
public:
    hdSound() :
        volume(1.0f),
        pitchOffset(1.0f),
        length(0),
        soundId(0),
        bytes(0),
        bitRate(e_bitRateUndefined),
        type(e_soundTypeNormal),
        format(e_soundFormatUndefined),
        isBackgroundPlaying(false)
    {}

    float volume;
    float pitchOffset;
    float length;
    unsigned int soundId;
    unsigned short bytes;
    e_bitRate bitRate;
    e_soundType type;
    e_soundFormat format;
    char filename[kMaxSoundPathSize];			// game path, including extension
    char fullFilePath[kMaxSoundPathSize];
    bool isBackgroundPlaying;
};

#endif
