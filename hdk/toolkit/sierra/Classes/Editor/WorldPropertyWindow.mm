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
		
		NSPoint o = NSMakePoint(0, 1600);
		
		[self.window setFrameTopLeftPoint:o];
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


- (IBAction)UpdateLevelFloorInfo:(id)sender
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


- (IBAction)levelAddRemoveSegment:(id)sender
{
	if ([_addRemoveSegment selectedSegment] == 0)
	{
		[self addNewLevel:sender];
	}
	else if ([_addRemoveSegment selectedSegment] == 1)
	{
		[self deleteSelectedLevel:sender];
	}
}


- (IBAction)addNewLevel:(id)sender
{
	[LevelEditor sharedInstance]->GenerateNewLevel();
	[_levelsTableView reloadData];
	[_levelsTableView selectRowIndexes:[NSIndexSet indexSetWithIndex:[LevelEditor sharedInstance]->GetCurrentTotemWorld()->GetLevelCount()-1]
				  byExtendingSelection:NO];
	[[NSNotificationCenter defaultCenter] 
	 postNotificationName:kLoadedLevelWasChangedNotification
	 object:nil];
}


/**
 * Needs an alert.
 */
- (IBAction)deleteSelectedLevel:(id)sender
{
	/*
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
	 */
}


- (IBAction)UpdateLevelAABB:(id)sender
{
	const totemLevel *level;
	if (!(level = (totemLevel *)[LevelEditor sharedInstance]->GetCurrentLevel()))
	{
		return;
	}
	
	((totemLevel *)level)->SetExtendAABB([_extendLevelAABBCheckbox state] == NSOnState);
}


- (IBAction)UpdateLevelBackgrounds:(id)sender
{
	const totemLevel *level;
	const char *textureName;
	if (!(level = [LevelEditor sharedInstance]->GetCurrentLevel()))
	{
		return;
	}
	
	textureName = [_skyTexture getResourcePathOfImage];
	if (textureName && strlen(textureName) > 0)
	{
		((totemLevel *)level)->SetSkyTextureName(textureName);
	}
	else
	{
		((totemLevel *)level)->SetSkyTextureName("");
	}
	
	textureName = [_horizonTexture getResourcePathOfImage];
	if (textureName && strlen(textureName) > 0)
	{
		((totemLevel *)level)->SetDistantBackgroundTextureName(textureName);
	}
	else
	{
		((totemLevel *)level)->SetDistantBackgroundTextureName("");
	}
	
	textureName = [_middleTexture getResourcePathOfImage];
	if (textureName && strlen(textureName) > 0)
	{
		((totemLevel *)level)->SetFarBackgroundTextureName(textureName);
	}
	else
	{
		((totemLevel *)level)->SetFarBackgroundTextureName("");
	}
	
	textureName = [_nearTexture getResourcePathOfImage];
	if (textureName && strlen(textureName) > 0)
	{
		((totemLevel *)level)->SetNearBackgroundTextureName(textureName);
	}
	else
	{
		((totemLevel *)level)->SetNearBackgroundTextureName("");
	}
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
