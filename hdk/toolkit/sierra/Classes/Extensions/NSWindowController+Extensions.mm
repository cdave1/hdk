//
//  NSWindowController+Extensions.mm
//  Sierra
//
//  Created by David Petrie on 8/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "NSWindowController+Extensions.h"


@implementation NSWindowController (Extensions)

- (void)setAllEnabled:(BOOL)enabled
{
	for(NSView *view in [[self.window contentView] subviews])
	{
		if ([view respondsToSelector:@selector(setEnabled:)])
		{
			[((NSControl *)view) setEnabled:enabled];
		}
	}
}

@end
