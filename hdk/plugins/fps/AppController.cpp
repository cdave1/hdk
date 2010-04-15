/*
 *  AppController.cpp
 *  TotemGame
 *
 *  Created by david on 30/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "AppController.h"


AppController::AppController() : hdInterfaceController()
{
	// Ensure we have a game dir
	hdAssert(strlen(FileSystem_BaseDir()) > 0);
	
	// Ensure config has been set up
	hdAssert(hdConfig::ConfigWasInitialized());
	
	// Load user config
	LoadPlayerConfigFile();
	
	EnsureGameController();
	
	activeController = gameController;
	gameController->AnimateShow();
}


AppController::~AppController()
{
	hdPrintf("######## Delete totem controller\n");
		
	if (gameController != NULL)
	{
		hdPrintf("\t*** Delete game controller\n");
		delete gameController;
		gameController = NULL;
	}
		
	Scripts_TearDown();
	
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


/*
void AppController::IntroFinished()
{
	delete introController;
	introController = NULL;
	
	EnsureMenuController();
	
	activeController = menuController;
	activeController->AnimateShow();
	//levelController->SetCurrentLevel((totemLevel *)m_totemWorld->GetLevels()[0]);
}*/


void AppController::EnsureGameController()
{
	if (gameController == NULL)
	{
		gameController = new GameController(this);
	}
	hdAssert(gameController != NULL);
}


/*
void AppController::GameControllerStopCurrentLevel()
{
	
	hdAssert(gameController != NULL);
	hdAssert(gameController->GetCurrentLevel() != NULL);
	hdAssert(m_levelPickerWorld != NULL);
	
	FreeMemory();
	
	// Update the game stats from the recently played level
	UpdateGameStats(m_levelPickerWorld, gameController->GetCurrentLevel(), gameController->gameInfo);
	SaveGameState();
	
	// Show the level picker controller.
	EnsureLevelPickerController();
	InitLevelPickerWorld();
	levelPickerController->PanToNewLevelBlock();
	levelPickerController->ShowNewLevelMessage(gameController->GetLevelStats());
	activeController = levelPickerController;
	activeController->AnimateShow();
	
	// Free up the game controller resources
	delete gameController;
	gameController = NULL;
	
	DispatchMessages();
	
} */


/*
void AppController::GameControllerRestartCurrentLevel()
{
	
	// Complete rebuild the game controller
	// No need to free asset memory though - every texture/sound/font
	// should still be in memory. Recreating will just get handles to these.
	hdAssert(gameController != NULL);
	hdAssert(gameController->GetCurrentLevel() != NULL);
	hdAssert(m_levelPickerWorld != NULL);
	
	UpdateGameStats(m_levelPickerWorld, gameController->GetCurrentLevel(), gameController->gameInfo);
	
	delete gameController;
	gameController = NULL;
	activeController = NULL;
	
	InitLevelPickerWorld();
	m_selectedLevelPickerLevel = m_levelPickerWorld->GetLevels()[m_levelId];
	
	EnsureGameController();
	gameController->InitLevel(m_selectedLevelPickerLevel, m_levelId);
	gameController->SetLevelStats(GetStatsForLevel(m_levelPickerWorld, m_selectedLevelPickerLevel));
	activeController = gameController;
	gameController->AnimateShow();

	DispatchMessages();	
} */


/*
void AppController::GameControllerQuitCurrentLevel()
{
	hdAssert(gameController != NULL);
	hdAssert(gameController->GetCurrentLevel() != NULL);
	hdAssert(m_levelPickerWorld != NULL);
	
	FreeMemory();
	
	// Show the level picker controller.
	EnsureLevelPickerController();
	InitLevelPickerWorld();
	levelPickerController->PanToNewLevelBlock();
	activeController = levelPickerController;
	activeController->AnimateShow();
	
	// Free up the game controller resources
	delete gameController;
	gameController = NULL;
	
	DispatchMessages();
}*/


/*
void AppController::SetLoading()
{
	loadingController->SetOrientation(e_hdInterfaceOrientationLandscape); // activeController->GetOrientation());
	activeController = loadingController;
	loadingController->Reset();
	loadingController->AnimateShow();
	//usleep(50);
}
 */


