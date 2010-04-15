/*
 *  hdSoundSystemEnums.h
 *  hdk
 *
 *  Created by David Petrie on 10/04/10.
 *  Copyright 2010 Hackdirt Ltd. All rights reserved.
 *
 */

#ifndef HD_SOUNDSYSTEM_ENUMS_H
#define HD_SOUNDSYSTEM_ENUMS_H

#define		kMaxSounds	512
#define kMaxSoundPathSize 256

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