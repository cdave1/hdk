//
//  TextureImageBrowserView.mm
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "TextureImageBrowserView.h"
#import "InterfaceBuilder.h"

@implementation TextureImageBrowserView

- (void)drawRect:(NSRect)rect
{
	LOCK_GL_CONTEXT;
	[super drawRect:rect];
	UNLOCK_GL_CONTEXT;
}




@end
