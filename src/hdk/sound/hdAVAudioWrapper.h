//
//  hdAVAudioWrapper.h
//  TotemGame
//
//  Created by david on 26/07/09.
//  Copyright 2009 n/a. All rights reserved.
//

#import "hdMusicPlayerInterface.h"
//#import <UIKit/UIKit.h>

#import <Foundation/Foundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import <AVFoundation/AVFoundation.h>

//@class  hdAVAudioWrapper;

@interface hdAVAudioWrapper : NSObject <AVAudioPlayerDelegate>
{
    //@property (nonatomic, assign)	AVAudioPlayer	*player;
@private
    NSMutableDictionary *players;

}
- (id)init;

- (NSInteger)LoadSoundWithFileName:(NSString *)fileName withVolume:(float)volume;
- (NSInteger)PlaySoundWithFileName:(NSString *)fileName loops:(BOOL)loops;
- (NSInteger)RewindSoundWithFileName:(NSString *)fileName playAfterRewind:(BOOL)play;
- (NSInteger)StopSoundWithFileName:(NSString *)fileName;
- (NSInteger)PauseSoundWithFileName:(NSString *)fileName;
- (NSInteger)UnloadSoundWithFileName:(NSString *)fileName;

- (void)audioPlayerBeginInterruption:(AVAudioPlayer *)player;
- (void)audioPlayerDecodeErrorDidOccur:(AVAudioPlayer *)player error:(NSError *)error;
- (void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag;
- (void)audioPlayerEndInterruption:(AVAudioPlayer *)player;

@property (nonatomic, assign) NSMutableDictionary *players;
@end