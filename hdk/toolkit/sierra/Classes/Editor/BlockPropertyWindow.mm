//
//  BlockPropertyWindow.mm
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "BlockPropertyWindow.h"
#import "LevelEditor.h"

@implementation BlockPropertyWindow

- (id) init
{
	if ((self = [super initWithWindowNibName:@"BlockPropertyWindow"]))
	{
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(itemWasSelected)
		 name:kSingleItemSelectedNotification
		 object:nil];
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(itemWasUnselected)
		 name:kSingleItemUnselectedNotification
		 object:nil];
		
		// initially hidden
		[self.window orderOut:self];
	}
	return self;
}



- (void)itemWasSelected
{
	const hdGameObject *selected;
	selected = [LevelEditor sharedInstance]->GetSelectedGameObjectSingle();
	if (selected->GetUserType() != e_totemTypeBlock)
	{
		[self.window orderOut:self];
		return;
	}
	[self.window orderBack:self];
	[_texture setImageAtContentRepositoryPath:((totemBlock*)selected)->GetTextureName()];
	[_textureName setStringValue:[NSString stringWithUTF8String:((totemBlock*)selected)->GetTextureName()]];
}


- (void)itemWasUnselected
{
	[self.window orderOut:self];
}


@end
