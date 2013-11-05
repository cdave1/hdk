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
		
		//! \brief initialization before the render API is intialized
		bool InitApplication();
	
		bool PrepareGameLoop();
		
		bool StartGameLoop();
		
		//! \brief release any memory/resources acquired by InitApplication()
		bool QuitApplication();
		
		
		bool PauseApplication();
		
		
		bool ResumeApplication();
		
		
		bool ApplicationWillTerminate();
		
		
		bool HandleLowMemoryWarning();
		
		bool HandleResize(const float newWidth, const float newHeight);
		
		bool UpdatePhysics();
		
		bool HandlePlayerInput();
		
		//! \brief update the camera matrix and other things that need to be done to setup rendering
		bool UpdateScene();
		
		//! \brief It is main application function in which you have to do your own rendering.  Will be
		//! called repeatedly until the application exits.
		bool RenderScene();
	};


#endif