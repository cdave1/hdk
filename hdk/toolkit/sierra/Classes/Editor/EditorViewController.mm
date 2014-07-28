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

#import "EditorViewController.h"
#import "hdGraphicsLibrary.h"
#import "LevelEditor.h"

@interface EditorViewController ()
- (void) createFailed;
@end


@implementation EditorViewController

@synthesize editorView = _editorView;

- (id) init
{
    if ((self = [super initWithNibName:@"EditorViewController" bundle:nil]))
    {
        _editorView = [[EditorView alloc] initWithFrame:self.view.frame
                                              colorBits:32 depthBits:16 fullscreen:FALSE];
        if(_editorView == nil)
            [self createFailed];
        [self.view addSubview:_editorView];
        [_editorView startRenderLoop];
    }
    return self;
}


- (void) handleResize
{
    [_editorView handleResize];
}


- (void) createFailed
{
    NSWindow *infoWindow;

    infoWindow = NSGetCriticalAlertPanel( @"Initialization failed",
                                         @"Failed to initialize OpenGL",
                                         @"OK", nil, nil );
    [NSApp runModalForWindow:infoWindow];
    [infoWindow close];
    [NSApp terminate:self];
}

@end
