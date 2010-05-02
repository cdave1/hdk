//
//  MainWindowController.h
//  Sierra
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "EditorViewController.h"
#import "LevelPropertyWindow.h"
#import "BlockPropertyWindow.h"
#import "PolygonPropertyWindow.h"
#import "JointPropertyWindow.h"
#import "EventPropertyWindow.h"
#import "TexturePaletteWindow.h"

@interface MainWindowController : NSWindowController
{
@private
	EditorViewController *_editorViewController;
	IBOutlet NSView *_contentView;
	
	LevelPropertyWindow *_levelPropertyWindow;
	
	TexturePaletteWindow *_texturePaletteWindow;
	
	BlockPropertyWindow *_blockPropertyWindow;
}

@end
