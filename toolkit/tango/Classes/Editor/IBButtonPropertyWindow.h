//
//  IBButtonPropertyWindow.h
//  Tango
//
//  Created by David Petrie on 15/06/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "IBPropertyWindow.h"
#import "hdImageView.h"

@interface IBButtonPropertyWindow : IBPropertyWindow 
{
@private
	IBOutlet NSColorWell *_tint;
	IBOutlet hdImageView *_normalTexture;
	IBOutlet hdImageView *_overTexture;
	IBOutlet hdImageView *_upTexture;
}

- (IBAction)UpdateNormalButtonTexture:(id)sender;

- (IBAction)UpdateOverButtonTexture:(id)sender;

- (IBAction)UpdateUpButtonTexture:(id)sender;

@end
