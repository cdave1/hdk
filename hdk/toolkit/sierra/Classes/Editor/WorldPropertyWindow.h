//
//  LevelPropertyWindow.h
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface WorldPropertyWindow : NSWindowController 
	<NSTableViewDataSource,
	NSTableViewDelegate>
{
	IBOutlet NSTextField *_worldName;
	
	IBOutlet NSTableView *_levelsTableView;
	
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


- (void)worldWasLoaded;

- (void)worldWasUnloaded;


- (void)levelDidChange:(NSNotification *)notification;

- (void)levelWasLoaded;


- (IBAction)deleteSelectedLevel:(id)sender;
/*
{
	if (levelEditorController->settings.deleteButtonTaps >= 3)
	{
		// Do a bakup save
		levelEditorController->Save();
		
		// Remove the level
		int index = -1;
		for (int i = 0; i < levelEditorController->GetCurrentTotemWorld()->GetLevelCount(); i++)
		{
			if (levelEditorController->GetCurrentTotemWorld()->GetLevels()[i] == levelEditorController->GetCurrentLevel())
			{
				index = i;
				break;
			}
		}
		
		if (index == -1) return;
		
		if (index == 0)
		{
			lev = levelEditorController->GetCurrentTotemWorld()->GetLevels()[1];
		}
		else
		{
			lev = levelEditorController->GetCurrentTotemWorld()->GetLevels()[0];
		}
		
		
		levelEditorController->SetCurrentLevel(lev);
		txtLevelName->set_text(lev->GetLevelName());
		
		levelEditorController->GetCurrentTotemWorld()->RemoveLevelAtIndex(index);
		
		UpdateInterfaceState();
		
		RefreshLevelList();
		
		levelEditorController->settings.deleteButtonTaps = 0;
	}
}*/

- (IBAction)levelInfoWasChanged:(id)sender;

@end
