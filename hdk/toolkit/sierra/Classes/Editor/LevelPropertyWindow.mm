//
//  LevelPropertyWindow.mm
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "LevelPropertyWindow.h"
#import "LevelEditor.h"
#import "NSImageView+Extensions.h"
@implementation LevelPropertyWindow

- (id) init
{
	if ((self = [super initWithWindowNibName:@"LevelPropertyWindow"]))
	{
		[_levelName setEnabled:NO];
		
		/*
		_extendLevelAABBCheckbox;
		_skyCheckbox;
		
		_skyInfoBox;
		_skyTexture;
		_horizonTexture;
		_middleTexture;
		_nearTexture;
		
		_hasFloor;
		_hasFloorBox;
		_floorTexture1;
		_floorTexture2;
		_floorLevel;
		_isWater;
		_isReflective;
		 */
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(levelWasChanged)
		 name:kLevelWasLoadedNotification
		 object:nil];
	}
	return self;
}


- (void)levelWasChanged
{
	const totemLevel *updatedLevel = [LevelEditor sharedInstance]->GetCurrentLevel();
	[_levelName setStringValue:[NSString stringWithUTF8String:updatedLevel->GetLevelName()]];
	
	if (updatedLevel->GetFloorInfo() == NULL)
	{
		[_hasFloor setState:NSOffState];
		[_hasFloorBox setHidden:YES];
	}
	else
	{
		[_hasFloor setState:NSOnState];
		[_hasFloorBox setHidden:NO];
		
		const totemFloorInfo *floorInfo = updatedLevel->GetFloorInfo();
		const char *floorTexture1Path = ((totemFloorInfo *)floorInfo)->GetFirstTexture()->name;
		const char *floorTexture2Path = ((totemFloorInfo *)floorInfo)->GetSecondTexture()->name;
		
		[_floorLevel setFloatValue:((totemFloorInfo *)floorInfo)->m_floorLevel];
		[_floorLevelText setFloatValue:((totemFloorInfo *)floorInfo)->m_floorLevel];
		
		[_isWater setState:(((totemFloorInfo *)floorInfo)->m_isWater ? NSOnState : NSOffState)];
		[_isReflective setState:(((totemFloorInfo *)floorInfo)->m_isReflective ? NSOnState : NSOffState)];
		
		[_floorTexture1 setImageAtContentRepositoryPath:floorTexture1Path];
		[_floorTexture2 setImageAtContentRepositoryPath:floorTexture2Path];
	}
	
	[_skyTexture setImageAtContentRepositoryPath:updatedLevel->GetSkyTextureName()];
	[_horizonTexture setImageAtContentRepositoryPath:updatedLevel->GetDistantBackgroundTextureName()];
	[_middleTexture setImageAtContentRepositoryPath:updatedLevel->GetFarBackgroundTextureName()];
	[_nearTexture setImageAtContentRepositoryPath:updatedLevel->GetNearBackgroundTextureName()];
}


- (IBAction)levelInfoWasChanged:(id)sender
{
	
}

@end
