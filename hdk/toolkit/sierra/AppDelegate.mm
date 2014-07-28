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

#import "AppDelegate.h"
#import "LevelEditor.h"

@implementation AppDelegate

@synthesize window = _window;

#pragma mark -
#pragma mark Application life cycle

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    [_progressPanel orderOut:nil];
}


- (void) awakeFromNib
{
    _lock = [[NSObject alloc] init];

    [NSApp setDelegate:self];

    _mainWindowController = [[MainWindowController alloc] init];

    [LevelEditor sharedInstance];

    [_mainWindowController.window makeKeyAndOrderFront:self];
}



- (void)showProgressPanel:(NSString *)title
{
    [_progressPanel setTitle:title];
    [_progressPanel makeKeyAndOrderFront:nil];
    [_progressIndicator startAnimation:self];
}


- (void)hideProgressPanel
{
    [_progressPanel orderOut:self];
    [_progressIndicator stopAnimation:self];
}


- (void) dealloc
{
    [_lock release];
    [super dealloc];
}

@end
