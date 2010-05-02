//
//  OpenGLAppDelegate.h
//  OpenGL
//
//  Created by David Petrie on 28/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MainWindowController.h"

@interface AppDelegate : NSObject 
{
@private
	NSObject*	_lock;
	NSWindow *_window;
	
	MainWindowController *_mainWindowController;
	
	
	// ItemPropertiesWindowController
	// LevelPropertiesWindowController
	// 
}

@property (assign) IBOutlet NSWindow *window;

//- (void) keyDown:(NSEvent *)theEvent;
//- (IBAction) setFullScreen:(id)sender;
//- (IBAction) resize:(id)sender;

@end
