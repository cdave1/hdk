//
//  OpenGLAppDelegate.m
//  OpenGL
//
//  Created by David Petrie on 28/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "AppDelegate.h"
#import "LevelEditor.h"

@interface AppDelegate ()
@end


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
