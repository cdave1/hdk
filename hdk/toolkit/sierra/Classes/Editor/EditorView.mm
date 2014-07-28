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

#import "EditorView.h"
#import "LevelEditor.h"

#import <OpenGL/OpenGL.h>
#import <OpenGL/gl.h>
#import <OpenGL/glu.h>

id idResponder;

static NSOpenGLPixelFormat *sharedPixelFormat = nil;
static NSOpenGLContext *sharedContext = nil;

mousevalues_t GetMouseValues()
{
    return [idResponder GetMouseValues];
}

@interface EditorView (InternalMethods)
- (NSOpenGLPixelFormat *) createPixelFormat:(NSRect)frame;
- (void) switchToOriginalDisplayMode;
- (BOOL) initGL;
- (void)RenderThread;

@end


@implementation EditorView

- (id) initWithFrame:(NSRect)frame colorBits:(int)numColorBits
           depthBits:(int)numDepthBits fullscreen:(BOOL)runFullScreen
{
    _lock = [[NSLock alloc] init];

    if (!sharedPixelFormat)
    {
        colorBits = numColorBits;
        depthBits = numDepthBits;
        runningFullScreen = runFullScreen;
        originalDisplayMode = (NSDictionary *) CGDisplayCurrentMode(kCGDirectMainDisplay );

        sharedPixelFormat = [self createPixelFormat:frame];
        if (!sharedPixelFormat)
        {
            hdAssert(false);
        }
    }

    if (!sharedContext)
    {
        sharedContext = [[NSOpenGLContext alloc] initWithFormat:sharedPixelFormat shareContext:nil];
    }

    if ((self = [super initWithFrame:frame pixelFormat:sharedPixelFormat]))
    {
        [self clearGLContext];
        NSOpenGLContext *localContext = [[NSOpenGLContext alloc] initWithFormat:sharedPixelFormat
                                                                   shareContext:sharedContext];
        [self setOpenGLContext:localContext];
        [localContext release];

        if(![self initGL])
        {
            [self clearGLContext];
            self = nil;
        }
    }

    [self handleResize];
    [LevelEditor sharedInstance]->Resize(frame.size.width, frame.size.height);

    return self;
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


- (void)startRenderLoop
{
    keepRenderThreadAlive = YES;
    [NSTimer scheduledTimerWithTimeInterval:0.016 target:self selector:@selector(RenderThread) userInfo:nil repeats:YES];
}


- (void)drawRect:(NSRect)rect
{
    LOCK_GL_CONTEXT;
    [LevelEditor sharedInstance]->Draw();
    [[self openGLContext] flushBuffer];
    [super drawRect:rect];
    UNLOCK_GL_CONTEXT;
}


- (void)prepareOpenGL
{
    LOCK_GL_CONTEXT;
    [super prepareOpenGL];
    UNLOCK_GL_CONTEXT;
}


- (void)clearGLContext
{
    LOCK_GL_CONTEXT;
    [super clearGLContext];
    UNLOCK_GL_CONTEXT;
}


- (void)update
{
    LOCK_GL_CONTEXT;
    [super update];
    UNLOCK_GL_CONTEXT;
}


- (void)reshape
{
    LOCK_GL_CONTEXT;
    [LevelEditor sharedInstance]->Resize(self.frame.size.width, self.frame.size.height);
    [super reshape];
    UNLOCK_GL_CONTEXT;
}


- (void)RenderThread
{
    NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
    LOCK_GL_CONTEXT;
    [[self openGLContext] makeCurrentContext];
    [LevelEditor sharedInstance]->Draw();
    [[self openGLContext] flushBuffer];
    UNLOCK_GL_CONTEXT;
    [pool drain];
}


- (BOOL)acceptsFirstMouse:(NSEvent *)theEvent
{
    return YES;
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

    pixelAttribs[ pixNum ] = 0;
    pixelFormat = [ [ NSOpenGLPixelFormat alloc ]
                   initWithAttributes:pixelAttribs ];

    return pixelFormat;
}


- (void) handleResize
{
    [self reshape];
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
    glShadeModel(GL_SMOOTH);
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


- (void)rightMouseDown:(NSEvent *)theEvent
{
    LOCK_GL_CONTEXT;
    hdPrintf("Right Mouse down\n");
    [LevelEditor sharedInstance]->StartPan(theEvent.locationInWindow.x, theEvent.locationInWindow.y);
    [super rightMouseDown:theEvent];
    UNLOCK_GL_CONTEXT;
}


- (void)rightMouseDragged:(NSEvent *)theEvent
{
    LOCK_GL_CONTEXT;
    [LevelEditor sharedInstance]->Pan(theEvent.deltaX, -theEvent.deltaY);
    [super rightMouseDragged:theEvent];
    UNLOCK_GL_CONTEXT;
}


- (void)mouseDown:(NSEvent *)theEvent
{
    uint32 selectedCount = [LevelEditor sharedInstance]->GetSelectedGameObjectsCount();

    [LevelEditor sharedInstance]->MouseDown(theEvent.locationInWindow.x, theEvent.locationInWindow.y);

    if (([LevelEditor sharedInstance]->GetSelectedGameObjectSingle()))
    {
        [[NSNotificationCenter defaultCenter]
         postNotificationName:kSingleItemSelectedNotification
         object:nil];
    }
    else if (selectedCount == 1)
    {
        [[NSNotificationCenter defaultCenter]
         postNotificationName:kSingleItemUnselectedNotification
         object:nil];
    }
}


- (void)mouseDragged:(NSEvent *)theEvent
{
    if (([theEvent modifierFlags] & (NSShiftKeyMask | NSControlKeyMask)) ==
        (NSShiftKeyMask | NSControlKeyMask))
    {
        [LevelEditor sharedInstance]->SetDragStyleScale();
    }
    else if (([theEvent modifierFlags] & NSControlKeyMask) == NSControlKeyMask)
    {
        [LevelEditor sharedInstance]->SetDragStyleRotate();
    }
    else
    {
        [LevelEditor sharedInstance]->SetDragStyleMove();
    }
    
    [LevelEditor sharedInstance]->MouseDrag(theEvent.locationInWindow.x, theEvent.locationInWindow.y);
}


- (void)mouseUp:(NSEvent *)theEvent
{
    bool shiftKeyDown = (([theEvent modifierFlags] & NSShiftKeyMask) == NSShiftKeyMask);
    [LevelEditor sharedInstance]->MouseUp(shiftKeyDown);
    
    if ([LevelEditor sharedInstance]->GetSelectedGameObjectsCount() > 0)
    {
        if (([LevelEditor sharedInstance]->GetSelectedGameObjectSingle()))
        {
            [[NSNotificationCenter defaultCenter] 
             postNotificationName:kSingleItemSelectedNotification
             object:nil];
        }
        else
        {
            [[NSNotificationCenter defaultCenter] 
             postNotificationName:kMultipleItemsSelectedNotification
             object:nil];	
        }
    }
}


- (void)scrollWheel:(NSEvent *)theEvent
{
    hdPrintf("Mouse scroll\n");
    if (theEvent.type == NSScrollWheel)
    {
        [LevelEditor sharedInstance]->Zoom([theEvent deltaY]);
    }
}

@end
