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

#import <Cocoa/Cocoa.h>
#import "EditorViewController.h"
#import "WorldPropertyWindow.h"
#import "BlockPropertyWindow.h"
#import "PolygonPropertyWindow.h"
#import "RevoluteJointPropertyWindow.h"
#import "PrismaticJointPropertyWindow.h"
#import "EventPropertyWindow.h"
#import "TexturePaletteWindow.h"
#import "PrefabsPaletteWindow.h"

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
	IBOutlet NSButton *_meshButton;
	
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
	
	PrefabsPaletteWindow *_prefabsPaletteWindow;
}

/**
 * Notification handlers
 */
- (void)levelWillBeDeleted;

- (void)deleteAlertEnded:(NSAlert *)alert returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo;

- (IBAction)togglePhysics:(id)sender;

- (IBAction)setFilter:(id)sender;

- (IBAction)commitBlockStates:(id)sender;

- (IBAction)setCursorMode:(id)sender;

- (IBAction)SetNewShapeRectangle:(id)sender;
- (IBAction)SetNewShapeCircle:(id)sender;
- (IBAction)SetNewShapeFivePointStar:(id)sender;
- (IBAction)SetNewShapeHexagon:(id)sender;
- (IBAction)SetNewShapeTriangle:(id)sender;
- (IBAction)SetNewShapeMultiStar:(id)sender;
- (IBAction)SetNewShapeMesh:(id)sender;

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
