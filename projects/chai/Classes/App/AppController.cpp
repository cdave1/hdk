/*
 *  AppController.cpp
 *  TotemGame
 *
 *  Created by david on 30/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "AppController.h"
using namespace chaiscript;

static char *script;

static char *buttons;

static hdUIContainer *m_interfaceItems; 

static ChaiScript chai;

AppController::AppController() : hdInterfaceController()
{	
	// Ensure we have a game dir
	hdAssert(strlen(FileSystem_BaseDir()) > 0);
	
	// Ensure config has been set up
	hdAssert(hdConfig::ConfigWasInitialized());
	
	// Load user config
	LoadPlayerConfigFile();
	
	
	m_interfaceItems = new hdUIContainer(NULL, hdVec3(0,0,0), hdVec3(480, 320,0));
	
	// Goal (using chai):
	// - (DONE) chai eval
	//
	// - (DONE) chai eval_file
	//
	// - expose hdButton classes to chai.
	// - load buttons in a chai script.
	// - draw on screen.
	
	//LoadChaiScript(&script, "Scripts/while.chai");
	LoadChaiScript(&buttons, "Scripts/buttons.chai");
	
		
	chai.add(user_type<hdButton>(), "hdButton");
	chai.add(bootstrap::basic_constructors<hdButton>("hdButton"));
	chai.add(constructor<hdButton (string, string, string)>(), "hdButton");
	
	
	chai.add(fun(boost::function<void (hdButton*)>(&AddInterfaceElement)), "AddInterfaceElement");
	
	chai.add(fun(boost::function<int (hdButton*)>(&hdButton::GetVertexCount)), "GetVertexCount");
	chai.add(fun(boost::function<void (hdButton*, int, int, int, int)>(&hdButton::SetDimensions)), "SetDimensions");
	chai.add(fun(boost::function<bool (hdButton*, double, double)>(&hdButton::MouseDown)), "MouseDown");
	
	chai.eval(buttons);
}


void AppController::AddInterfaceElement(hdButton *button)
{
	m_interfaceItems->Add(button);
}


AppController::~AppController()
{
	free(script);
	free(buttons);
	
	hdPrintf("\t*** Sound Manager: tear down\n");
	hdSoundManager::TearDown();
	
	hdPrintf("\t*** Font Manager: tear down\n");
	hdFontManager::TearDown();
	
	hdPrintf("\t*** Texture Manager: tear down\n");
	hdTextureManager::TearDown();
	
	hdPrintf("\t*** Animation Controller: tear down\n");
	hdAnimationController::TearDown();
	
	hdPrintf("\t*** Totem File System: tear down\n");	
	FileSystem_Teardown();
}


void AppController::LoadChaiScript(char **in, char *path)
{
	filehandle_t *hnd;
	int len;
	
	if (NULL == (hnd = FileSystem_OpenFile(path, 0)))
    {
		hdPrintf("failed to load");
		return;
    }
	
	len = FileSystem_GetFileSize(hnd);
	if (NULL == (*in = (char *)malloc(sizeof(char) * len)))
	{
		hdPrintf("MEM");
		hdAssert(false);
	}
	
	FileSystem_ReadFile(*in, len, 1, hnd);
	FileSystem_CloseFile(hnd);
}


void AppController::LoadPlayerConfigFile()
{
	if (!hdPlayerConfig::LoadConfigFile())
	{
		// Set some defaults
		hdPlayerConfig::SetValue("PlayMusic", "On");
		hdPlayerConfig::SetValue("PlaySounds", "On");
	}
	
	if (hdPlayerConfig::GetValue("PlayMusic") == "On")
	{
		hdSoundManager::SetMusicMaxVolume(1.0f);
	}
	else
	{
		hdSoundManager::SetMusicMaxVolume(0.0f);
	}
	
	if (hdPlayerConfig::GetValue("PlaySounds") == "On")
	{
		hdSoundManager::SetSoundMaxVolume(1.0f);
	}
	else
	{
		hdSoundManager::SetSoundMaxVolume(0.0f);
	}
	
}


void AppController::FreeIdleMemory()
{
	
}


void AppController::TearDown()
{	

	
}


bool AppController::SaveGameState() {}


void AppController::FreeMemory()
{
	hdFontManager::TearDown();
	hdTextureManager::TearDown();
	hdSoundManager::DestroySounds();
}


void AppController::Step(double sysInterval, double fixedInterval)
{
#if (TARGET_IPHONE_SIMULATOR == 1) || (TARGET_OS_IPHONE == 1)
	//hdAnimationController::Instance()->StepWithInterval(hdClamp(sysInterval, 0.016, 0.04)); //0.032);// fixedInterval);
#else
	//hdAnimationController::Instance()->StepWithInterval(hdMin(fixedInterval, 0.04));// fixedInterval);
#endif
}



void AppController::Draw()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glPushMatrix();
	hdglOrthof(0, 480, 0, 320, 0, -100);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	if (m_interfaceItems)
		m_interfaceItems->Draw();
	
	//glDisable(GL_BLEND);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}


void AppController::HandleSingleTap(float x, float y)
{
	
	//hdInterfaceContext::HandleSingleTap(x, y);
}


void AppController::HandleDoubleTap(float x, float y)
{
	//hdInterfaceContext::HandleDoubleTap(x, y);
}


void AppController::HandleTapUp(float x, float y, int tapCount)
{
	//hdInterfaceContext::HandleTapUp(x, y, tapCount);
}


void AppController::HandleTapDown(float x, float y, int tapCount)
{
	//hdInterfaceContext::HandleTapDown(x, y, tapCount);
}


void AppController::HandleTapMovedSingle(const float previousX, const float previousY, 
										   const float currentX, const float currentY)
{
	//hdInterfaceContext::HandleTapMovedSingle(previousX, previousY, currentX, currentY);
}


void AppController::HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
										   const float aCurrentX, const float aCurrentY,
										   const float bPreviousX, const float bPreviousY,	
										   const float bCurrentX, const float bCurrentY)
{
	//hdInterfaceContext::HandleTapMovedDouble(aPreviousX, aPreviousY, aCurrentX, aCurrentY, 
	//									   bPreviousX, bPreviousY, bCurrentX, bCurrentY);
}


/*
 * Switch between landscape and portrait
 *
 * A bit hacky - these numbers worked best in practise.
 */
void AppController::HandleOrientationVectorUpdate(const float x, const float y, const float z)
{
	
}


void AppController::HandleResize(const float newWidth, const float newHeight)
{

}