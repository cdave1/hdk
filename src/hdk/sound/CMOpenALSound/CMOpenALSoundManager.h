//
//  CMOpenALSoundManager.h
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

#define USE_AS_SINGLETON 0	//Set this to 0 if you want to use this class as a regular class instead of as a singleton.

@class CMOpenALSound;

@interface CMOpenALSoundManager : NSObject 
{	
	CMOpenALSound		*backgroundAudio;	// background music
	
	NSMutableDictionary	*soundDictionary;	// stores our soundkeys
	
	BOOL				isiPodAudioPlaying;	// ipod playing music? 

	float				backgroundMusicVolume;
	float				soundEffectsVolume;
}

@property (nonatomic, readonly) BOOL isiPodAudioPlaying;
@property (nonatomic) float backgroundMusicVolume;
@property (nonatomic) float soundEffectsVolume;

#if USE_AS_SINGLETON
+ (CMOpenALSoundManager *) sharedCMOpenALSoundManager;
#endif

- (void) purgeSounds;		// purges all sounds from memory, in case of a memory warning...
- (void) shutdownOpenAL;

- (void) playBackgroundMusic:(NSString *)file withVolume:(float)volume;
- (void) playBackgroundMusic:(NSString *)file forcePlay:(BOOL)forcePlay withVolume:(float)volume; //if forcePlay is YES, iPod audio will be stopped.
- (void) stopBackgroundMusic;
- (void) pauseBackgroundMusic;
- (void) resumeBackgroundMusic;

- (void) loadSound:(NSString *)file doesLoop:(bool)loops withVolume:(float)volume withPitch:(float)pitch;
- (void) playSound:(NSString *)file;
- (void) playSoundWithVolumePitch:(NSString *)file withVolume:(float)volume withPitch:(float)pitch;
- (void) stopSound:(NSString *)file;
//- (void) pauseSound:(NSString *)file;
//- (void) rewindSound:(NSString *)file;
- (BOOL) isPlayingSound:(NSString *)file;

/*
- (void) playSoundWithID:(NSUInteger)soundID;
- (void) stopSoundWithID:(NSUInteger)soundID;
- (void) pauseSoundWithID:(NSUInteger)soundID;
- (void) rewindSoundWithID:(NSUInteger)soundID;
- (BOOL) isPlayingSoundWithID:(NSUInteger)soundID;
*/
- (BOOL) isBackGroundMusicPlaying;

- (void) checkIfiPodIsPlaying;	//checks for external audio in use (iPod)

@end
