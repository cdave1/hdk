/*
 Oolong Engine for the iPhone / iPod touch
 Copyright (c) 2007-2008 Wolfgang Engel  http://code.google.com/p/oolongengine/
 
 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from the use of this software.
 Permission is granted to anyone to use this software for any purpose, 
 including commercial applications, and to alter it and redistribute it freely, 
 subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 */


#import "Delegate.h"




static hdApplication *shell = NULL;
static bool keepRenderThreadAlive = true;
static int poolDrain = 0;
@interface AppController (Private)
- (void)RenderThread;

- (void)UpdateSceneThread;
- (void)RenderSceneThread;

@end



@implementation AppController

- (void) update
{
	@synchronized(_lock)
	{
		if(!shell->UpdateScene())
			printf("UpdateScene error\n");
	}
	
	@synchronized(_lock)
	{
		
		//[_glView setCurrentContext];
		//[_glView bindFrameBuffer];
		if(!shell->RenderScene())
			printf("RenderScene error\n");
		//[_glView bindRenderBufferAndPresent];
		[_glView swapBuffers];
	}
}


- (void)UpdateSceneThread
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	while (keepRenderThreadAlive)
	{
		@synchronized(_lock)
		{
			[_glView setSecondaryContext];
			if (!shell->HandlePlayerInput())
				printf("HandlePlayerInput error\n");
			if(!shell->UpdateScene())
				printf("UpdateScene error\n");
		}
		[pool drain];
		pool = [[NSAutoreleasePool alloc] init];
	}

}


- (void)RenderSceneThread
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	//[_glView swapBuffers];

	while (keepRenderThreadAlive)
	{
		@synchronized(_lock)
		{
			[_glView setCurrentContext];
			[_glView bindFrameBuffer];
			
			if(!shell->RenderScene())
				printf("RenderScene error\n");
			//[_glView bindRenderBufferAndPresent];
		}
		
		[_glView swapBuffers];
		
		//
		poolDrain = (poolDrain + 1) & 0x001F;
		if (poolDrain == 0)
		{
			[pool drain];
			pool = [[NSAutoreleasePool alloc] init];
		}
	}
	//[_glView swapBuffers];
//	[pool drain]; 
}


- (void)RenderThread
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	[_glView setCurrentContext];
	[_glView bindFrameBuffer];
	
	NSLog(@"Initialising sound manager.");
	
	hdSoundManager::InitSoundManager();
	
	shell->PrepareGameLoop();

	NSLog(@"Starting game loop.");

	while (keepRenderThreadAlive)
	{
		@synchronized(_lock)
		{
			if (!shell->HandlePlayerInput())
				printf("HandlePlayerInput error\n");
		//} 
		 
		//@synchronized(_lock)
		//{
			if(!shell->UpdateScene())
				printf("UpdateScene error\n");
		//} 
		
		//@synchronized(_lock)
		//{
			[_glView bindFrameBuffer];
			[_glView setCurrentContext];
			if(!shell->RenderScene())
				printf("RenderScene error\n");
			[_glView swapBuffers];
		}
		poolDrain = (poolDrain + 1) & 0x001F;
		if (poolDrain == 0)
		{
			[pool drain];
			pool = [[NSAutoreleasePool alloc] init];
		}
	}
	
	if (pool)
		[pool drain];
	//[pool drain];
}