void AppController::SaveGameState()
{
	// Save user config settings
	if (!hdPlayerConfig::SaveConfigFile())
	{
		hdError(-1, "Could not save player config settings.");
	}
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
	//if (menuController != NULL)
	{
		hdPrintf("\t*** Delete menu controller\n");
		//delete menuController;
		//menuController = NULL;
	}
	
	/* DANGER!
	 *
	 * Clearing the cache will create actually cause worse performance - try to figure out why...
	 */
	//totemWorldManager::Instance()->ClearCache();
	
	SaveGameState();
	
	/*
	 // To save memory on the iphone, delete all levels - they will be 
	 // rebuilt from the saved binary data when the player returns to
	 // to level picker screen.
	 int i = 0;
	 while (m_levelPickerWorld->GetLevelCount() > 1)
	 {
	 if (m_levelPickerWorld->GetLevels()[i] != levelPickerController->GetCurrentLevel())
	 {
	 m_levelPickerWorld->RemoveLevelAtIndex(i);
	 }
	 else
	 {
	 ++i;
	 }
	 }*/
}


void AppController::TearDown()
{	

	
}


void AppController::FreeMemory()
{
	hdFontManager::TearDown();
	hdTextureManager::TearDown();
	hdSoundManager::DestroySounds();
//	
	//hdSoundManager::DestroySounds();
	//hdSoundManager::TearDown();
	//hdSoundManager::InitSoundManager();
}


void AppController::Step(double interval)
{
	//hdAnimationController::Instance()->StepWithInterval(interval);
	hdAnimationController::Instance()->StepWithInterval(interval);
	activeController->Step(interval); //interval);
}


void AppController::Step(double sysInterval, double fixedInterval)
{
#if (TARGET_IPHONE_SIMULATOR == 1) || (TARGET_OS_IPHONE == 1)
	hdAnimationController::Instance()->StepWithInterval(hdClamp(sysInterval, 0.016, 0.04)); //0.032);// fixedInterval);
#else
	hdAnimationController::Instance()->StepWithInterval(hdMin(fixedInterval, 0.04));// fixedInterval);
#endif
	activeController->Step(sysInterval);
}



void AppController::Draw()
{
	//hdInitImmediateModeGL();
	hdAssert(activeController != NULL);
	activeController->Draw();
}


void AppController::HandleSingleTap(float x, float y)
{
	hdInterfaceContext::HandleSingleTap(x, y);
}


void AppController::HandleDoubleTap(float x, float y)
{
	hdInterfaceContext::HandleDoubleTap(x, y);
}


void AppController::HandleTapUp(float x, float y, int tapCount)
{
	hdInterfaceContext::HandleTapUp(x, y, tapCount);
}


void AppController::HandleTapDown(float x, float y, int tapCount)
{
	hdInterfaceContext::HandleTapDown(x, y, tapCount);
}


void AppController::HandleTapMovedSingle(const float previousX, const float previousY, 
										   const float currentX, const float currentY)
{
	hdInterfaceContext::HandleTapMovedSingle(previousX, previousY, currentX, currentY);
}


void AppController::HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
										   const float aCurrentX, const float aCurrentY,
										   const float bPreviousX, const float bPreviousY,	
										   const float bCurrentX, const float bCurrentY)
{
	hdInterfaceContext::HandleTapMovedDouble(aPreviousX, aPreviousY, aCurrentX, aCurrentY, 
										   bPreviousX, bPreviousY, bCurrentX, bCurrentY);
}


/*
 * Switch between landscape and portrait
 *
 * A bit hacky - these numbers worked best in practise.
 */
void AppController::HandleOrientationVectorUpdate(const float x, const float y, const float z)
{
	if (activeController == NULL) return;
	
	if (activeController != gameController) return;
	
	if (fabs(x) > 0.4f && fabs(y) < 0.3f && 
		activeController->GetOrientation() != e_hdInterfaceOrientationLandscape)
	{
		activeController->SetOrientation(e_hdInterfaceOrientationLandscape);
	}
	else if (fabs(y) > 0.4f && fabs(x) < 0.3f && 
			 activeController->GetOrientation() != e_hdInterfaceOrientationPortrait)
	{
		activeController->SetOrientation(e_hdInterfaceOrientationPortrait);
	}
}


void AppController::HandleResize(const float newWidth, const float newHeight)
{
	if (activeController == NULL) return;
	
	activeController->HandleResize(newWidth, newHeight);
}