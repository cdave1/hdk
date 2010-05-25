//
//  MainWindowController.m
//  Sierra
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

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
		//[self.window makeMainWindow];
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
		//[_blockPropertyWindow showWindow:self];
		

		
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
		
		//[_toolPanel setFloatingPanel:YES];
		
		[_blockMaterialComboBox selectItemAtIndex:0];
		[_blockTypeComboBox selectItemAtIndex:0];
		[_jointTypeComboBox selectItemAtIndex:0];
		[_blockShapeComboBox selectItemAtIndex:0];
	}
	return self;
}


- (void)windowDidResize:(NSNotification *)notification
{
	[_editorViewController handleResize];
}


- (void)mouseMoved:(NSEvent *)mouseOver
{
	//hdPrintf("Mouse moved!\n");
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
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSControlKeyMask)
			{
				[LevelEditor sharedInstance]->ApplyCurrentTextureToSelected();
			}
			break;
		case 'j':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSControlKeyMask)
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


/*
 - (IBAction)UpdateNewEventType(int)
 {
 levelEditorController->settings.newTotemEventType = (e_totemEventType)newEventTypesList->get_int_val();	
 }*/


- (IBAction)SetNewEventMode:(id)sender
{
	//[LevelEditor sharedInstance]->
}




- (IBAction)SetNewBlockMaterial:(id)sender
{
	[LevelEditor sharedInstance]->SetNewBlockMaterialType(totemMaterialMenuItems[[_blockMaterialComboBox indexOfSelectedItem]].material);
}


- (IBAction)SetNewBlockType:(id)sender
{
	[LevelEditor sharedInstance]->SetNewBlockType(totemBlockTypeMenuItems[[_blockTypeComboBox indexOfSelectedItem]].blockType);
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
