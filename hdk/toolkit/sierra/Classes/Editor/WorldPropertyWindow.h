//
//  LevelPropertyWindow.h
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "hdImageView.h"
#import "NSView+Extensions.h"

@interface WorldPropertyWindow : NSWindowController 
	<NSTableViewDataSource,
	NSTableViewDelegate>
{
	IBOutlet NSTextField *_worldName;
	
	IBOutlet NSTableView *_levelsTableView;
	
	IBOutlet NSTextField *_levelName;
	IBOutlet NSButton *_extendLevelAABBCheckbox;
	
	IBOutlet NSBox *_skyInfoBox;
	IBOutlet hdImageView *_skyTexture;
	IBOutlet hdImageView *_horizonTexture;
	IBOutlet hdImageView *_middleTexture;
	IBOutlet hdImageView *_nearTexture;
	
	IBOutlet NSButton *_hasFloor;
	IBOutlet NSBox *_hasFloorBox;
	IBOutlet hdImageView *_floorTexture1;
	IBOutlet hdImageView *_floorTexture2;
	IBOutlet NSSlider *_floorLevel;
	IBOutlet NSTextField *_floorLevelText;
	IBOutlet NSButton *_isWater;
	IBOutlet NSButton *_isReflective;
	
	IBOutlet NSSegmentedControl *_addRemoveSegment;
}


- (void)worldWasLoaded;

- (void)worldWasUnloaded;


- (void)levelDidChange:(NSNotification *)notification;

- (void)levelWasLoaded;

- (IBAction)levelAddRemoveSegment:(id)sender;

- (IBAction)addNewLevel:(id)sender;

- (IBAction)deleteSelectedLevel:(id)sender;

- (IBAction)UpdateHasFloor:(id)sender;

- (IBAction)UpdateLevelFloorInfo:(id)sender;

- (IBAction)UpdateLevelAABB:(id)sender;

- (IBAction)UpdateLevelBackgrounds:(id)sender;

- (IBAction)UpdateLevelName:(id)sender;

@end
