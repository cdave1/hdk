//
//  CMOpenALSound.m
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

#import "CMOpenALSound.h"
#import "AppleOpenALSupport.h"

@interface CMOpenALSound()
-(BOOL) loadSoundFile:(NSString *)file;
@end

@implementation CMOpenALSound
@synthesize error, duration;



#pragma mark -
#pragma mark init/dealloc
//initialize with a sound file...
- (id) initWithSoundFile:(NSString *)file doesLoop:(BOOL)loops
{
	self = [super init];
	if (self != nil) 
	{		
		isLoaded = NO;
		bufferData = NULL;
		m_loops = loops;
		memset(retainedSources, 0, sizeof(retainedSources));
		
		if(![self loadSoundFile:file])
		{
			hdPrintf("Failed to load the sound file: %s...", [file cStringUsingEncoding:NSASCIIStringEncoding]);
			[self release];
			return nil;
		}
		
		self.volume = 1.0;
		self.pitch = 1.0;
		currSourceID = -1;
		retainSourceCurrent = 0;
	}
	return self;
}


- (void) dealloc
{	
	// Danger Will Robinson!!
	//
	// freeing this data here will have grave consequences if the source is still playing - an
	// entirely plausible situation given that the openAL sound is running in another thread.
	//
	// Simply stopping the sound is not enough to ensure successful deletion. The solution is to
	// check the result of [isPlaying] before releasing the pointer. This appears to
	// be a good enough guard against the sound thread finding a null pointer when it plays 
	// its sounds.
	//
	// The ideal would be to have explicit critical sections for both the openAL and
	// sound management threads. However, I'm currently not sure how to do this and time
	// is well shorter than is available to research a solution.
	if(bufferData != NULL)
	{
		hdPrintf("\tPurging buffer data\n");
		free(bufferData);
		bufferData = NULL;
	}
	
	if (m_loops)
	{
		for (int i = 0; i < kRetainedSourceCount; ++i)
		{
			OpenALPool_ReleaseSource(retainedSources[i]);
		}
	}
	
	[super dealloc];
}

