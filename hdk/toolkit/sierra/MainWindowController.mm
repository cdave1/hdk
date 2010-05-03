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
		
		_worldPropertyWindow = [[WorldPropertyWindow alloc] init];
		[_worldPropertyWindow showWindow:self];
		
		_texturePaletteWindow = [[TexturePaletteWindow alloc] init];
		[_texturePaletteWindow showWindow:self];
		
		_blockPropertyWindow = [[BlockPropertyWindow alloc] init];
		_revoluteJointPropertyWindow = [[RevoluteJointPropertyWindow alloc] init];
		_prismaticJointPropertyWindow = [[PrismaticJointPropertyWindow alloc] init];
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
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeCursor;
}


- (IBAction)setBlockMode:(id)sender
{
	[LevelEditor sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddTotemBlock;
}


- (IBAction)setFilter:(id)sender
{
	[LevelEditor sharedInstance]->settings.showCosmetics = [_cosmeticsFilterCheckbox state] == NSOnState;
	[LevelEditor sharedInstance]->settings.showGround = [_groundFilterCheckbox state] == NSOnState;
	[LevelEditor sharedInstance]->settings.showPhysics = [_physicsFilterCheckbox state] == NSOnState;
}

/*

void SetInterfaceMode_AddJoint(int)
{
	levelEditorController->settings.interfacePaletteMode = e_interfacePaletteModeTotemJoint;
}



void SetInterfaceMode_AddJack(int)
{
	levelEditorController->settings.interfacePaletteMode = e_interfacePaletteModeTotemJack;
}


void SetInterfaceMode_AddEvent(int)
{
	levelEditorController->settings.interfacePaletteMode = e_interfacePaletteModeTotemEvent;
}


void UpdateNewJackType(int)
{
	levelEditorController->settings.newTotemJackType = (e_totemJackType)newJackTypesList->get_int_val();
}


void UpdateNewBlockType(int newType)
{
	levelEditorController->settings.newTotemBlockType = (e_totemBlockType)newBlockTypesList->get_int_val();
}


void UpdateNewBlockShapeType(int)
{
	levelEditorController->settings.newTotemShapeType = (e_totemShapeType)newBlockShapesList->get_int_val();	
}


void UpdateNewEventType(int)
{
	levelEditorController->settings.newTotemEventType = (e_totemEventType)newEventTypesList->get_int_val();	
}


void UpdateNewJointType(int)
{
	levelEditorController->settings.newTotemJointType = (e_totemJointType)newJointTypesList->get_int_val();
}


void UpdateNewBlockMaterial(int newType)
{
	levelEditorController->settings.newTotemMaterial = (e_totemMaterial)newMaterialList->get_int_val();
}*/


#pragma mark -
#pragma mark NSComboBoxDataSource functions
/*
- (id)comboBox:(NSComboBox *)aComboBox objectValueForItemAtIndex:(NSInteger)index
{
	if (aComboBox == _materialComboBox)
	{
		return [NSString stringWithUTF8String:totemMaterialMenuItems[index].materialName];
	}
	else if (aComboBox == _blockTypeComboBox)
	{
		return [NSString stringWithUTF8String:totemBlockTypeMenuItems[index].blockTypeName];
	}
	return nil;
}


- (NSInteger)numberOfItemsInComboBox:(NSComboBox *)aComboBox
{
	if (aComboBox == _materialComboBox)
	{
		return e_totemMaterial_Count;
	}
	else if (aComboBox == _blockTypeComboBox)
	{
		return e_totemBlockType_Count;
	}
	return 0;
}*/


@end
