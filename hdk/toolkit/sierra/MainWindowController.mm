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

#import "MainWindowController.h"
#import "LevelEditor.h"

@interface MainWindowController ()
- (void)SetButtonStates:(id)sender;
@end


@implementation MainWindowController

- (id) init
{
	if ((self = [super initWithWindowNibName:@"MainWindow"]))
	{
		_editorViewController = [[EditorViewController alloc] init];
		[self.window setContentView:_editorViewController.editorView];
		[self.window setAcceptsMouseMovedEvents:YES];
		[_editorViewController handleResize];
		
		_worldPropertyWindow = [[WorldPropertyWindow alloc] init];
		[_worldPropertyWindow showWindow:self];
		
		_texturePaletteWindow = [[TexturePaletteWindow alloc] init];
		[_texturePaletteWindow showWindow:self];
		
		_blockPropertyWindow = [[BlockPropertyWindow alloc] init];
		_revoluteJointPropertyWindow = [[RevoluteJointPropertyWindow alloc] init];
		_prismaticJointPropertyWindow = [[PrismaticJointPropertyWindow alloc] init];
		_prefabsPaletteWindow = [[PrefabsPaletteWindow alloc] init];
		[_prefabsPaletteWindow.window setFrameOrigin:NSMakePoint(0, 0)];
		
		[_texturePaletteWindow.window
		 setFrameOrigin:NSMakePoint([NSScreen mainScreen].frame.size.width - _texturePaletteWindow.window.frame.size.width, 1600)];
		
		[_toolPanel setFrameOrigin:NSMakePoint(_worldPropertyWindow.window.frame.origin.x + 
											   _worldPropertyWindow.window.frame.size.width, 
											   1600)];
		
		[self.window setFrameOrigin:NSMakePoint(_worldPropertyWindow.window.frame.size.width, 5)];
		NSRect mainWindowFrame = self.window.frame;
		mainWindowFrame.size = NSMakeSize([NSScreen mainScreen].frame.size.width - 
										  (_texturePaletteWindow.window.frame.size.width +
										   _worldPropertyWindow.window.frame.size.width),
										  [NSScreen mainScreen].frame.size.height - _toolPanel.frame.size.height - 30);
		[self.window setFrame:mainWindowFrame display:YES];
		[self.window addChildWindow:_toolPanel ordered:NSWindowAbove];
		
		[_blockMaterialComboBox selectItemAtIndex:0];
		[_blockTypeComboBox selectItemAtIndex:0];
		[_jointTypeComboBox selectItemAtIndex:0];
		[_blockShapeComboBox selectItemAtIndex:0];

		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(levelWillBeDeleted)
		 name:kLevelWasDeletedNotification
		 object:nil];
	}
	return self;
}


- (void)windowDidResize:(NSNotification *)notification
{
	[_editorViewController handleResize];
}


- (void)mouseMoved:(NSEvent *)mouseOver
{
	if (![self.window isKeyWindow])
		[self.window makeKeyWindow];
}


