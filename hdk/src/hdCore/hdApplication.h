/*
 *  hdApplication.h
 *  TotemGameOSX
 *
 *  Created by david on 11/09/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_APPLICATION_H_
#define HD_APPLICATION_H_

class hdApplication
	{
	public:
		
		bool InitApplication();
	
		bool PrepareGameLoop();
		
		bool StartGameLoop();
		
		bool QuitApplication();
		
		bool PauseApplication();
		
		bool ResumeApplication();
		
		bool ApplicationWillTerminate();
		
		bool HandleLowMemoryWarning();
		
		bool HandleResize(const float newWidth, const float newHeight);
		
		bool UpdatePhysics();
		
		bool HandlePlayerInput();
		
		bool UpdateScene();
		
		bool RenderScene();
	};


#endif