//
//  EditorViewController.m
//  Sierra
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "EditorViewController.h"
#import "hdGraphicsLibrary.h"
#import "InterfaceBuilder.h"

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
