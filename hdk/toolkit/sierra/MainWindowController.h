//
//  MainWindowController.h
//  Sierra
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "EditorViewController.h"
#import "WorldPropertyWindow.h"
#import "BlockPropertyWindow.h"
#import "PolygonPropertyWindow.h"
#import "RevoluteJointPropertyWindow.h"
#import "PrismaticJointPropertyWindow.h"
#import "EventPropertyWindow.h"
#import "TexturePaletteWindow.h"

@interface MainWindowController : NSWindowController <NSComboBoxDelegate, NSComboBoxDataSource>
{
@private
	EditorViewController *_editorViewController;
	IBOutlet NSView *_contentView;
	IBOutlet NSPanel *_mainPanel;
	IBOutlet NSPanel *_toolPanel;
	IBOutlet NSButton *_cursorButton;
	IBOutlet NSButton *_blockButton;
	IBOutlet NSButton *_jointButton;
	IBOutlet NSButton *_cosmeticsFilterCheckbox;
	IBOutlet NSButton *_groundFilterCheckbox;
	IBOutlet NSButton *_physicsFilterCheckbox;
	
	IBOutlet NSComboBox *_blockMaterialComboBox;
	IBOutlet NSComboBox *_blockTypeComboBox;
	IBOutlet NSComboBox *_jointTypeComboBox;
	IBOutlet NSComboBox *_blockShapeComboBox;
	IBOutlet NSColorWell *_currentTint;
	IBOutlet hdImageView *_currentTexture;
	
	IBOutlet NSButton *_togglePhysics;
	IBOutlet NSProgressIndicator *_progress;
	
	WorldPropertyWindow *_worldPropertyWindow;
	
	TexturePaletteWindow *_texturePaletteWindow;
	
	BlockPropertyWindow *_blockPropertyWindow;
	
	RevoluteJointPropertyWindow *_revoluteJointPropertyWindow;
	
	PrismaticJointPropertyWindow *_prismaticJointPropertyWindow;
}

- (IBAction)togglePhysics:(id)sender;

- (IBAction)setCursorMode:(id)sender;

- (IBAction)setBlockMode:(id)sender;

- (IBAction)setFilter:(id)sender;

- (IBAction)commitBlockStates:(id)sender;

- (IBAction)updatePaletteTexture:(id)sender;

- (IBAction)updatePaletteTint:(id)sender;



- (IBAction)SetNewJointMode:(id)sender;

- (IBAction)SetNewEventMode:(id)sender;

- (IBAction)UpdateNewBlockType:(id)sender;

- (IBAction)UpdateNewBlockShapeType:(id)sender;

- (IBAction) UpdateNewJointType:(id)sender;

- (IBAction)UpdateNewBlockMaterial:(id)sender;
@end
