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

#ifndef HD_SOUNDSYSTEM_ENUMS_H
#define HD_SOUNDSYSTEM_ENUMS_H

#define kMaxSounds          512
#define kMaxSoundPathSize   256

typedef enum
{
    e_bitRateUndefined = 0,
    e_bitRate44100,
    e_bitRate_Count,
} e_bitRate;


typedef enum
{
    e_soundTypeNormal = 0,
    e_soundTypeLoop,
    e_soundTypeFrequent,
    e_soundTypeBackground,
    e_soundTypeBackgroundLooping,
    e_soundTypeVibration,
    e_soundType_Count,
} e_soundType;


typedef enum
{
    e_soundFormatUndefined = 0,
    e_soundFormatCAF,
    e_soundFormatOGG,
    e_soundFormatMP3,
    e_soundFormatWAV,
    e_soundFormatAFF,
    e_soundFormat_Count,
} e_soundFormat;

#endif