#pragma mark -
#pragma mark loading
-(BOOL) loadSoundFile:(NSString *)fileName
{		
	ALenum  format;
	ALsizei size;
	ALsizei freq;
	unsigned i;
	
	error = AL_NO_ERROR;
	alGetError(); //clear any previous error...
	
	//get the URL for the sound file...
	NSBundle *bundle = [NSBundle mainBundle];
	NSString *path = [bundle pathForResource:[fileName stringByDeletingPathExtension] 
									  ofType:[fileName pathExtension]];
	
	if(!path) 
	{
		// try absolute path
		path = fileName;
	}
	
	CFURLRef fileURL = (CFURLRef)[[NSURL fileURLWithPath:path] retain];
	
	if (!fileURL)
	{
		hdPrintf("file: %s not found.\n", [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
		return NO;
	}
	
	// get audio data from file
	bufferData = MyGetOpenALAudioData(fileURL, &size, &format, &freq, &duration);
	CFRelease(fileURL);		
	
	// grab a buffer ID from openAL
	isLoaded = YES;
	bufferID = OpenALPool_GetNextBuffer();
	
	// load the awaiting data blob into the openAL buffer.
	// http://developer.apple.com/iphone/library/technotes/tn2008/tn2199.html
	// Use the alBufferDataStatic API, found in the oalStaticBufferExtension.h header file, 
	// instead of the standard alBufferData function. This eliminates extra buffer copies by allowing your application to own
	// the audio data memory used by the buffer objects.
	alBufferDataStaticProc(bufferID, format, bufferData, size, freq);	

	if (m_loops)
	{
		for (i = 0; i < kRetainedSourceCount; ++i)
		{
			retainedSources[i] = OpenALPool_RetainSource(self);
			if((error = alGetError()) != AL_NO_ERROR) 
			{
				hdPrintf("error loading retained source %d: %d\n", i, error);
				return NO;
			}
			
			if (retainedSources[i] == -1)
			{
				hdPrintf("error loading sound: no sources were available for looping sound.\n", error);
				return NO;			
			}
			
			alSourcei(retainedSources[i], AL_BUFFER, 0);
			alSourcei(retainedSources[i], AL_BUFFER, bufferID);
			
			if((error = alGetError()) != AL_NO_ERROR) 
			{
				hdPrintf("error binding retained source %d to buffer: %d\n", i, error);
				return NO;
			}
			
			alSourcei(retainedSources[i], AL_SOURCE_RELATIVE, AL_TRUE);
			alSource3f(retainedSources[i], AL_POSITION, 0.0f, 0.0f, 0.0f);
		}
	}
	
	
	if((error = alGetError()) != AL_NO_ERROR) 
	{
		hdPrintf("error loading sound: %d\n", error);
		return NO;
	}
	
	return YES;	
}


#pragma mark -
#pragma mark playback
- (BOOL) play
{
	ALenum state;
	unsigned i;
	
	if (m_loops)
	{
		
		/*
		 if (m_loops)
		 {
		 alSourcei(currSourceID, AL_LOOPING, AL_TRUE);
		 }
		 else
		 {
		 alSourcei(currSourceID, AL_LOOPING, AL_FALSE);
		 }*/
		
		// First non-playing sound wins.
		/*retainSourceCurrent
		for (i = 0; i < kRetainedSourceCount; ++i)
		{
			alGetSourcei(retainedSources[i], AL_SOURCE_STATE, &state);
			if (state != AL_PLAYING)
			{
				currSourceID = retainedSources[i];
				alSourcePlay(currSourceID);
				break;
			}
		}*/
		
		alSourceStop(retainedSources[retainSourceCurrent]);
		currSourceID = retainedSources[retainSourceCurrent];
		

		alSourcePlay(currSourceID);
		//alSourcef(currSourceID, AL_GAIN, volume);
		//alSourcef(currSourceID, AL_PITCH, pitch);
		
		
		++retainSourceCurrent;
		if (retainSourceCurrent >= kRetainedSourceCount)
			retainSourceCurrent = 0;
	}
	else
	{
		//return YES;
		
		currSourceID = OpenALPool_GetFreeSource();
	
		if (currSourceID == -1)
		{
			hdPrintf("Could not play sound: %d\n", error);
			return NO;		
		}
	
		alSourcei(currSourceID, AL_BUFFER, 0);
		alSourcei(currSourceID, AL_BUFFER, bufferID);
		
		if((error = alGetError()) != AL_NO_ERROR) 
		{
			hdPrintf("error playing sound: %d\n", error);
			return NO;
		}
		
		
		alSourcePlay(currSourceID);
		
		if((error = alGetError()) != AL_NO_ERROR) 
		{
			hdPrintf("error playing sound: %d\n", error);
			return NO;
		}
	}
	
	lastPlayTimeStamp = CFAbsoluteTimeGetCurrent();
	return YES;
}


- (BOOL) stop
{
	alSourceStop(currSourceID);
	return ((error = alGetError()) != AL_NO_ERROR);
}


- (BOOL) pause
{
	alSourcePause(currSourceID);
	return ((error = alGetError()) != AL_NO_ERROR);
}


- (BOOL) rewind
{
	lastPlayTimeStamp = CFAbsoluteTimeGetCurrent();
	alSourceRewind(currSourceID);
	return ((error = alGetError()) != AL_NO_ERROR);
}


// returns whether the BASE source is playing or not

// If time between last play timestamp and now is less
// than duration, then it's still playing.
- (BOOL) isPlaying
{	
	if ((lastPlayTimeStamp + 0.05) - CFAbsoluteTimeGetCurrent() > duration)
	{
		ALenum state;	
		alGetSourcei(currSourceID, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);		
	}
	else
	{
		return YES;
	}
}

#pragma mark -
#pragma mark properties
- (ALfloat) volume
{
	return volume;
}


- (void) setVolume:(ALfloat)newVolume
{
	volume = MAX(MIN(newVolume, 1.0f), 0.0f); //cap to 0-1
	alSourcef(currSourceID, AL_GAIN, volume);
}


- (ALfloat) pitch
{
	return pitch;
}


- (void) setPitch:(ALfloat)newPitch
{
	pitch = newPitch;
	alSourcef(currSourceID, AL_PITCH, pitch);
}
@end
