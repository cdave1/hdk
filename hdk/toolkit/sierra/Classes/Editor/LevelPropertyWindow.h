//
//  LevelPropertyWindow.h
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface LevelPropertyWindow : NSWindowController 
{
	IBOutlet NSTextField *_levelName;
	IBOutlet NSButton *_extendLevelAABBCheckbox;
	
	IBOutlet NSBox *_skyInfoBox;
	IBOutlet NSImageView *_skyTexture;
	IBOutlet NSImageView *_horizonTexture;
	IBOutlet NSImageView *_middleTexture;
	IBOutlet NSImageView *_nearTexture;
	
	IBOutlet NSButton *_hasFloor;
	IBOutlet NSBox *_hasFloorBox;
	IBOutlet NSImageView *_floorTexture1;
	IBOutlet NSImageView *_floorTexture2;
	IBOutlet NSSlider *_floorLevel;
	IBOutlet NSTextField *_floorLevelText;
	IBOutlet NSButton *_isWater;
	IBOutlet NSButton *_isReflective;
}


- (void)levelWasChanged;


- (IBAction)levelInfoWasChanged:(id)sender;

@end
