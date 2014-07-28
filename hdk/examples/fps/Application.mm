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

 /*
 * Copyright (c) 2014 Hackdirt Ltd.
 * Author: David Petrie (david@davidpetrie.com)
 * 
 * Significant changes made to original Oolong code.
 */

#include "hdk.h"
#include "hdInput/hdMultiTouchView.h"
#include "hdInput/hdAccelerometer.h"
#include "PlatformIncludes.h"

#include <stdio.h>
#include <sys/time.h> 

#include "AppController.h"

#define kAccelerometerFrequency		20.0 //Hz

typedef struct
{
	int frames;
	CFTimeInterval	LastTimeInterval;
	CFTimeInterval	LastFrameTime;
	CFTimeInterval	CurrentTime;
	CFTimeInterval	LastFPSUpdateTime;
	CFTimeInterval	TimeSinceLastFrame;
	bool isPrintable;
	int totalFrames;
	int totalSecs;
} timerValues_t;

bool synchronizedUpdates;
float frameDuration;
bool threadSleepingAllowed;
short sleepDuration;

AppController* controller;

#if (TARGET_IPHONE_SIMULATOR == 1) || (TARGET_OS_IPHONE == 1)
TouchScreenValues *TouchScreen;
int CountTouchesBegan;
int CountTouchesMoved;
Accel *gAccel;
#else
mousevalues_t MouseValues;
hdUIImage *mousePoint;
#endif

static timerValues_t m_handleInputTimerValues;
static timerValues_t m_updateSceneTimerValues;
static timerValues_t m_renderSceneTimerValues;

 
void HandleUserInput();
 
void InitTimerValues(timerValues_t& timerValues);

void UpdateTimerValues(timerValues_t& timerValues);



bool hdApplication::InitApplication(bool synchronized)
{
	char fsRootPath[512];
	
	SystemSettings_Init();
	
	InitMatrixSettings();
	
	hdglClearBuffers();
	
#if (TARGET_IPHONE_SIMULATOR == 1) || (TARGET_OS_IPHONE == 1)
	gAccel = [Accel alloc];
	[gAccel SetupAccelerometer: kAccelerometerFrequency];
#endif
	
	// Root file path.
	snprintf(fsRootPath, 512, "%s/BaseDir/", [[[NSBundle mainBundle] resourcePath] cStringUsingEncoding:NSASCIIStringEncoding]);
	
	FileSystem_Init(fsRootPath);
	 
	/*
	 * script files loader
	 */
	Scripts_Init();

	printf("Running arch: %s\n", SystemSettings_SystemArchName());
#if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE	
	/*
	 * Config file loader 
	 */
	if (0 == strcmp(SystemSettings_SystemName(), "iPhone1,1") || 
		0 == strcmp(SystemSettings_SystemName(), "iPod1,1"))
	{
		hdConfig::LoadConfigFile("Config/iPhone_1g.config");
	}
	else if  (0 == strcmp(SystemSettings_SystemName(), "iPhone1,2") || 
			  0 == strcmp(SystemSettings_SystemName(), "iPod1,2") ||
			  0 == strcmp(SystemSettings_SystemName(), "iPod2,1"))
	{
		hdConfig::LoadConfigFile("Config/iPhone_2g.config");
	}
	else if (0 == strcmp(SystemSettings_SystemName(), "iPhone2,1"))
	{ 
		hdConfig::LoadConfigFile("Config/iPhone_3gs.config");
	}
	else if (0 == strcmp(SystemSettings_SystemName(), "iPod3,1"))
	{
		hdConfig::LoadConfigFile("Config/iPod_31.config");
	}
	else if (0 == strcmp(SystemSettings_SystemName(), "i386")) // SIM
	{
		hdConfig::LoadConfigFile("Config/iPhone_Sim.config");
	}  
	else
	{
		hdConfig::LoadConfigFile("Config/iPhone_3gs.config");
	} 
#else
	hdConfig::LoadConfigFile("Config/osx.config");
	
	mousePoint = new hdUIImage("Interface/mousePoint.png", NULL);
#endif
	
	frameDuration = strtof(hdConfig::GetValueForKey(CONFIG_MINFRAMEDURATION_KEY).c_str(), NULL);
	frameDuration = hdClamp(frameDuration, 0.016f, 0.05f);
	
	if (synchronized)
	{
		synchronizedUpdates = true;
		sleepDuration = 0;
		threadSleepingAllowed = false;
	}
	else
	{
		if (0 == strcmp(SystemSettings_SystemName(), "iPhone1,1") || 
			0 == strcmp(SystemSettings_SystemName(), "iPod1,1"))
		{
			// Don't sleep too long due to weird timing issues.
			sleepDuration = 250;
			threadSleepingAllowed = false; 
		}
		else if (0 == strcmp(SystemSettings_SystemName(), "iPhone2,1")) 
		{
			sleepDuration = 500;
			threadSleepingAllowed = false;
		}
		else
		{
			sleepDuration = 1000;
			threadSleepingAllowed = true;
		} 
	}
	
	return true;
} 


