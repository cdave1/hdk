//
//  AppMainView.m
//  TotemGameOSX
//
//  Created by david on 11/09/09.
//  Copyright 2009 n/a. All rights reserved.
//

#import "AppMainView.h"
#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>

id idResponder;


mousevalues_t GetMouseValues()
{
	return [idResponder GetMouseValues];
}

@interface AppMainView (InternalMethods)
- (NSOpenGLPixelFormat *) createPixelFormat:(NSRect)frame;
- (void) switchToOriginalDisplayMode;
- (BOOL) initGL;
mousevalues_t mouseValues;
mousevalues_t cpyMouseValues;
@end

@implementation AppMainView

- (id) initWithFrame:(NSRect)frame colorBits:(int)numColorBits
		   depthBits:(int)numDepthBits fullscreen:(BOOL)runFullScreen
{
	NSOpenGLPixelFormat *pixelFormat;
	
	colorBits = numColorBits;
	depthBits = numDepthBits;
	runningFullScreen = runFullScreen;
	originalDisplayMode = (NSDictionary *) CGDisplayCurrentMode(
																kCGDirectMainDisplay );
	pixelFormat = [ self createPixelFormat:frame ];
	if( pixelFormat != nil )
	{
		self = [ super initWithFrame:frame pixelFormat:pixelFormat ];
		[ pixelFormat release ];
		if( self )
		{
			[ [ self openGLContext ] makeCurrentContext ];
			if( runningFullScreen )
				[ [ self openGLContext ] setFullScreen ];
			[ self reshape ];
			if( ![ self initGL ] )
			{
				[ self clearGLContext ];
				self = nil;
			}
		}
		
		idResponder = self;
		memset(&mouseValues, 0, sizeof(mousevalues_t));
	}
	else
		self = nil;
	
	return self;
}


/*
 * Create a pixel format and possible switch to full screen mode
 */
- (NSOpenGLPixelFormat *) createPixelFormat:(NSRect)frame
{
	NSOpenGLPixelFormatAttribute pixelAttribs[ 16 ];
	int pixNum = 0;
	NSDictionary *fullScreenMode;
	NSOpenGLPixelFormat *pixelFormat;
	
	pixelAttribs[ pixNum++ ] = NSOpenGLPFADoubleBuffer;
	pixelAttribs[ pixNum++ ] = NSOpenGLPFAAccelerated;
	pixelAttribs[ pixNum++ ] = NSOpenGLPFAColorSize;
	pixelAttribs[ pixNum++ ] = colorBits;
	pixelAttribs[ pixNum++ ] = NSOpenGLPFADepthSize;
	pixelAttribs[ pixNum++ ] = depthBits;
	
	if( runningFullScreen )  // Do this before getting the pixel format
	{
		pixelAttribs[ pixNum++ ] = NSOpenGLPFAFullScreen;
		fullScreenMode = (NSDictionary *) CGDisplayBestModeForParameters(
																		 kCGDirectMainDisplay,
																		 colorBits, frame.size.width,
																		 frame.size.height, NULL );
		CGDisplayCapture( kCGDirectMainDisplay );
		CGDisplayHideCursor( kCGDirectMainDisplay );
		CGDisplaySwitchToMode( kCGDirectMainDisplay,
							  (CFDictionaryRef) fullScreenMode );
	}
	pixelAttribs[ pixNum ] = 0;
	pixelFormat = [ [ NSOpenGLPixelFormat alloc ]
                   initWithAttributes:pixelAttribs ];
	
	return pixelFormat;
}


/*
 * Enable/disable full screen mode
 */
- (BOOL) setFullScreen:(BOOL)enableFS inFrame:(NSRect)frame
{
	BOOL success = FALSE;
	NSOpenGLPixelFormat *pixelFormat;
	NSOpenGLContext *newContext;
	
	[ [ self openGLContext ] clearDrawable ];
	if( runningFullScreen )
		[ self switchToOriginalDisplayMode ];
	runningFullScreen = enableFS;
	pixelFormat = [ self createPixelFormat:frame ];
	if( pixelFormat != nil )
	{
		newContext = [ [ NSOpenGLContext alloc ] initWithFormat:pixelFormat
												   shareContext:nil ];
		if( newContext != nil )
		{
			[ super setFrame:frame ];
			[ super setOpenGLContext:newContext ];
			[ newContext makeCurrentContext ];
			if( runningFullScreen )
				[ newContext setFullScreen ];
			[ self reshape ];
			if( [ self initGL ] )
				success = TRUE;
		}
		[ pixelFormat release ];
	}
	if( !success && runningFullScreen )
		[ self switchToOriginalDisplayMode ];
	
	return success;
}


