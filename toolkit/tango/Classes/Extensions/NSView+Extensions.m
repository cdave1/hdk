//
//  NSView+Extensions.m
//  Sierra
//
//  Created by David Petrie on 9/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "NSView+Extensions.h"


@implementation NSView (Extensions)

- (void)setAllControlsEnabled:(BOOL)enabled
{
	for(NSView *view in [self subviews])
	{
		if ([view respondsToSelector:@selector(setEnabled:)])
		{
			[((NSControl *)view) setEnabled:enabled];
		}
	}
}

@end
