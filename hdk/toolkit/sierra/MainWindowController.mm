//
//  MainWindowController.m
//  Sierra
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "MainWindowController.h"
#import "LevelEditor.h"

@implementation MainWindowController

- (id) init
{
	if ((self = [super initWithWindowNibName:@"MainWindow"]))
	{
		_editorViewController = [[EditorViewController alloc] init];
		[self.window setContentView:_editorViewController.editorView];
		[_editorViewController handleResize];
		
		_levelPropertyWindow = [[LevelPropertyWindow alloc] init];
		[_levelPropertyWindow showWindow:self];
		
		_texturePaletteWindow = [[TexturePaletteWindow alloc] init];
		[_texturePaletteWindow showWindow:self];
		
		_blockPropertyWindow = [[BlockPropertyWindow alloc] init];
		//[_blockPropertyWindow showWindow:self];
	}
	return self;
}


- (void)windowDidResize:(NSNotification *)notification
{
	[_editorViewController handleResize];
}


- (void) keyDown:(NSEvent *)theEvent
{
	unichar unicodeKey;
	
	unicodeKey = [ [ theEvent characters ] characterAtIndex:0 ];
	unsigned short keyCode = [theEvent keyCode];
	
	hdPrintf("Key: %c %d %d %d\n", unicodeKey, unicodeKey, keyCode, [theEvent modifierFlags]);
	
	switch( unicodeKey )
	{
		case NSDeleteCharacter:
		case NSDeleteFunctionKey:
			[LevelEditor sharedInstance]->DeleteKey();
			break;
		case ' ':
			[LevelEditor sharedInstance]->TogglePerspectiveProjection();
			break;
		case 'c':
			if (([theEvent modifierFlags] & NSControlKeyMask) == NSControlKeyMask)
			{
				[LevelEditor sharedInstance]->CopySelectedObjects();
			}
			break;
			
		case 'v':
			if (([theEvent modifierFlags] & NSControlKeyMask) == NSControlKeyMask)
			{
				[LevelEditor sharedInstance]->PasteCopiedObjects();
			}
			break;
		case 't':
			if (([theEvent modifierFlags] & NSControlKeyMask) == NSControlKeyMask)
			{
				//[LevelEditor sharedInstance]->ApplyCurrentTextureToSelected();
			}
			break;
		case 'p':
			if (([theEvent modifierFlags] & NSControlKeyMask) == NSControlKeyMask)
			{
				[LevelEditor sharedInstance]->RepairSelectedShape();
			}
			break;
		case 'd':
			if (([theEvent modifierFlags] & NSControlKeyMask) == NSControlKeyMask)
			{
				[LevelEditor sharedInstance]->DumpUniqueTextureList();
			}
			break;
		case 'b':
			if (([theEvent modifierFlags] & (NSControlKeyMask | NSShiftKeyMask)) == 
				 (NSControlKeyMask | NSShiftKeyMask))
			{
				//[LevelEditor sharedInstance]->ApplyBGValuesToAllFollowingLevels();
			}
			break;
			
		case NSLeftArrowFunctionKey:
			[LevelEditor sharedInstance]->Pan(-20.0f, 0);
			break;
			
			// Press right to pan right.
		case NSRightArrowFunctionKey:
			[LevelEditor sharedInstance]->Pan(20.0f, 0);
			break;
			
			// Press down to pan down.
		case NSDownArrowFunctionKey:
			[LevelEditor sharedInstance]->Pan(0.0f, -20.0f);
			break;
			
			// Press up to pan up.
		case NSUpArrowFunctionKey:
			[LevelEditor sharedInstance]->Pan(0.0f, 20.0f);
			break;
		default:
			break;
	}
	[super keyDown:theEvent];
}


@end
