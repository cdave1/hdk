//
//  PrefabsPaletteWindow.m
//  Sierra
//
//  Created by David Petrie on 13/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "PrefabsPaletteWindow.h"


@implementation PrefabsPaletteWindow

- (id) init
{
	if ((self = [super initWithWindowNibName:@"PrefabsPaletteWindow"]))
	{
		// Load the prefabs world.
		
		// HACK HACK HACK
		//
		// prefab world just maintains its own level editor controller.
		_prefabEditor = new hdLevelEditorController();
		
		// If it does not exist, make a new one and save the file to disk.
		if (!_prefabEditor->LoadWorld("./prefabs.ttw"))
		{
			_prefabEditor->GenerateNewEmptyWorldWithName("prefabs.ttw");
			_prefabEditor->SaveCurrentWorldTo("./prefabs.ttw");
			_prefabEditor->LoadWorld("./prefabs.ttw");
		}
	}
	return self;
}


- (void)dealloc
{
	delete _prefabEditor;
	[super dealloc];
}


- (void)makeNewPrefabFromSelected
{
	// Get selected game objects from level editor
	// copy them to a new level.
	// set level name to prefabname
	// save level to world.
	// save prefab world to disk.
	if ([LevelEditor sharedInstance]->m_selectedGameObjects->GetItemCount() > 0)
	{
		[[NSApplication sharedApplication] runModalForWindow:_prefabNamePanel];
	}
}


- (IBAction)copySelectedToNewPrefab:(id)sender
{
	[[NSApplication sharedApplication] stopModal];
	[_prefabNamePanel close];
	hdAssert([LevelEditor sharedInstance]->m_selectedGameObjects->GetItemCount() > 0);
	if ([[_prefabName stringValue] length] > 0)
	{
		_prefabEditor->GenerateNewEmptyLevelWithName([[_prefabName stringValue] UTF8String]);
		_prefabEditor->PasteObjectsExternal((const hdGameObject **)[LevelEditor sharedInstance]->m_selectedGameObjects->GetItems(),
											[LevelEditor sharedInstance]->m_selectedGameObjects->GetItemCount());
		[LevelEditor sharedInstance]->SelectNone();
		
		[_prefabsTableView reloadData];
		[_prefabsTableView selectRowIndexes:[NSIndexSet indexSetWithIndex:[LevelEditor sharedInstance]->GetCurrentTotemWorld()->GetLevelCount()-1]
					   byExtendingSelection:NO];
	}
	if (_prefabEditor->GetCurrentTotemWorld()->GetLevelCount() > 0)
		_prefabEditor->SaveCurrentWorldTo("./prefabs.ttw");
	
}


- (IBAction)copyPrefabToLevelEditor:(id)sender
{
	// Get selected row index
	// Set level to selected.
	// Get game objects from selected.
	// Set selected objects in level editor
	// paste them in.
	
	NSInteger row = [_prefabsTableView selectedRow];
	if (!_prefabEditor->SetLevel(row))
	{
		hdPrintf("Couldn't select prefab at row %d\n", row);
		return;
	}
	
	_prefabEditor->SelectAll();
	
	[LevelEditor sharedInstance]->PasteObjectsExternal((const hdGameObject **)_prefabEditor->m_selectedGameObjects->GetItems(),
										_prefabEditor->m_selectedGameObjects->GetItemCount());
	_prefabEditor->SelectNone();
}


- (void)selectedPrefabDidChange:(NSNotification *)notification
{
	id obj = [notification object];
	if (obj == _prefabsTableView)
	{
		NSInteger row = [_prefabsTableView selectedRow];
		if (!_prefabEditor->SetLevel(row))
		{
			hdPrintf("Couldn't select prefab at row %d\n", row);
		}
	}
}


#pragma mark -
#pragma mark NSTableViewDelegate functions

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
	NSAssert(_prefabEditor->GetCurrentTotemWorld(), @"Prefab world must always be present.");
	return _prefabEditor->GetCurrentTotemWorld()->GetLevelCount();
	
}


- (id)tableView:(NSTableView *)aTableView objectValueForTableColumn:(NSTableColumn *)aTableColumn row:(NSInteger)rowIndex
{
	const totemWorld *world;
	const totemLevel *level;
	
	NSAssert(_prefabEditor->GetCurrentTotemWorld(), @"Prefab world must always be present.");
	
	if (rowIndex >= 0 && rowIndex < _prefabEditor->GetCurrentTotemWorld()->GetLevelCount())
	{
		if ((level = _prefabEditor->GetCurrentTotemWorld()->GetLevels()[rowIndex]))
		{
			return [NSString stringWithUTF8String: level->GetLevelName()];
		}
		else 
		{
			return nil;
		}
	}
	return nil;
}

@end