bool hdApplication::PrepareGameLoop()
{
	InitTimerValues(m_handleInputTimerValues);
	InitTimerValues(m_updateSceneTimerValues);
	InitTimerValues(m_renderSceneTimerValues);
	
	m_renderSceneTimerValues.isPrintable = true;
	
	controller = new AppController();
	return true;
}



bool hdApplication::QuitApplication()
{
	if (controller != NULL)
	{
		delete controller;
	}
	controller = NULL;
	
	return true;
}


bool hdApplication::PauseApplication()
{
	// save changes
	if (controller == NULL) return false;
	controller->SaveGameState();
	return true;
}


bool hdApplication::ResumeApplication()
{
	return true;
}


bool hdApplication::HandleLowMemoryWarning()
{
	if (controller == NULL) return false;
	controller->SaveGameState();
	
	controller->FreeIdleMemory();
	
	return true;
}


bool hdApplication::ApplicationWillTerminate()
{
	// save changes
	if (controller == NULL) return false;
	controller->SaveGameState();
	
	if (controller != NULL)
	{
		printf("Deleting main controller\n");
		delete controller;
		controller = NULL;
	}
	
	
	return true;
}


void InitTimerValues(timerValues_t& timerValues)
{
	timerValues.frames = 0;
	timerValues.LastTimeInterval = 0;
	timerValues.LastFrameTime = 0;
	timerValues.CurrentTime = 0;
	timerValues.LastFPSUpdateTime = 0;
	timerValues.TimeSinceLastFrame = 0;
	timerValues.isPrintable = false;
	timerValues.totalFrames = 0;
	timerValues.totalSecs = 0;
}


void UpdateTimerValues(timerValues_t& timerValues)
{
#if (TARGET_IPHONE_SIMULATOR == 1) || (TARGET_OS_IPHONE == 1)
	/*
	 * CACurrentMediaTime was recommended by an Apple engineer
	 * for thread and game loop timing.
	 */
	timerValues.CurrentTime = CACurrentMediaTime();
#else
	timerValues.CurrentTime = CFAbsoluteTimeGetCurrent();
#endif
	 
	if(timerValues.LastFPSUpdateTime == 0) 
		timerValues.LastFPSUpdateTime = timerValues.CurrentTime;
	
	if (timerValues.LastFrameTime == 0)
		timerValues.LastFrameTime = timerValues.CurrentTime;
	 
	if ((timerValues.CurrentTime - timerValues.LastFPSUpdateTime) > 1.0f)
	{ 
//#ifdef DEBUG 
		if (timerValues.isPrintable)
		{
			printf("fps: %d\n", timerValues.frames);
			timerValues.totalFrames += timerValues.frames;
			++timerValues.totalSecs;
			if (timerValues.totalSecs == 10)
			{
				printf("Frame rate: %d\n", timerValues.totalFrames/timerValues.totalSecs);
				timerValues.totalSecs =0;
				timerValues.totalFrames = 0;
			}
		} 
//#endif 
		
		timerValues.frames = 0;
		timerValues.LastFPSUpdateTime = timerValues.CurrentTime;
	} 
	
	timerValues.TimeSinceLastFrame = timerValues.CurrentTime - timerValues.LastFrameTime;
} 
 

bool hdApplication::HandlePlayerInput()
{
	//return;
	
	UpdateTimerValues(m_handleInputTimerValues);
	
	if (m_handleInputTimerValues.TimeSinceLastFrame >= (frameDuration)) // - 0.005f)) 
	{
		m_handleInputTimerValues.frames++;
		m_handleInputTimerValues.LastFrameTime = m_updateSceneTimerValues.CurrentTime;
		
		HandleUserInput();
	}
	else 
	{ 
		if (threadSleepingAllowed)
			usleep(((frameDuration) - m_handleInputTimerValues.TimeSinceLastFrame) * sleepDuration);
	}
	return true;
} 
 