/*
 * Switch to the display mode in which we originally began
 */
- (void) switchToOriginalDisplayMode
{
	CGDisplaySwitchToMode( kCGDirectMainDisplay,
                          (CFDictionaryRef) originalDisplayMode );
	CGDisplayShowCursor( kCGDirectMainDisplay );
	CGDisplayRelease( kCGDirectMainDisplay );
}


/*
 * Initial OpenGL setup
 */
- (BOOL) initGL
{ 
	glShadeModel( GL_SMOOTH );                // Enable smooth shading
	//glClearColor( 0.0f, 0.0f, 0.0f, 0.5f );   // Black background
	//glClearDepth( 1.0f );                     // Depth buffer setup
	//glEnable( GL_DEPTH_TEST );                // Enable depth testing
	//glDepthFunc( GL_LEQUAL );                 // Type of depth test to do
	// Really nice perspective calculations
	//glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	
	return TRUE;
}


/*
 * Are we full screen?
 */
- (BOOL) isFullScreen
{
	return runningFullScreen;
}


- (mousevalues_t)GetMouseValues
{
	memcpy(&cpyMouseValues, &mouseValues, sizeof(mousevalues_t));
	
	if (mouseValues.leftButton.isUp)
	{
		memset(&(mouseValues.leftButton), 0, sizeof(clickinfo_t));
	}
	
	if (mouseValues.rightButton.isUp)
	{
		mouseValues.rightButton.isDown = false;
		mouseValues.rightButton.isMoving = false;
		mouseValues.rightButton.isUp = false;
	}
	
	if (mouseValues.scrollWheel.isMoving)
	{
		memset(&(mouseValues.scrollWheel), 0, sizeof(clickinfo_t));
	}
	
	mouseValues.xPrevious = mouseValues.xCurrent;
	mouseValues.yPrevious = mouseValues.yCurrent;
	
	
	return cpyMouseValues;
}

- (void)mouseDown:(NSEvent *)theEvent
{
	mouseValues.xPrevious = mouseValues.xCurrent;
	mouseValues.yPrevious = mouseValues.yCurrent;
	
	mouseValues.xCurrent = theEvent.locationInWindow.x;
	mouseValues.yCurrent = theEvent.locationInWindow.y;
	
	if (theEvent.type == NSLeftMouseDown)
	{
		assert(mouseValues.leftButton.isDown == false &&
			   mouseValues.leftButton.isUp == false);
		
		mouseValues.leftButton.isDown = true;
		mouseValues.leftButton.isMoving = false;
		mouseValues.leftButton.isUp = false;
		
		mouseValues.leftButton.xClickDown = theEvent.locationInWindow.x; 
		mouseValues.leftButton.yClickDown = theEvent.locationInWindow.y;
		mouseValues.leftButton.clickDownTimeStamp = theEvent.timestamp;
		
		mouseValues.leftButton.clickCount = [theEvent clickCount];
	}
	else if (theEvent.type == NSRightMouseDown)
	{
		assert(mouseValues.rightButton.isDown == false &&
			   mouseValues.rightButton.isUp == false);
		
		mouseValues.rightButton.isDown = true;
		mouseValues.rightButton.isMoving = false;
		mouseValues.rightButton.isUp = false;
		
		mouseValues.rightButton.xClickDown = theEvent.locationInWindow.x; 
		mouseValues.rightButton.yClickDown = theEvent.locationInWindow.y;
		mouseValues.rightButton.clickDownTimeStamp = theEvent.timestamp;
		
		mouseValues.rightButton.clickCount = [theEvent clickCount];
	}
}


