//
//  LevelPropertyWindow.mm
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "WorldPropertyWindow.h"
#import "LevelEditor.h"
#import "NSImageView+Extensions.h"


@implementation WorldPropertyWindow

- (id) init
{
	if ((self = [super initWithWindowNibName:@"WorldPropertyWindow"]))
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
		 selector:@selector(levelWasLoaded)
		 name:kLoadedLevelWasChangedNotification
		 object:nil];
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(worldWasLoaded)
		 name:kWorldWasLoadedNotification
		 object:nil];
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(levelDidChange:)
		 name:NSTableViewSelectionDidChangeNotification
		 object:nil];
	}
	return self;
}


- (void)worldWasLoaded
{	
	const totemWorld *world;
	if ((world = [LevelEditor sharedInstance]->GetCurrentTotemWorld()))
	{
		[_worldName setStringValue:[NSString stringWithUTF8String:world->GetName().c_str()]];
		[self levelWasLoaded];
		[_levelsTableView reloadData]; 
	}
	
}


- (void)worldWasUnloaded
{
}


- (void)levelDidChange:(NSNotification *)notification
{
	id obj = [notification object];
	if (obj == _levelsTableView)
	{
		NSInteger row = [_levelsTableView selectedRow];
		if ([LevelEditor sharedInstance]->SetLevel(row))
		{
			[[NSNotificationCenter defaultCenter] 
			 postNotificationName:kLoadedLevelWasChangedNotification
			 object:nil];
		}
	}
}


- (void)levelWasLoaded
{
	const totemLevel *updatedLevel;
	
	if (!(updatedLevel = [LevelEditor sharedInstance]->GetCurrentLevel()))
	{
		return;
	}
	
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
	/*
	totemFloorInfo *floorInfo = (totemFloorInfo *)(level->GetFloorInfo());
	
	if (floorInfo == NULL)
	{
		floorInfo = new totemFloorInfo();
		level->SetFloorInfo(floorInfo);
	}
	
	if (currentLevelFloorTexture1->get_int_val() != -1)
	{
		snprintf(floorInfo->m_firstTextureName, kMaxTexturePathSize, "%s", currentLevelFloorTexture1->curr_text.c_str());
	}
	
	if (currentLevelFloorTexture2->get_int_val() != -1)
	{
		snprintf(floorInfo->m_secondTextureName, kMaxTexturePathSize, "%s", currentLevelFloorTexture2->curr_text.c_str());
	}
	floorInfo->m_floorLevel =  (float) strtof(currentLevelFloorLevel->get_text(), NULL);
	floorInfo->m_isWater = (currentLevelFloorIsWater->get_int_val() != 0);
	floorInfo->m_isReflective = (currentLevelReflectiveFloor->get_int_val() != 0);*/
}


#pragma mark -
#pragma mark NSTableViewDelegate functions

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
	const totemWorld *world = [LevelEditor sharedInstance]->GetCurrentTotemWorld();
	return (world) ? world->GetLevelCount() : 0;
	
}


- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	const totemWorld *world;
	const totemLevel *level;
	if ((world = [LevelEditor sharedInstance]->GetCurrentTotemWorld()))
	{
		if (rowIndex >= 0 && rowIndex < world->GetLevelCount())
		{
			if ((level = world->GetLevels()[rowIndex]))
			{
				return [NSString stringWithUTF8String: level->GetLevelName()];
			}
			else {
				return nil;
			}

		}
	}
	return nil;
}

@end