- (void) keyDown:(NSEvent *)theEvent
{
	unichar unicodeKey;
	
	NSString *chars = [theEvent charactersIgnoringModifiers];
	unicodeKey = [chars characterAtIndex:0];
	unsigned short keyCode = [theEvent keyCode];
	
	switch( unicodeKey )
	{
		case NSDeleteCharacter:
		case NSDeleteFunctionKey:
			[LevelEditor sharedInstance]->DeleteKey();
			break;
		case ' ':
			[LevelEditor sharedInstance]->TogglePerspectiveProjection();
			break;
		case 'p':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
			{
				[_prefabsPaletteWindow makeNewPrefabFromSelected];
			}
			break;
		case 'a':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
			{
				[LevelEditor sharedInstance]->SelectAll();
			}
			break;
		case 'c':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
			{
				[LevelEditor sharedInstance]->CopySelectedObjects();
			}
			break;
			
		case 'v':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
			{
				[LevelEditor sharedInstance]->PasteCopiedObjects();
			}
			break;
		case 't':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
			{
				[LevelEditor sharedInstance]->ApplyCurrentTextureToSelected();
			}
			break;
		case 'j':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
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
		case 'g':
			if (([theEvent modifierFlags] & (NSControlKeyMask) == (NSControlKeyMask)))
			{
				[LevelEditor sharedInstance]->ApplyBGValuesToAllFollowingLevels();
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


- (IBAction)togglePhysics:(id)sender
{
	if ([LevelEditor sharedInstance]->isPhysicsOn())
	{
		[LevelEditor sharedInstance]->PhysicsOff();
		[_togglePhysics setTitle:@"Start Physics"];
		[_progress stopAnimation:self];
	}
	else
	{
		[LevelEditor sharedInstance]->PhysicsOn();
		[_togglePhysics setTitle:@"Stop Physics"];
		[_progress startAnimation:self];
	}
}


- (IBAction)setCursorMode:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeCursor;
}


- (IBAction)setFilter:(id)sender
{
	[LevelEditor sharedInstance]->settings.showCosmetics = [_cosmeticsFilterCheckbox state] == NSOnState;
	[LevelEditor sharedInstance]->settings.showGround = [_groundFilterCheckbox state] == NSOnState;
	[LevelEditor sharedInstance]->settings.showPhysics = [_physicsFilterCheckbox state] == NSOnState;
}


- (IBAction)commitBlockStates:(id)sender
{
	const totemLevel *level;
	if (!(level = [LevelEditor sharedInstance]->GetCurrentLevel()))
	{
		return;
	}
	// Current shapes: make the current positions of the 
	// blocks into the start positions.
	((totemLevel *)level)->CommitBlockStates();
}


- (IBAction)updatePaletteTexture:(id)sender
{
	const char *textureName = [_currentTexture getResourcePathOfImage];
	if (textureName)
	{
		[LevelEditor sharedInstance]->SetPaletteTexture(textureName);
	}
}


- (IBAction)updatePaletteTint:(id)sender
{
}


- (IBAction)SetNewEventMode:(id)sender
{
}


- (IBAction)SetNewBlockMaterial:(id)sender
{
	[LevelEditor sharedInstance]->SetNewBlockMaterialType(totemMaterialMenuItems[[_blockMaterialComboBox indexOfSelectedItem]].material);
}


- (IBAction)SetNewBlockType:(id)sender
{
	[LevelEditor sharedInstance]->SetNewBlockType(totemBlockTypeMenuItems[[_blockTypeComboBox indexOfSelectedItem]].blockType);
}


// TEMPORARY
- (IBAction)SetNewShapeMesh:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeStraightlineMesh;
}


/**
 * toolbar buttons
 */
- (IBAction)SetNewShapeRectangle:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_totemShapeTypeRectangle);
}


- (IBAction)SetNewShapeCircle:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_totemShapeTypeCylinder);
}


- (IBAction)SetNewShapeFivePointStar:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_totemShapeTypeStar);
}


- (IBAction)SetNewShapeHexagon:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_totemShapeTypeHexagon);
}


- (IBAction)SetNewShapeTriangle:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_totemShapeTypeTriangle);
}


- (IBAction)SetNewShapeMultiStar:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_totemShapeTypeMultiStar);
}


- (IBAction)SetNewShapeTriangleFan:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_totemShapeTypeCustom);
}


- (IBAction)SetNewShapeTriangleStrip:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_totemShapeTypeTriStrip);
}


- (IBAction)SetNewShapeSpikes:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
	[LevelEditor sharedInstance]->SetNewBlockShapeType(e_totemShapeTypeSpikes);
}


- (IBAction)SetNewJointDistance:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeTotemJoint;
	[LevelEditor sharedInstance]->SetNewJointType(e_totemJointTypeDistance);
}


- (IBAction)SetNewJointPrismatic:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeTotemJoint;
	[LevelEditor sharedInstance]->SetNewJointType(e_totemJointTypePrismatic);
}


- (IBAction)SetNewJointRevolute:(id)sender
{
	[self SetButtonStates:sender];
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeTotemJoint;
	[LevelEditor sharedInstance]->SetNewJointType(e_totemJointTypeRevolute);
}