bool hdApplication::UpdateScene()
{	
	UpdateTimerValues(m_updateSceneTimerValues);
	
	if (m_updateSceneTimerValues.TimeSinceLastFrame >= frameDuration) 
	{
		m_updateSceneTimerValues.frames++;
		m_updateSceneTimerValues.LastFrameTime = m_updateSceneTimerValues.CurrentTime;
		
		//HandleUserInput();
		controller->Step(m_updateSceneTimerValues.TimeSinceLastFrame, frameDuration);
	}
	else
	{
		if (threadSleepingAllowed)
			usleep((frameDuration - m_updateSceneTimerValues.TimeSinceLastFrame) * sleepDuration);
	}
	
	return true;
}



void HandleUserInput()
{
#if (TARGET_IPHONE_SIMULATOR == 1) || (TARGET_OS_IPHONE == 1)
	TouchScreen = GetValuesMultiTouchScreen();
	
	int TouchCount = GetTouchCount() + 0;
	
	double AccelerometerVector[3];
	[gAccel GetAccelerometerVector:(double *) AccelerometerVector];
	controller->HandleOrientationVectorUpdate(AccelerometerVector[0], AccelerometerVector[1], AccelerometerVector[2]);

	
	if (TouchScreen == nil) return;
	 
#ifdef DEBUG
	if (TouchCount > 0)
		hdPrintf("Handling touches\n"); 
#endif
	
	if (TouchCount == 1)
	{
		if (TouchScreen->TouchDown)
		{
#ifdef DEBUG
			hdPrintf("HandleTapDown: %d, %d\n", TouchScreen->LocationXTouchMoved, TouchScreen->LocationYTouchMoved);
#endif
			controller->HandleTapDown(TouchScreen->LocationXTouchMoved, TouchScreen->LocationYTouchMoved, TouchScreen->TapCount);
		}
		
		if (TouchScreen->TouchMoved)
		{
#ifdef DEBUG
			hdPrintf("HandleTapMoved: %d, %d\n", TouchScreen->LocationXTouchMoved, TouchScreen->LocationYTouchMoved);
#endif
			controller->HandleTapMovedSingle(TouchScreen[0].LocationXTouchMovedPrevious, 
											 TouchScreen[0].LocationYTouchMovedPrevious,
											 TouchScreen[0].LocationXTouchMoved, 
											 TouchScreen[0].LocationYTouchMoved);
		}
		 
		if (TouchScreen->TouchUp)
		{
#ifdef DEBUG
			hdPrintf("HandleTapUp: %d, %d\n", TouchScreen->LocationXTouchMoved, TouchScreen->LocationYTouchMoved);
#endif
			controller->HandleTapUp(TouchScreen->LocationXTouchMoved, TouchScreen->LocationYTouchMoved, TouchScreen->TapCount);
			
			if (TouchScreen->TapCount == 1)
			{
				controller->HandleSingleTap(TouchScreen->LocationXTouchBegan, TouchScreen->LocationYTouchBegan);
			}
			else if (TouchScreen->TapCount == 2)
			{
				controller->HandleDoubleTap(TouchScreen->LocationXTouchBegan, TouchScreen->LocationYTouchBegan);
			}
		}
	}
	else if (TouchCount == 2)
	{
		controller->HandleTapMovedDouble(TouchScreen[0].LocationXTouchMovedPrevious, 
										 TouchScreen[0].LocationYTouchMovedPrevious,
										 TouchScreen[0].LocationXTouchMoved, 
										 TouchScreen[0].LocationYTouchMoved,
										 TouchScreen[1].LocationXTouchMovedPrevious, 
										 TouchScreen[1].LocationYTouchMovedPrevious,
										 TouchScreen[1].LocationXTouchMoved, 
										 TouchScreen[1].LocationYTouchMoved);
	}
#else
	MouseValues = GetMouseValues();
	
	if (MouseValues.leftButton.isDown)
	{
		controller->HandleTapDown(MouseValues.leftButton.xClickDown, 
								  MouseValues.leftButton.yClickDown, 
								  1);
	}
	if (MouseValues.leftButton.isMoving)
	{
		controller->HandleTapMovedSingle(MouseValues.xPrevious, 
										 MouseValues.yPrevious,
										 MouseValues.xCurrent, 
										 MouseValues.yCurrent);
	}
	if (MouseValues.leftButton.isUp)
	{
		controller->HandleTapUp(MouseValues.leftButton.xClickUp, 
								MouseValues.leftButton.yClickUp, 
								1);
		
		if (MouseValues.leftButton.clickCount == 1)
		{
			controller->HandleSingleTap(MouseValues.leftButton.xClickDown, 
										MouseValues.leftButton.yClickDown);
		}
		else if (MouseValues.leftButton.clickCount == 2)
		{
			controller->HandleDoubleTap(MouseValues.leftButton.xClickDown, 
										MouseValues.leftButton.yClickDown);
		}
	}
	
	if (MouseValues.scrollWheel.isMoving)
	{
		if (MouseValues.scrollWheel.yClickDown > 0)
		{
			controller->HandleTapMovedDouble(0, 0, 0, 0,
										 0, 0, 
										 MouseValues.scrollWheel.xClickDown,
										 3.0f * MouseValues.scrollWheel.yClickDown);		
		}
		else
		{
			controller->HandleTapMovedDouble(0, 0, 0, 0,
											 MouseValues.scrollWheel.xClickDown,
											 3.0f * MouseValues.scrollWheel.yClickDown,
											 0, 0);			
		}
	}
	
#endif
}


