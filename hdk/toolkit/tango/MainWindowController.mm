//
//  MainWindowController.m
//  Sierra
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "MainWindowController.h"
#import "InterfaceBuilder.h"

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
		
		_texturePaletteWindow = [[TexturePaletteWindow alloc] init];
		[_texturePaletteWindow showWindow:self];
		
		_blockPropertyWindow = [[IBImagePropertyWindow alloc] init];
		_buttonPropertyWindow = [[IBButtonPropertyWindow alloc] init];
		//_prefabsPaletteWindow = [[PrefabsPaletteWindow alloc] init];
		//[_prefabsPaletteWindow.window setFrameOrigin:NSMakePoint(0, 0)];
		//[_blockPropertyWindow showWindow:self];
		

		
		[_texturePaletteWindow.window
		 setFrameOrigin:NSMakePoint([NSScreen mainScreen].frame.size.width - _texturePaletteWindow.window.frame.size.width, 1600)];
		
		NSWindowController *_worldPropertyWindow = [[NSWindowController alloc] init];
		_worldPropertyWindow.window = [[NSWindow alloc] init];
		
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
		
		
		/*[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(levelWillBeDeleted)
		 name:kLevelWasDeletedNotification
		 object:nil];*/
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(paletteTextureChanged)
		 name:kSelectedTextureImageChanged
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
			[InterfaceBuilder sharedInstance]->DeleteKey();
			break;
		case ' ':
			//[InterfaceBuilder sharedInstance]->TogglePerspectiveProjection();
			// Set to 1:1 zoom level.
			break;
		case 'a':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
			{
				[InterfaceBuilder sharedInstance]->SelectAll();
			}
			break;
		case 'c':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
			{
				[InterfaceBuilder sharedInstance]->CopySelectedObjects();
			}
			break;
			
		case 'v':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSCommandKeyMask)
			{
				[InterfaceBuilder sharedInstance]->PasteCopiedObjects();
			}
			break;
		case 't':
			if (([theEvent modifierFlags] & NSCommandKeyMask) == NSControlKeyMask)
			{
				[InterfaceBuilder sharedInstance]->ApplyCurrentTextureToSelected();
			}
			break;
		case 'd':
			if (([theEvent modifierFlags] & NSControlKeyMask) == NSControlKeyMask)
			{
				[InterfaceBuilder sharedInstance]->DumpUniqueTextureList();
			}
			break;
		case 'b':
			if (([theEvent modifierFlags] & (NSControlKeyMask | NSShiftKeyMask)) == 
				 (NSControlKeyMask | NSShiftKeyMask))
			{
				//[InterfaceBuilder sharedInstance]->ApplyBGValuesToAllFollowingLevels();
			}
			break;
			
		case NSLeftArrowFunctionKey:
			[InterfaceBuilder sharedInstance]->Pan(-20.0f, 0);
			break;
			
			// Press right to pan right.
		case NSRightArrowFunctionKey:
			[InterfaceBuilder sharedInstance]->Pan(20.0f, 0);
			break;
			
			// Press down to pan down.
		case NSDownArrowFunctionKey:
			[InterfaceBuilder sharedInstance]->Pan(0.0f, -20.0f);
			break;
			
			// Press up to pan up.
		case NSUpArrowFunctionKey:
			[InterfaceBuilder sharedInstance]->Pan(0.0f, 20.0f);
			break;
		default:
			break;
	}
	[super keyDown:theEvent];
}


- (IBAction)setCursorMode:(id)sender
{
	[self SetButtonStates:sender];
	[InterfaceBuilder sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeCursor;
}


- (IBAction)updatePaletteTexture:(id)sender
{
	const char *textureName = [_currentTexture getResourcePathOfImage];
	if (textureName)
	{
		[InterfaceBuilder sharedInstance]->SetPaletteTexture(textureName);
	}
}


- (void)paletteTextureChanged
{
	[_currentTexture setImageAtContentRepositoryPath:[InterfaceBuilder sharedInstance]->GetPaletteTexture()];
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
	//[InterfaceBuilder sharedInstance]->
}


/**
 * toolbar buttons
 e_interfacePaletteModeAddContainer,
 e_interfacePaletteModeAddButton,
 e_interfacePaletteModeAddImage,
 e_interfacePaletteModeAddEvent,
 */
- (IBAction)SetNewContainer:(id)sender
{
	[self SetButtonStates:sender];
	[InterfaceBuilder sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddContainer;
}


- (IBAction)SetNewButton:(id)sender
{
	[self SetButtonStates:sender];
	[InterfaceBuilder sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddButton;
}


- (IBAction)SetNewImage:(id)sender
{
	[self SetButtonStates:sender];
	[InterfaceBuilder sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddImage;
}


- (IBAction)SetNewEvent:(id)sender
{
	[self SetButtonStates:sender];
	[InterfaceBuilder sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddEvent;
}


- (IBAction)SetNewFontPolygon:(id)sender
{
	[self SetButtonStates:sender];
	[InterfaceBuilder sharedInstance]->settings.interfacePaletteMode = e_interfacePaletteModeAddFontPolygon;
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
	return nil;
}


- (NSInteger)numberOfItemsInComboBox:(NSComboBox *)aComboBox
{
	return 0;
}


@end
