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
