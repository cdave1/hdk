//
//  IBImagePropertyWindow.h
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "IBPropertyWindow.h"
#import "hdImageView.h"

@interface IBImagePropertyWindow : IBPropertyWindow <NSComboBoxDelegate, NSComboBoxDataSource>
{
	IBOutlet NSSlider *_tiling;
	IBOutlet NSTextField *_tilingText;
	
	IBOutlet NSColorWell *_tint;
	IBOutlet hdImageView *_texture;
	IBOutlet NSTextField *_textureName;
}
- (void)itemWasSelected;
- (void)itemWasUnselected;
- (void)multipleItemsSelected;



- (IBAction)UpdateSlidersFromText:(id)sender;

- (IBAction)UpdateImageTexture:(id)sender;
- (IBAction)UpdateImageTiling:(id)sender;


@end
