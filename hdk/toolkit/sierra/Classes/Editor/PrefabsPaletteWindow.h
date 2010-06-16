//
//  PrefabsPaletteWindow.h
//  Sierra
//
//  Created by David Petrie on 13/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "hdk.h"
#import "hdLevelEditorController.h"
#import "LevelEditor.h"

@interface PrefabsPaletteWindow : NSWindowController 
	<NSTableViewDataSource,
		NSTableViewDelegate>
{
@private
	IBOutlet NSTableView *_prefabsTableView;
	IBOutlet NSButton *_copyPrefab;
	
	IBOutlet NSPanel *_prefabNamePanel;
	IBOutlet NSTextField *_prefabName;
	
	hdLevelEditorController *_prefabEditor;
}

- (void)makeNewPrefabFromSelected;
- (IBAction)copySelectedToNewPrefab:(id)sender;
- (IBAction)copyPrefabToLevelEditor:(id)sender;
- (void)selectedPrefabDidChange:(NSNotification *)notification;

@end