- (void)mouseUp:(NSEvent *)theEvent
{
	mouseValues.xPrevious = mouseValues.xCurrent;
	mouseValues.yPrevious = mouseValues.yCurrent;
	
	mouseValues.xCurrent = theEvent.locationInWindow.x;
	mouseValues.yCurrent = theEvent.locationInWindow.y;
	
	if (theEvent.type == NSLeftMouseUp)
	{
		assert((mouseValues.leftButton.isDown ||
				mouseValues.leftButton.isMoving) &&
			   !mouseValues.leftButton.isUp);
		
		if ([theEvent timestamp] - mouseValues.leftButton.clickDownTimeStamp < 0.02)
		{
			mouseValues.leftButton.isDown = true;
		}
		else
		{
			mouseValues.leftButton.isDown = false;
		}
		
		mouseValues.leftButton.isMoving = false;
		mouseValues.leftButton.isUp = true;
		
		mouseValues.leftButton.xClickUp = theEvent.locationInWindow.x; 
		mouseValues.leftButton.yClickUp = theEvent.locationInWindow.y;
		mouseValues.leftButton.clickUpTimeStamp = theEvent.timestamp;
		
		mouseValues.leftButton.clickCount = [theEvent clickCount];
	}
	else if (theEvent.type == NSRightMouseDown)
	{
		assert((mouseValues.rightButton.isDown ||
				mouseValues.rightButton.isMoving) &&
			   !mouseValues.rightButton.isUp);
		
		if ([theEvent timestamp] - mouseValues.rightButton.clickDownTimeStamp < 0.02)
		{
			mouseValues.rightButton.isDown = true;
		}
		else
		{
			mouseValues.rightButton.isDown = false;
		}
		
		mouseValues.rightButton.isMoving = false;
		mouseValues.rightButton.isUp = true;
		
		mouseValues.rightButton.xClickUp = theEvent.locationInWindow.x; 
		mouseValues.rightButton.yClickUp = theEvent.locationInWindow.y;
		mouseValues.rightButton.clickDownTimeStamp = theEvent.timestamp;
		
		mouseValues.rightButton.clickCount = [theEvent clickCount];
	}
}


- (void)mouseMoved:(NSEvent *)theEvent
{
	if (theEvent.type == NSMouseMoved)
	{
		mouseValues.xPrevious = mouseValues.xCurrent;
		mouseValues.yPrevious = mouseValues.yCurrent;
		
		mouseValues.xCurrent = theEvent.locationInWindow.x;
		mouseValues.yCurrent = theEvent.locationInWindow.y;
		
		if (mouseValues.leftButton.isDown)
		{
			mouseValues.leftButton.isDown = false;
			mouseValues.leftButton.isMoving = true;
			mouseValues.leftButton.isUp = false;
		}
		
		if (mouseValues.rightButton.isDown)
		{
			mouseValues.rightButton.isDown = false;
			mouseValues.rightButton.isMoving = true;
			mouseValues.rightButton.isUp = false;
		}
	}
}


- (void)mouseDragged:(NSEvent *)theEvent
{
	mouseValues.xPrevious = mouseValues.xCurrent;
	mouseValues.yPrevious = mouseValues.yCurrent;
	
	mouseValues.xCurrent = theEvent.locationInWindow.x;
	mouseValues.yCurrent = theEvent.locationInWindow.y;
	
	if (theEvent.type == NSLeftMouseDragged)
	{
		if (mouseValues.leftButton.isDown)
		{
			mouseValues.leftButton.isDown = false;
			mouseValues.leftButton.isMoving = true;
			mouseValues.leftButton.isUp = false;
		}
	}
	else if (theEvent.type == NSRightMouseDragged)
	{
		if (mouseValues.rightButton.isDown)
		{
			mouseValues.rightButton.isDown = false;
			mouseValues.rightButton.isMoving = true;
			mouseValues.rightButton.isUp = false;
		}
	}
}

- (void)scrollWheel:(NSEvent *)theEvent
{
	if (theEvent.type == NSScrollWheel)
	{
		mouseValues.scrollWheel.isMoving = true;
		mouseValues.scrollWheel.xClickDown = [theEvent deltaX];
		mouseValues.scrollWheel.yClickDown = [theEvent deltaY];
		
		//printf("%3.2f, %3.2f\n", [theEvent deltaX], [theEvent deltaY]);
	}
}


/*
 * Cleanup
 */
- (void) dealloc
{
	if( runningFullScreen )
		[ self switchToOriginalDisplayMode ];
	[ originalDisplayMode release ];
	[super dealloc];
}
@end
