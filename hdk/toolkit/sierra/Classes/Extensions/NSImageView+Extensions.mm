//
//  NSImageView+Extensions.m
//  Sierra
//
//  Created by David Petrie on 2/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "NSImageView+Extensions.h"
#import "LevelEditor.h"

@implementation NSImageView (Extensions)

- (void)setImageAtContentRepositoryPath:(const char *)path
{
	if (path && strlen(path) > 0)
	{
		NSString *fullPath = [NSString stringWithFormat:@"%s%s", FileSystem_BaseDir(), path];
		NSImage *img = [[NSImage alloc] initWithContentsOfFile:fullPath];
		[self setImage:[img autorelease]];
	}
}

@end
