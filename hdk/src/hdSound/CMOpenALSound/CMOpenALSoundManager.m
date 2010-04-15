//
//  CMOpenALSoundManager.m
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

#import "CMOpenALSoundManager.h"
#import "CMOpenALSound.h"
#import "SynthesizeSingleton.h"

@interface CMOpenALSoundManager()
@property (nonatomic, retain) NSMutableDictionary *soundDictionary;
@property (nonatomic, retain) CMOpenALSound *backgroundAudio;
@end

@interface CMOpenALSoundManager(private)
- (BOOL) initOpenAL;
- (void) cleanUpAudioChannel;
- (NSString *) keyForSoundID:(NSUInteger)soundID;
@end

@implementation CMOpenALSoundManager
@synthesize soundDictionary, backgroundAudio, isiPodAudioPlaying;

#if USE_AS_SINGLETON
SYNTHESIZE_SINGLETON_FOR_CLASS(CMOpenALSoundManager);
#endif

#pragma mark -
#pragma mark init/dealloc
- (id) init
{
	self = [super init];		
	if (self != nil) 
	{
	#if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
		//session info
		AudioSessionInitialize(NULL, NULL, NULL, NULL);	
#endif
		//[self checkIfiPodIsPlaying];
		
		if(![self initOpenAL])
		{
			NSLog(@"OpenAL initialization failed!!");
			[self release];
			self = nil;
			return nil;
		}		
		
		self.soundDictionary = [NSMutableDictionary dictionary];
		self.soundEffectsVolume = 1.0;
		//self.backgroundMusicVolume = 1.0;
	}
	return self;
}

// start up openAL
-(BOOL) initOpenAL
{
	ALCcontext	*context = NULL;
	ALCdevice	*device = NULL;

	// Initialization
	device = alcOpenDevice(NULL); // select the "preferred device"
	if(!device) return NO;
		
	// use the device to make a context
	context = alcCreateContext(device, NULL);
	if(!context) return NO;
	
	// set my context to the currently active one
	alcMakeContextCurrent(context);
	
	NSLog(@"oal inited ok");
	return YES;	
}

- (void) dealloc
{
	[self shutdownOpenAL];
	
	[backgroundAudio release];
	[soundDictionary release];
	[super dealloc];
}

- (void) shutdownOpenAL
{
	self.backgroundAudio = nil;
	self.soundDictionary = nil;
	
	ALCcontext	*context = NULL;
    ALCdevice	*device = NULL;
	
	//Get active context (there can only be one)
    context = alcGetCurrentContext();
	
	if (context)
	{
		//Get device for active context
		device = alcGetContextsDevice(context);
	
		alcMakeContextCurrent(NULL);
	
		//Release context
		alcDestroyContext(context);
	}
	
	if (device)
	{
		//Close device
		alcCloseDevice(device);
	}
}

#pragma mark -
#pragma mark audio session mgmt

//code from: http://www.idevgames.com/forum/showthread.php?p=143030
//this will return YES if another audio source is active (iPod app)
- (void) checkIfiPodIsPlaying
{
	UInt32	propertySize, audioIsAlreadyPlaying;
	
#if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
	// do not open the track if the audio hardware is already in use (could be the iPod app playing music)
	propertySize = sizeof(UInt32);
	AudioSessionGetProperty(kAudioSessionProperty_OtherAudioIsPlaying, &propertySize, &audioIsAlreadyPlaying);	
	
	if (audioIsAlreadyPlaying != 0 && ![self isBackGroundMusicPlaying]) //audio could be ours...
	{
		isiPodAudioPlaying = YES;
		
		UInt32	sessionCategory = kAudioSessionCategory_AmbientSound;
		AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
		AudioSessionSetActive(YES);
	}
	else
	{
		[self cleanUpAudioChannel];
	}
#endif
}

- (void) cleanUpAudioChannel
{	
	isiPodAudioPlaying = NO;
#if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1	
	// since no other audio is *supposedly* playing, then we will make darn sure by changing the audio session category temporarily
	// to kick any system remnants out of hardware (iTunes (or the iPod App, or whatever you wanna call it) sticks around)
	UInt32	sessionCategory = kAudioSessionCategory_MediaPlayback;
	AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
	AudioSessionSetActive(YES);
	
	// now change back to ambient session category so our app honors the "silent switch"
	sessionCategory = kAudioSessionCategory_AmbientSound;
	AudioSessionSetProperty(kAudioSessionProperty_AudioCategory, sizeof(sessionCategory), &sessionCategory);
#endif
}

