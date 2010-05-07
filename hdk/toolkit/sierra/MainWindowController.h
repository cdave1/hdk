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





- (IBAction)setFilter:(id)sender;

- (IBAction)commitBlockStates:(id)sender;



/**
 * toolbar buttons
 */


- (IBAction)setCursorMode:(id)sender;

- (IBAction)SetNewShapeRectangle:(id)sender;
- (IBAction)SetNewShapeCircle:(id)sender;
- (IBAction)SetNewShapeFivePointStar:(id)sender;
- (IBAction)SetNewShapeHexagon:(id)sender;
- (IBAction)SetNewShapeTriangle:(id)sender;
- (IBAction)SetNewShapeMultiStar:(id)sender;

- (IBAction)SetNewShapeTriangleFan:(id)sender;
- (IBAction)SetNewShapeTriangleStrip:(id)sender;
- (IBAction)SetNewShapeSpikes:(id)sender;

- (IBAction)SetNewJointDistance:(id)sender;
- (IBAction)SetNewJointPrismatic:(id)sender;
- (IBAction)SetNewJointRevolute:(id)sender;

/**
 * Events
 */
- (IBAction)SetNewEventMode:(id)sender;

/**
 * Toolbar tint and texture
 */
- (IBAction)updatePaletteTexture:(id)sender;
- (IBAction)updatePaletteTint:(id)sender;

/**
 * toolbar drop boxes
 */
- (IBAction)SetNewBlockMaterial:(id)sender;
- (IBAction)SetNewBlockType:(id)sender;

@end
