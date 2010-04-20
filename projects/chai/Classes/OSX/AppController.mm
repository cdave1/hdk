
#import "AppController.h"
#import "hdApplication.h"

#include "hdSoundManager.h"



static hdApplication *shell = NULL;
static bool keepRenderThreadAlive = true;



@interface AppController (Private)
- (void)StartApplication;
- (void)RenderThread;
- (void) createFailed;
@end



@implementation AppController




- (void) update
{
	if(!shell->UpdateScene())
		printf("UpdateScene error\n");
	
    if(!shell->RenderScene())
		printf("RenderScene error\n");
	
	//[_glView swapBuffers];
}


- (void)RenderThread
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	hdSoundManager::InitSoundManager();
	[[_glView openGLContext] makeCurrentContext];
	shell->PrepareGameLoop();
	
	while (keepRenderThreadAlive)
	{
		@synchronized(_lock)
		{
			shell->HandlePlayerInput();
		}
		
		
		@synchronized(_lock)
		{		
			if(!shell->UpdateScene())
				printf("UpdateScene error\n");
		}
		
		//if(!shell->HandleResize([_glWindow frame].size.width, [_glWindow frame].size.height))
		//	printf("UpdateScene error\n");
		
		[[_glView openGLContext] makeCurrentContext];
		//[_glView bindFrameBuffer];
		@synchronized(_lock)
		{
			if(!shell->RenderScene())
				printf("RenderScene error\n");
		}
		[ [ _glView openGLContext ] flushBuffer ];
		//[_glView bindRenderBufferAndPresent];
	}
	//[_glView swapBuffers];
	[pool drain];
}


- (void)StartApplication
{
	
	shell = new hdApplication();
	
	if(!shell->InitApplication())
		printf("InitApplication error\n");
	
	// create our rendering timer
	//[NSTimer scheduledTimerWithTimeInterval:kFrameDuration target:self selector:@selector(update) userInfo:nil repeats:YES];
	
	[NSThread detachNewThreadSelector:@selector(RenderThread) toTarget:self withObject:nil];
}


- (void) awakeFromNib
{  
	_lock = [[NSObject alloc] init];
	
	[ NSApp setDelegate:self ];   // We want delegate notifications
	
	_glView = [ [ AppMainView alloc ] initWithFrame:[ _glWindow frame ]
										  colorBits:32 depthBits:16 fullscreen:FALSE ];
	if( _glView != nil )
	{
		[ _glWindow setContentView:_glView ];
		[ _glWindow makeKeyAndOrderFront:self ];
		[ self StartApplication ];
	}
	else
		[ self createFailed ];
} 


/*
 * Handle key presses
 */
- (void) keyDown:(NSEvent *)theEvent
{
	unichar unicodeKey;
	
	unicodeKey = [ [ theEvent characters ] characterAtIndex:0 ];
	
	hdPrintf("%c", unicodeKey);
	
	switch( unicodeKey )
	{
			// Handle key presses here
	}
} 






- (IBAction) resize:(id)sender
{
	@synchronized(_lock)
	{
		
		
		if(!shell->HandleResize([_glWindow frame].size.width, [_glWindow frame].size.height))
			printf("UpdateScene error\n");
	}
}


/*
 * Set full screen.
 */

- (IBAction)setFullScreen:(id)sender
{
	/*
	[ _glWindow setContentView:nil ];
	if( [ _glView isFullScreen ] )
	{
		if( ![ _glView setFullScreen:FALSE inFrame:[ _glWindow frame ] ] )
			[ self createFailed ];
		else
			[ _glWindow setContentView:_glView ];
	}
	else
	{
		if( ![ _glView setFullScreen:TRUE
							inFrame:NSMakeRect( 0, 0, 800, 600 ) ] )
			[ self createFailed ];
	}
	 */
}


/*
- (void) applicationDidFinishLaunching:(UIApplication*)application
{
	
	CGRect	rect = [[UIScreen mainScreen] bounds];
	
	// create a full-screen window
	//_window = [[UIWindow alloc] initWithFrame:rect];
	
	// create the OpenGL view and add it to the window
	//_glView = [[EAGLView alloc] initWithFrame:rect];
	//_glView = [[EAGLView alloc] initWithFrame:rect pixelFormat:GL_RGB565_OES depthFormat:GL_DEPTH_COMPONENT16_OES preserveBackbuffer:NO];
	//_glView = [[EAGLCameraView alloc] initWithFrame:rect pixelFormat:GL_RGB565_OES depthFormat:GL_DEPTH_COMPONENT16_OES preserveBackbuffer:NO];
	
	
	//[_window addSubview:_glView];
	
	// show the window
	//[_window makeKeyAndVisible];
}
 */


/*
 * Save changes and pause the timer
 */
/*
- (void)applicationWillResignActive:(UIApplication *)application 
{
	@synchronized(_lock)
	{
		if(!shell->PauseApplication())
			printf("PauseApplication error\n");
	}
}*/


/*
 * Un pause timer.
 */
/*
- (void)applicationDidBecomeActive:(UIApplication *)application 
{
	@synchronized(_lock)
	{
		if(!shell->ResumeApplication())
			printf("PauseApplication error\n");
	}
}


-(void)applicationDidReceiveMemoryWarning:(UIApplication *)application 
{
	printf("Application applicationDidReceiveMemoryWarning \n");
	
	@synchronized(_lock)
	{
		if(!shell->HandleLowMemoryWarning())
			printf("Handle low mem error\n");
	}
}
*/


/*
 * Save changes only - dealloc will take care of the rest.
 */
/*
-(void)applicationWillTerminate:(UIApplication *)application 
{
	@synchronized(_lock)
	{
		keepRenderThreadAlive = false;
		if(!shell->ApplicationWillTerminate())
			printf("ApplicationWillTerminate error\n");
	}
}*/


/*
 * Called if we fail to create a valid OpenGL view
 */

- (void) createFailed
{
	NSWindow *infoWindow;
	
	infoWindow = NSGetCriticalAlertPanel( @"Initialization failed",
                                         @"Failed to initialize OpenGL",
                                         @"OK", nil, nil );
	[ NSApp runModalForWindow:infoWindow ];
	[ infoWindow close ];
	[ NSApp terminate:self ];
}



- (void) dealloc
{
	keepRenderThreadAlive = false;
	
	if(!shell->QuitApplication())
		printf("QuitApplication error\n");
	
	
	
	[_glView release];
	[_glWindow release];
	[_lock release];
	
	
	
	[super dealloc];
}

@end
