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
#import "hdImageView.h"

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
    IBOutlet hdImageView *_texture;
    IBOutlet NSTextField *_textureName;
}

- (void)itemWasSelected;

- (void)itemWasUnselected;

- (void)multipleItemsSelected;

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