bool hdApplication::RenderScene()
{ 
	UpdateTimerValues(m_renderSceneTimerValues);
	
	if (m_renderSceneTimerValues.TimeSinceLastFrame >= frameDuration) 
	{
		m_renderSceneTimerValues.frames++;
		m_renderSceneTimerValues.LastFrameTime = m_renderSceneTimerValues.CurrentTime;
	 
		hdInitImmediateModeGL();
		 
		hdglClearBuffers();
		
#if TARGET_GL_OPENGL == 1		
		glEnable(GL_LINE_SMOOTH);
#endif
		
		controller->Draw();	

#ifdef DEBUG
		
#if (TARGET_IPHONE_SIMULATOR == 1) || (TARGET_OS_IPHONE == 1)
		if (TouchScreen != NULL)
		{
			for (int i = 0; i < kMultiTouchScreenValues; i++)
			{
				if (TouchScreen[i].TouchDown)
				{
					 
					hdPrintf("%d: touch: X: %3.2f Y: %3.2f Tap Count: %d", 
												i + 1,
								 				TouchScreen[i].LocationXTouchBegan, 
												TouchScreen[i].LocationYTouchBegan,
												TouchScreen[i].TapCount);
				} 
				
				if (TouchScreen[i].TouchMoved)
				{
					hdPrintf("\t\tmoved: X: %3.2f Y: %3.2f Tap Count: %d", 
												TouchScreen[i].LocationXTouchMoved, 
												TouchScreen[i].LocationYTouchMoved,
												TouchScreen[i].TapCount);
				}
			}
		}
#else
#ifdef DEBUG
		mousePoint->SetTexture(hdTextureManager::Instance()->FindTexture("Interface/mousePoint.png", TT_16Pic));
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glPushMatrix();
		hdglOrthof(0, 480, 0, 320, 0, -100);
		
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if (MouseValues.leftButton.isDown)
		{
			mousePoint->SetAs2DBox(MouseValues.leftButton.xClickDown - 32.0f,
								   MouseValues.leftButton.yClickDown - 32.0f,
								   64.0f, 64.0f);
			mousePoint->Draw();
		}
		if (MouseValues.leftButton.isMoving)
		{
			mousePoint->SetAs2DBox(MouseValues.xCurrent - 32.0f,
								   MouseValues.yCurrent - 32.0f,
								   64.0f, 64.0f);
			mousePoint->Draw();
		}
		if (MouseValues.leftButton.isUp)
		{
			mousePoint->SetAs2DBox(MouseValues.leftButton.xClickUp - 32.0f,
								   MouseValues.leftButton.yClickUp - 32.0f,
								   64.0f, 64.0f);
			mousePoint->Draw();
		}
		glDisable(GL_BLEND);
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
#endif	
		
		//mousevalues_t mousevalues = GetMouseValues();
		
		//if (mousevalues.leftButton.clickState == e_clickstate_down)
		//{
		//	hdPrintf("Left click down: X: %3.2f Y: %3.2f\n", 
		//			 mousevalues.leftButton.xClickDown, 
		//			 mousevalues.leftButton.yClickDown);
		//}
#endif
#endif
	}
	else
	{
		if (threadSleepingAllowed)
			usleep((frameDuration - m_updateSceneTimerValues.TimeSinceLastFrame) * sleepDuration);
	}
	return true;
}



bool hdApplication::HandleResize(const float newWidth, const float newHeight)
{
	if (controller == NULL) return false;
	controller->HandleResize(newWidth, newHeight);
	return true;
}