//
//  Delegate.h
//  TotemGame
//
//  Created by david on 25/03/09.
//  Copyright 2009 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "AppMainView.h"
#import "constants.h"
#import "PlatformIncludes.h"

@interface AppController : NSResponder
{
	IBOutlet NSWindow *_glWindow;
	AppMainView*		_glView;
	NSObject*			_lock;
}

- (void) awakeFromNib;
- (void) keyDown:(NSEvent *)theEvent;
- (IBAction) setFullScreen:(id)sender;
- (IBAction) resize:(id)sender;
- (void) dealloc;



@end