#pragma mark -
#pragma mark cleanup
- (void) purgeSounds
{
	NSArray *keys = [soundDictionary allKeys];
	CMOpenALSound *sound;
	
	for (NSObject *k in keys)
	{
		sound = [soundDictionary objectForKey:k];
		
		if ([sound isPlaying] == NO)
		{
			[soundDictionary removeObjectForKey:k];
		}
	}
	
	//if there's a background audio that's not playing, remove that too...
	//if(![backgroundAudio isPlaying])
	//	self.backgroundAudio = nil;
}

#pragma mark -
#pragma mark background music playback
// plays a file as the background audio...
- (void) playBackgroundMusic:(NSString *)file withVolume:(float)volume
{
	[self playBackgroundMusic:file forcePlay:NO withVolume:volume];
}

- (void) playBackgroundMusic:(NSString *)file forcePlay:(BOOL)forcePlay withVolume:(float)volume
{
	[backgroundAudio stop]; //if there's audio already playing...
		
	if(forcePlay)	//if we want to kill other audio sources, like the iPod...
		[self cleanUpAudioChannel];
	
	if(isiPodAudioPlaying) //if other background audio is playing bail out...
		return;
	
	CMOpenALSound *audio = [[CMOpenALSound alloc] initWithSoundFile:file doesLoop:YES];
	[audio play];
	audio.volume = volume;// self.backgroundMusicVolume;
	
	self.backgroundAudio = audio;
	[audio release];
}

- (void) stopBackgroundMusic
{
	[backgroundAudio stop];
}

- (void) pauseBackgroundMusic
{
	[backgroundAudio pause];
}

- (void) resumeBackgroundMusic
{
	[backgroundAudio play];
}

#pragma mark -
#pragma mark effects playback
// grab the filename (key) from the filenames array
- (NSString *) keyForSoundID:(NSUInteger)soundID
{
	//if(soundID < 0 || soundID >= [soundFileNames count])
	//	return nil;
	
	//return [[soundFileNames objectAtIndex:soundID] lastPathComponent];
	return @"";
}



- (void) loadSound:(NSString *)file doesLoop:(bool)loops withVolume:(float)volume withPitch:(float)pitch
{
	CMOpenALSound *sound = [soundDictionary objectForKey:file];
	
	if(sound == nil)
	{
		//create a new sound
		sound = [[CMOpenALSound alloc] initWithSoundFile:file doesLoop:loops]; //this will return nil on failure
		
		if(!sound) //error
			return;
		
		[soundDictionary setObject:sound forKey:file];
		[sound release];
	}
	
	sound.volume = volume;
	sound.pitch = pitch;
}


- (void) playSound:(NSString *)file
{
	CMOpenALSound *sound = [soundDictionary objectForKey:file];
	
	if (sound == nil)
	{
		NSLog(@"Could not find sound (%@)\n", file);
		return;
	}
	[sound play];
}


- (void) playSoundWithVolumePitch:(NSString *)file withVolume:(float)volume withPitch:(float)pitch
{
	CMOpenALSound *sound = [soundDictionary objectForKey:file];
	
	if (sound == nil)
	{
		NSLog(@"Could not find sound (%@)\n", file);
		return;
	}
	[sound play];
	sound.volume = volume;
	sound.pitch = pitch;
}


- (void) stopSound:(NSString *)file
{
	CMOpenALSound *sound = [soundDictionary objectForKey:file];
	if (sound == nil)
	{
		NSLog(@"Could not find sound (%@)\n", file);
		return;
	}
	
	[sound stop];
}


- (BOOL) isPlayingSound:(NSString *)file
{
	CMOpenALSound *sound = [soundDictionary objectForKey:file];
	if (sound == nil) return NO;
	return [sound isPlaying];
}


