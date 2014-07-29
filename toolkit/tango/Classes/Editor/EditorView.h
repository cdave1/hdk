//
//  EditorView.h
//  Smashed_OSX
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "hdInput.h"

@interface EditorView : NSOpenGLView
{
	int colorBits, depthBits;
	BOOL runningFullScreen;
	NSDictionary *originalDisplayMode;
@private
	mousevalues_t mouseValues;
	mousevalues_t cpyMouseValues;
	BOOL keepRenderThreadAlive;
	NSLock *_lock;
	NSOpenGLContext *_localContext;
	
	
}

- (id) initWithFrame:(NSRect)frame colorBits:(int)numColorBits
		   depthBits:(int)numDepthBits fullscreen:(BOOL)runFullScreen;
- (void) reshape;
- (BOOL) isFullScreen;
//- (BOOL) setFullScreen:(BOOL)enableFS inFrame:(NSRect)frame;
- (mousevalues_t)GetMouseValues;
- (void)startRenderLoop;
- (void) handleResize;
@end