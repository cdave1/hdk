//
//  hdAVAudioWrapper.mm
//  TotemGame
//
//  Created by david on 26/07/09.
//  Copyright 2009 n/a. All rights reserved.
//

#import "hdAVAudioWrapper.h"

@interface hdAVAudioWrapper ()
- (void)isIPodPlaying;
- (BOOL)isPlayingAvAudio;
- (void)cleanUpSound;
@end



@implementation hdAVAudioWrapper

/*
static hdAVAudioWrapper* audioWrapper = nil;


void MusicPlayer_Init()
{
	if (audioWrapper == nil)
	{
		audioWrapper = [[hdAVAudioWrapper alloc] init];
	}
}


int MusicPlayer_LoadSound(const char* fileName, const float volume)
{
	assert(audioWrapper != nil);
	return [audioWrapper LoadSoundWithFileName:[NSString stringWithCString:fileName encoding:NSASCIIStringEncoding] withVolume:volume];
}


int MusicPlayer_PlaySound(const char* fileName)
{
	assert(audioWrapper != nil);
	return [audioWrapper PlaySoundWithFileName:[NSString stringWithCString:fileName encoding:NSASCIIStringEncoding]];
}


int MusicPlayer_StopSound(const char* fileName)
{
	assert(audioWrapper != nil);
	return [audioWrapper StopSoundWithFileName:[NSString stringWithCString:fileName encoding:NSASCIIStringEncoding]];
}


int MusicPlayer_RewindSound(const char* fileName)
{
	assert(audioWrapper != nil);
	return [audioWrapper RewindSoundWithFileName:[NSString stringWithCString:fileName encoding:NSASCIIStringEncoding] playAfterRewind:false];
}


int MusicPlayer_UnloadSound(const char* fileName)
{
	assert(audioWrapper != nil);
	return [audioWrapper UnloadSoundWithFileName:[NSString stringWithCString:fileName encoding:NSASCIIStringEncoding]];
}


void MusicPlayer_Teardown()
{
	assert(audioWrapper != nil);
	[audioWrapper release];
	audioWrapper = nil;
}
*/


@synthesize players;

- (id)init
{
	if ((self = [super init]) != nil) 
	{
		players = [[NSMutableDictionary alloc] initWithCapacity:15];
	}
	return self;
}


- (void)dealloc
{
	[players removeAllObjects];
	[players release];
	players = nil;
	[super dealloc];
}



- (BOOL)isPlaying
{
	BOOL playing = false;
	NSArray *keys = [players keys];
	for (NSString *filename in keys)
	{
		AVAudioPlayer *player = [players valueForKey:filename];
		if (player.playing)
		{
			playing = true;
			break;
		}
	}
	return playing;
}



- (NSInteger)LoadSoundWithFileName:(NSString *)fileName withVolume:(float)volume
{
	assert(players != nil);
	NSURL *url = [[NSURL alloc] initFileURLWithPath:fileName];
	NSError *errorCode = nil;
	AVAudioPlayer *player = [[AVAudioPlayer alloc] initWithContentsOfURL:url error:&errorCode];
	[url release];
	
	/*
	 * TODO: Better error code return values.
	 */
	if (errorCode != nil)
	{ 
#if DEBUG
		printf("AVAudioWrapper error while loading file %s:\n\tCode: %d\n\tDescription: %s\n\tReason: %s\n", 
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding],
			   (int)[errorCode code], 
			   [[errorCode localizedDescription] cStringUsingEncoding:NSASCIIStringEncoding], 
			   [[errorCode localizedFailureReason] cStringUsingEncoding:NSASCIIStringEncoding]);
#endif
		return -1;
	}
	
	if (player == nil)
	{
#if DEBUG
		printf("AVAudioWrapper error while loading file %s:\n\tReason: Could not create AVAudioPlayer\n", 
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
#endif
		return -1;
	}

	player.volume = volume;
	player.delegate = self;
	[players setValue:player forKey:fileName];
	
#if DEBUG	
	NSLog(@"There are now %d players in the dictionary.", [players count]);
#endif
	
	return 0;
}


- (NSInteger)PlaySoundWithFileName:(NSString *)fileName  loops:(BOOL)loops
{
	assert(players != nil);
	AVAudioPlayer *player = [players valueForKey:fileName];
	
	if (player == nil)
		return -1;
	
	if (loops == YES)
		[player setNumberOfLoops:-1];
	
	if (![player prepareToPlay])
		return -1;
	
	if ([player play]) 
		return 0;
	else 
		return -1;
}


- (NSInteger)RewindSoundWithFileName:(NSString *)fileName playAfterRewind:(BOOL)play
{
	assert(players != nil);
	AVAudioPlayer *player = [players valueForKey:fileName];
	
	if (player == nil)
		return -1;
	
	if (player.playing)
		[player stop];
	
	player.currentTime = 0;
	
	if (play)
		[player play];
	
	return 0;
}


- (NSInteger)StopSoundWithFileName:(NSString *)fileName
{
	assert(players != nil);
	AVAudioPlayer *player = [players valueForKey:fileName];
	
	if (player == nil)
		return -1;
	
	if (player.playing)
		[player stop];

	return 0;
}


- (NSInteger)PauseSoundWithFileName:(NSString *)fileName
{
	assert(players != nil);
	AVAudioPlayer *player = [players valueForKey:fileName];
	
	if (player == nil)
		return -1;
	
	if (player.playing)
		[player pause];
	
	return 0;
}


- (NSInteger)UnloadSoundWithFileName:(NSString *)fileName
{
	assert(players != nil);
	AVAudioPlayer *player = [players valueForKey:fileName];
	
	if (player == nil)
		return -1;
	
	if (player.playing)
		[player stop];
	
	[players removeObjectForKey:fileName];
	//[player release];
	return 0;
}


- (void)audioPlayerBeginInterruption:(AVAudioPlayer *)player
{
	if (player.playing)
		[player stop];
}


- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError *)error
{
	printf("AVAudioWrapper decode error for player with url %s - offending player will be removed. Error:\n",
		   [[player.url absoluteString] cStringUsingEncoding:NSASCIIStringEncoding]);

	printf("\tCode: %d\n\tDescription: %s\n\tReason: %s\n", 
		   (int)[error code], 
		   [[error localizedDescription] cStringUsingEncoding:NSASCIIStringEncoding], 
		   [[error localizedFailureReason] cStringUsingEncoding:NSASCIIStringEncoding]);
	
	// Remove the player
	[self UnloadSoundWithFileName:[player.url absoluteString]];
}


- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag
{
	NSInteger numloops = [player numberOfLoops];
	
	if (numloops <= -1)
	{
		player.currentTime = 0;
		
		if (player.playing == NO)
			[player play];
		
		return;
	}
	
	assert(players != nil);
	
	// Remove the player...
	AVAudioPlayer *p = [players valueForKey:[player.url absoluteString]];
	
	if (p == nil) return;
	
	if (p.playing)
		[p stop];
	
	[players removeObjectForKey:[p.url absoluteString]];
	//[p release];
}


- (void)audioPlayerEndInterruption:(AVAudioPlayer *)player
{
	if (!player.playing)
		[player play];
}


@end