/*
- (void) playSoundWithID:(NSUInteger)soundID
{	
	//get sound key
	NSString *soundFile = [self keyForSoundID:soundID];
	if(!soundFile) return;
	
	CMOpenALSound *sound = [soundDictionary objectForKey:soundFile];
	
	if(!sound)
	{
		//create a new sound
		sound = [[CMOpenALSound alloc] initWithSoundFile:soundFile doesLoop:NO]; //this will return nil on failure
		
		if(!sound) //error
			return;
		
		[soundDictionary setObject:sound forKey:soundFile];
		[sound release];
	}
	
	[sound play];
	sound.volume = self.soundEffectsVolume;
}

- (void) stopSoundWithID:(NSUInteger)soundID
{
	NSString *soundFile = [self keyForSoundID:soundID];
	if(!soundFile) return;
	
	CMOpenALSound *sound = [soundDictionary objectForKey:soundFile];		
	[sound stop];
}

- (void) pauseSoundWithID:(NSUInteger)soundID
{
	NSString *soundFile = [self keyForSoundID:soundID];
	if(!soundFile) return;
	
	CMOpenALSound *sound = [soundDictionary objectForKey:soundFile];		
	[sound stop];
}

- (void) rewindSoundWithID:(NSUInteger)soundID
{
	NSString *soundFile = [self keyForSoundID:soundID];
	if(!soundFile) return;
	
	CMOpenALSound *sound = [soundDictionary objectForKey:soundFile];
	[sound rewind];
}

- (BOOL) isPlayingSoundWithID:(NSUInteger)soundID
{
	NSString *soundFile = [self keyForSoundID:soundID];
	if(!soundFile) return NO;
	
	CMOpenALSound *sound = [soundDictionary objectForKey:soundFile];		
	return [sound isAnyPlaying];
}









- (void) playSoundWithID:(NSUInteger)soundID
{	
	//get sound key
	NSString *soundFile = [self keyForSoundID:soundID];
	if(!soundFile) return;
	
	CMOpenALSound *sound = [soundDictionary objectForKey:soundFile];
	
	if(!sound)
	{
		//create a new sound
		sound = [[CMOpenALSound alloc] initWithSoundFile:soundFile doesLoop:NO]; //this will return nil on failure
		
		if(!sound) //error
			return;
		
		[soundDictionary setObject:sound forKey:soundFile];
		[sound release];
	}
	
	[sound play];
	sound.volume = self.soundEffectsVolume;
}

- (void) stopSoundWithID:(NSUInteger)soundID
{
	NSString *soundFile = [self keyForSoundID:soundID];
	if(!soundFile) return;
	
	CMOpenALSound *sound = [soundDictionary objectForKey:soundFile];		
	[sound stop];
}

- (void) pauseSoundWithID:(NSUInteger)soundID
{
	NSString *soundFile = [self keyForSoundID:soundID];
	if(!soundFile) return;
	
	CMOpenALSound *sound = [soundDictionary objectForKey:soundFile];		
	[sound stop];
}

- (void) rewindSoundWithID:(NSUInteger)soundID
{
	NSString *soundFile = [self keyForSoundID:soundID];
	if(!soundFile) return;
	
	CMOpenALSound *sound = [soundDictionary objectForKey:soundFile];
	[sound rewind];
}

- (BOOL) isPlayingSoundWithID:(NSUInteger)soundID
{
	NSString *soundFile = [self keyForSoundID:soundID];
	if(!soundFile) return NO;
	
	CMOpenALSound *sound = [soundDictionary objectForKey:soundFile];		
	return [sound isAnyPlaying];
}
*/
- (BOOL) isBackGroundMusicPlaying
{
	return [backgroundAudio isPlaying];
}

#pragma mark -
#pragma mark properties
- (float) backgroundMusicVolume
{	
	return backgroundMusicVolume;
}

- (void) setBackgroundMusicVolume:(float) newVolume
{	
	backgroundMusicVolume = newVolume;
	backgroundAudio.volume = newVolume;
}

- (float) soundEffectsVolume
{
	return soundEffectsVolume;
}

- (void) setSoundEffectsVolume:(float) newVolume
{
	soundEffectsVolume = newVolume;
	for(NSString *key in soundDictionary)
	{
		((CMOpenALSound *)[soundDictionary objectForKey:key]).volume = newVolume;
	}
}
@end
