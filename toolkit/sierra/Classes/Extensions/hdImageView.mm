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

#import "hdImageView.h"
#include "hdk.h"
@implementation hdImageView

@synthesize currentImagePath = _currentImagePath;

// via http://cocoadevcentral.com/articles/000056.php and
// http://webcache.googleusercontent.com/search?q=cache:E2-VxkryiOsJ:www.cocoadev.com/index.pl%3FNSImageView+NSImageView+drag+drop&cd=1&hl=en&ct=clnk&gl=nz&client=firefox-a

- (NSDragOperation)draggingUpdated:(id <NSDraggingInfo>)sender
{
    NSPasteboard* thePasteboard = [sender draggingPasteboard];
    NSArray* theTypes = [thePasteboard types];
    if([theTypes containsObject:NSFilenamesPboardType])
    {
        NSArray* thePaths = [thePasteboard propertyListForType:NSFilenamesPboardType];
        _currentImagePath = [[thePaths objectAtIndex:0] copy];
    }
    else
    {
        _currentImagePath = nil;
    }
    return [super draggingUpdated:sender];
}


- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender
{
    NSPasteboard* thePasteboard = [sender draggingPasteboard];
    NSArray* theTypes = [thePasteboard types];
    if([theTypes containsObject:NSFilenamesPboardType])
    {
        NSArray* thePaths = [thePasteboard propertyListForType:NSFilenamesPboardType];
        _currentImagePath = [[thePaths objectAtIndex:0] copy];
    }
    else
    {
        _currentImagePath = nil;
    }

    return YES;
}


- (void)setImageAtContentRepositoryPath:(const char *)path
{
    if (path && strlen(path) > 0)
    {
        NSString *fullPath = [NSString stringWithFormat:@"%s%s", FileSystem_BaseDir(), path];
        _currentImagePath = [fullPath copy];
        NSImage *img = [[NSImage alloc] initWithContentsOfFile:fullPath];
        [self setImage:[img autorelease]];
    }
}


- (const char *)getResourcePathOfImage
{
    if (![self image]) return NULL;
    if (_currentImagePath)
    {
        NSLog(@"%@", _currentImagePath);
        return [[_currentImagePath stringByReplacingOccurrencesOfString:[NSString stringWithUTF8String:FileSystem_BaseDir()]
                                                             withString:@""] UTF8String];
    }
    return NULL;
}

@end