- (void) applicationDidFinishLaunching:(UIApplication*)application
{
	NSLog(@"Application did finish launching start.");
	
	application.statusBarHidden = YES;
	application.idleTimerDisabled = YES;
	
	_lock = [[NSObject alloc] init];
	CGRect	rect = [[UIScreen mainScreen] bounds];
	
	NSLog(@"Initialising camera view.");
	
	// create a full-screen window
	_window = [[UIWindow alloc] initWithFrame:rect];
	
	// create the OpenGL view and add it to the window
	_glView = [[EAGLCameraView alloc] initWithFrame:rect pixelFormat:GL_RGB565_OES depthFormat:GL_DEPTH_COMPONENT16_OES preserveBackbuffer:NO];
	
	NSLog(@"Adding sub view and making key.");
	
	//[_glView setRenderLock:_lock];
	((EAGLCameraView *)_glView).renderLock = _lock;
	
	[_window addSubview:_glView];
	
	// show the window
	[_window makeKeyAndVisible];
	
	

	
	/*
	 NOTES ON GETTING MORE PERFORMANCE OUT OF 1G iPHONEs
	 OPTION 1
	 - Timer
		- RESULT: Timer was unviersally slower than a separate render thread. Couldn't get any more than about 23 FPS with the timer.
	 OPTION 2
	 - A single update thread, containing both game state stepping and frame rendering performs much better - at the rate we specify.
		- RESULT: Generally faster, but some bad choppiness when a lot of physics stuff is happening.
	 OPTION 2A:
	 - Two threads. One thread is physics simulation. The other is everything else.
	 - Physics is only synchronized with clearing the contacts buffer and input handler step.
	 OPTION 3
	 - Trying two threads; game state stepper, and frame renderer.
		- First iteration: Update scene not synchronized with renderer, apart from getting user input.
			- Update scene will only block if getting user input touch values while the render thread is in the critical section.
			- RESULTS:
				- 1st attempt crashes on startup.
				- Old timing code was not valid.
				- Textures are disappearing.
				- Some strange bugs to do with synchronization.
		- Second iteration: Update scene thread is fully synchronized with rendering thread.
	 
	*/
	
	

	
	if(!shell->InitApplication())
		printf("InitApplication error\n");
	
	NSLog(@"Initialising shell application.");

	
	// create our rendering timer
	//
	
	NSLog(@"Kicking off main thread...");
	
	//char *systemName = SystemSettings_SystemName();
	
	// Use the timer for old iPhones/Sim.
#if 0
	if (0 == strcmp(systemName, "iPhone1,1") || 0 == strcmp(systemName, "iPod1,1") || 0 == strcmp(systemName, "i386"))
	{
		double minFrameDuration = SystemSettings_MinFrameDuration();
		[NSTimer scheduledTimerWithTimeInterval:minFrameDuration target:self selector:@selector(update) userInfo:nil repeats:YES];
	}
	else
	{
		[NSThread detachNewThreadSelector:@selector(RenderThread) toTarget:self withObject:nil];
	}
#endif
	 
#if 1
	[NSThread detachNewThreadSelector:@selector(RenderThread) toTarget:self withObject:nil];
#endif
	
#if 0
	NSLog(@"Initialising sound manager.");
	
	hdSoundManager::InitSoundManager();
	
	shell->PrepareGameLoop();
	[NSThread detachNewThreadSelector:@selector(UpdateSceneThread) toTarget:self withObject:nil];
	[NSThread detachNewThreadSelector:@selector(RenderSceneThread) toTarget:self withObject:nil];
#endif
	
	
	
}


/*
 * Save changes and pause the timer
 */
- (void)applicationWillResignActive:(UIApplication *)application 
{
	@synchronized(_lock)
	{
		if(!shell->PauseApplication())
			printf("PauseApplication error\n");
	}
}


/*
 * Un pause timer.
 */
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


/*
 * Save changes only - dealloc will take care of the rest.
 */
-(void)applicationWillTerminate:(UIApplication *)application 
{
	keepRenderThreadAlive = false;
	@synchronized(_lock)
	{
		if(!shell->ApplicationWillTerminate())
			printf("ApplicationWillTerminate error\n");
	}
}


- (void) dealloc
{
	keepRenderThreadAlive = false;
	
	@synchronized(_lock)
	{
		if(!shell->QuitApplication())
			printf("QuitApplication error\n");
	}
	
	[_glView release];
	[_window release];
	[_lock release];
	
	
	
	[super dealloc];
}

@end


/*
 
 
 - (void)UpdatePhysicsThread
 {
 NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
 while(keepRenderThreadAlive)
 {
 //@synchronized(_lock)
 //{
 @synchronized(_physicsLock)
 {
 if (shell)
 shell->UpdatePhysics();
 //}
 }
 
 if (poolDrain == 0)
 {
 [pool drain];
 pool = [[NSAutoreleasePool alloc] init];
 }
 }
 
 if (pool)
 [pool drain];
 }
 
 
 - (void)RenderThread
 {
 NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];

 [_glView setCurrentContext];
 [_glView bindFrameBuffer];
 while (1)
 {
 
 if(!shell->UpdateScene())
 printf("UpdateScene error\n");
 if(!shell->RenderScene())
 printf("RenderScene error\n");
 [_glView swapBuffers];
 usleep(50);
 }

[_glView setCurrentContext];
[_glView bindFrameBuffer];

NSLog(@"Initialising sound manager.");

hdSoundManager::InitSoundManager();

shell->PrepareGameLoop();


NSLog(@"Starting game loop.");

//[_glView swapBuffers];

while (keepRenderThreadAlive)
{
	@synchronized(_lock)
	{
		@synchronized(_physicsLock)
		{
			shell->HandlePlayerInput();
		}
		
		@synchronized(_physicsLock)
		{
			shell->UpdateScene();
		}
		
		[_glView bindFrameBuffer];
		[_glView setCurrentContext];
		@synchronized(_physicsLock)
		{
			shell->RenderScene();
		}
		[_glView swapBuffers];
	}
	poolDrain = (poolDrain + 1) & 0x001F;
	if (poolDrain == 0)
	{
		[pool drain];
		pool = [[NSAutoreleasePool alloc] init];
	}
}

if (pool)
[pool drain];
//[pool drain];
}
 
 */
