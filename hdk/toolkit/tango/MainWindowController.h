//
//  MainWindowController.h
//  Sierra
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "EditorViewController.h"
#import "IBImagePropertyWindow.h"
#import "IBButtonPropertyWindow.h"
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
	
	
	
	TexturePaletteWindow *_texturePaletteWindow;
	
	IBImagePropertyWindow *_blockPropertyWindow;
	
	IBButtonPropertyWindow *_buttonPropertyWindow;
}

//- (IBAction)showPrefabs:(id)sender;



- (IBAction)setFilter:(id)sender;

- (IBAction)commitBlockStates:(id)sender;




/**
 * toolbar buttons
 */


- (IBAction)setCursorMode:(id)sender;

- (IBAction)SetNewButton:(id)sender;
- (IBAction)SetNewContainer:(id)sender;
- (IBAction)SetNewImage:(id)sender;
- (IBAction)SetNewFontPolygon:(id)sender;

/**
 * Events
 */
- (IBAction)SetNewEventMode:(id)sender;

/**
 * Toolbar tint and texture
 */
- (IBAction)updatePaletteTexture:(id)sender;
- (IBAction)updatePaletteTint:(id)sender;


@end
