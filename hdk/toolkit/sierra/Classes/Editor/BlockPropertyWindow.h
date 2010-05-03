//
//  BlockPropertyWindow.h
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface BlockPropertyWindow : NSWindowController <NSComboBoxDelegate, NSComboBoxDataSource>
{
	IBOutlet NSComboBox *_materialComboBox;
	IBOutlet NSComboBox *_blockTypeComboBox;
	IBOutlet NSTextField *_blockTag;
	
	IBOutlet NSSlider *_depth;
	IBOutlet NSSlider *_zOffset;
	IBOutlet NSSlider *_tiling;
	IBOutlet NSTextField *_depthText;
	IBOutlet NSTextField *_zOffsetText;
	IBOutlet NSTextField *_tilingText;
	
	IBOutlet NSColorWell *_tint;
	IBOutlet NSImageView *_texture;
	IBOutlet NSTextField *_textureName;
}
- (void)itemWasSelected;
- (void)itemWasUnselected;
- (void)updateTextInterface;

- (IBAction)UpdateSlidersFromText:(id)sender;

- (IBAction)UpdateSelectedBlockMaterial:(id)sender;
- (IBAction)UpdateSelectedBlockType:(id)sender;

- (IBAction)UpdateSelectedBlockZOffset:(id)sender;
- (IBAction)UpdateSelectedBlockTexture:(id)sender;
- (IBAction)UpdateSelectedBlockTiling:(id)sender;
- (IBAction)UpdateSelectedBlockDepth:(id)sender;
- (IBAction)UpdateSelectedBlockTag:(id)sender;


@end
