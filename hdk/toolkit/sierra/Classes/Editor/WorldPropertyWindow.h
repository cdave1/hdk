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

- (void)deleteAlertEnded:(NSAlert *)alert returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo;

- (IBAction)UpdateHasFloor:(id)sender;

- (IBAction)UpdateLevelFloorInfo:(id)sender;

- (IBAction)UpdateLevelAABB:(id)sender;

- (IBAction)UpdateLevelBackgrounds:(id)sender;

- (IBAction)UpdateLevelName:(id)sender;

- (IBAction)UpdateWorldName:(id)sender;

@end
