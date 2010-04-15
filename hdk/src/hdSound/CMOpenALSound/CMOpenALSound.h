//
//  CMOpenALSound.h
//
//  Created by Alex Restrepo on 5/19/09.
//  Copyright 2009 Colombiamug. All rights reserved.
//
//	Portions of this code are adapted from Apple's oalTouch example and
//	http://www.gehacktes.net/2009/03/iphone-programming-part-6-multiple-sounds-with-openal/
//
//  Permission is given to use this source code file, free of charge, in any
//  project, commercial or otherwise, entirely at your risk, with the condition
//  that any redistribution (in part or whole) of source code must retain
//  this copyright and permission notice. Attribution in compiled projects is
//  appreciated but not required.

#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <OpenAL/al.h>
#import <OpenAL/alc.h>
#import "hdOpenALPool.h"


#define kRetainedSourceCount 2

@interface CMOpenALSound : NSObject 
{
	ALuint			bufferID;		
	ALuint			currSourceID;			// current source ID
	ALdouble		duration;			//duration of the sound in seconds
	ALfloat			volume;				//volume [0 - 1]
	ALfloat			pitch;				//speed
	bool			m_loops;
	ALenum			error;				
	ALvoid			*bufferData;		//holds the actual sound data
	bool			isLoaded;
	CFTimeInterval  lastPlayTimeStamp;

	int		retainSourceCurrent;
	ALuint	retainedSources[kRetainedSourceCount];
}

@property (nonatomic, readonly) ALenum error;
@property (nonatomic, readonly) ALdouble duration;
@property (nonatomic) ALfloat volume;
@property (nonatomic) ALfloat pitch;

- (id) initWithSoundFile:(NSString *)file doesLoop:(BOOL)loops;

- (BOOL) play;
- (BOOL) stop;
- (BOOL) pause;
- (BOOL) rewind;
- (BOOL) isPlaying;
@end
