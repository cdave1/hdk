//
//  BlockPropertyWindow.h
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface BlockPropertyWindow : NSWindowController 
{
	IBOutlet NSComboBox *_materialComboBox;
	IBOutlet NSComboBox *_blockTypeComboBox;
	IBOutlet NSTextField *_blockTag;
	IBOutlet NSSlider *_depth;
	IBOutlet NSSlider *_zOffset;
	IBOutlet NSSlider *_tiling;
	IBOutlet NSColorWell *_tint;
	IBOutlet NSImageView *_texture;
	IBOutlet NSTextField *_textureName;
}
- (void)itemWasSelected;
- (void)itemWasUnselected;
@end