- (void)SetButtonStates:(id)sender
{
	for(NSView *view in [[_toolPanel contentView] subviews])
	{
		if (view != sender &&
			[view class] == [NSButton class] && 
			[view tag] == kToolbarToggleButtonTag)
		{
			[((NSButton *)view) setState:NSOffState];
		}
	}
	[((NSButton *)sender) setState:NSOnState];
}


- (void)levelWillBeDeleted
{
	NSAlert *alert = [[[NSAlert alloc] init] autorelease];
	
	[alert addButtonWithTitle:@"OK"];
	[alert addButtonWithTitle:@"Cancel"];
	[alert setMessageText:@"Delete this level?"];
	[alert setInformativeText:@"Deleted levels cannot be restored."];
	[alert setAlertStyle:NSWarningAlertStyle];
	[alert beginSheetModalForWindow:[self window] modalDelegate:self didEndSelector:@selector(deleteAlertEnded:returnCode:contextInfo:) contextInfo:nil];
}


- (void)deleteAlertEnded:(NSAlert *)alert returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo 
{
    if (returnCode == NSAlertFirstButtonReturn)
	{
		// Do a backup save
		[LevelEditor sharedInstance]->SaveCurrentWorld();
		
		// Remove the level
		int index = -1;
		for (int i = 0; i < [LevelEditor sharedInstance]->GetCurrentTotemWorld()->GetLevelCount(); i++)
		{
			if ([LevelEditor sharedInstance]->GetCurrentTotemWorld()->GetLevels()[i] == [LevelEditor sharedInstance]->GetCurrentLevel())
			{
				index = i;
				break;
			}
		}
		
		if (index == -1) return;
		
		if (index == ([LevelEditor sharedInstance]->GetCurrentTotemWorld()->GetLevelCount() - 1))
		{
			[LevelEditor sharedInstance]->SetLevel(hdMax(0, index - 1));
		}
		else
		{
			[LevelEditor sharedInstance]->SetLevel(index + 1);
		}

		[LevelEditor sharedInstance]->GetCurrentTotemWorld()->RemoveLevelAtIndex(index);

		[[NSNotificationCenter defaultCenter] 
		 postNotificationName:kWorldWasLoadedNotification
		 object:nil];
    }
}


#pragma mark -
#pragma mark NSComboBoxDelegate functions

- (void)comboBoxSelectionDidChange:(NSNotification *)notification
{
	NSLog(@"%@", [notification name]);
}


- (void)comboBoxWillDismiss:(NSNotification *)notification
{
	NSLog(@"%@", [notification name]);
}


#pragma mark -
#pragma mark NSComboBoxDataSource functions

- (id)comboBox:(NSComboBox *)aComboBox objectValueForItemAtIndex:(NSInteger)index
{
	if (aComboBox == _blockMaterialComboBox)
	{
		return [NSString stringWithUTF8String:totemMaterialMenuItems[index].materialName];
	}
	if (aComboBox == _blockTypeComboBox)
	{
		return [NSString stringWithUTF8String:totemBlockTypeMenuItems[index].blockTypeName];
	}
	if (aComboBox == _jointTypeComboBox)
	{
		return [NSString stringWithUTF8String:totemJointTypeMenuItems[index].jointTypeName];
	}
	if (aComboBox == _blockShapeComboBox)
	{
		return [NSString stringWithUTF8String:totemShapeTypeMenuItems[index].shapeTypeName];
	}
	return nil;
}


- (NSInteger)numberOfItemsInComboBox:(NSComboBox *)aComboBox
{
	if (aComboBox == _blockMaterialComboBox)
	{
		return e_totemMaterial_Count;
	}
	if (aComboBox == _blockTypeComboBox)
	{
		return e_totemBlockType_Count;
	}
	if (aComboBox == _jointTypeComboBox)
	{
		return e_totemJoinType_Count;
	}
	if (aComboBox == _blockShapeComboBox)
	{
		return e_totemShapeType_Count;
	}
	return 0;
}

@end
