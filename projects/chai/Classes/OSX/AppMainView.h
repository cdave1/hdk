//
//  AppMainView.h
//  TotemGameOSX
//
//  Created by david on 11/09/09.
//  Copyright 2009 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "hdMouseInput.h"

@interface AppMainView : NSOpenGLView
{
	int colorBits, depthBits;
	BOOL runningFullScreen;
	NSDictionary *originalDisplayMode;
}

- (id) initWithFrame:(NSRect)frame colorBits:(int)numColorBits
		   depthBits:(int)numDepthBits fullscreen:(BOOL)runFullScreen;
- (void) reshape;
- (void) drawRect:(NSRect)rect;
- (BOOL) isFullScreen;
- (BOOL) setFullScreen:(BOOL)enableFS inFrame:(NSRect)frame;
- (void) dealloc;
- (mousevalues_t)GetMouseValues;
@end
