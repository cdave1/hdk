/*
 *  TotemGameLevelLoader.cpp
 *  TotemGame
 *
 *  Created by david on 21/03/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */



#include "GameController.h"


 



GameController::GameController(const hdInterfaceController* parentController) : AppLevelController(parentController)
{
	m_fadeAction = new hdAlphaAction();
	m_fadeAction->SetDuration(0.8f);
	m_fadeAction->SetStartAlpha(0.0f);
	m_fadeAction->SetEndAlpha(1.0f);
	
	m_fadeAnim = hdAnimationController::CreateAnimation(this, false);
	m_fadeAnim->AddGameObject(m_fadeAction->GetAlphaObject());
	m_fadeAnim->AddAction(m_fadeAction);
	
	//ticksSinceLastUpdate = 0;
	//ticksInfoCycle = 0;
	//m_heavyCollisionCount = 0;
	
	//this->InitAnimations();
	this->InitSounds();
	//this->InitEffects();
	this->InitInterface();
	this->RefreshLayout();
}



GameController::~GameController()
{
	/**********************************************************************/
	/* Used in interface for animations */
	/**********************************************************************/
	delete m_btnShowMenu;
	
	delete m_btnFinishLevel;
	
	delete m_btnFailRestart;
	
	delete m_btnBG;
	
	delete m_btnMenuRestart;
	
	delete m_btnMenuQuit;
	
	delete m_btnMenuSkip;
	
	delete	m_btnMenuToggleMusic;
	
	delete m_btnMenuToggleSound;
	
	delete m_btnMenuClose;
		
	delete m_gameButtonLayer;
	
	delete m_menuButtonLayer;
	
	
	delete m_interfaceProjection;
	
	
	
	/**********************************************************************/
	/* Goal */
	/**********************************************************************/

	
	
	/**********************************************************************/
	/* Fonts */
	/**********************************************************************/

	
	
	/**********************************************************************/
	/* Images */
	/**********************************************************************/

	
	
	
	
	
	

	
	
	hdAnimationController::Instance()->StopAnimations(this);
	hdAnimationController::Instance()->PurgeAnimations(this);
	
}


void GameController::InitInterface()
{
	hdAABB interfaceAABB;
	interfaceAABB.lower = hdVec3(0,0,100.0f);
	interfaceAABB.upper = hdVec3(m_PixelScreenWidth, m_PixelScreenHeight, -100.0f);
	m_interfaceProjection = new hdOrthographicProjection(NULL, interfaceAABB);
	
	m_gameButtonLayer = new hdInterfaceLayer(m_gameWorld, interfaceAABB);
	m_btnShowMenu = new hdButton("/Interface/btnMenuNormal.png", 
								 "/Interface/btnMenuOver.png", 
								 "/Interface/btnMenuNormal.png", 
								 NULL);
	
	m_btnFinishLevel = new hdButton("/Interface/btnFinishLevelNormal.png", 
									"/Interface/btnFinishLevelOver.png", 
									"/Interface/btnFinishLevelOver.png", 
									NULL);
	m_btnFinishLevel->Hide();
	
	m_btnFailRestart = new hdButton("/Interface/btnFailRestartNormal.png", 
									"/Interface/btnFailRestartOver.png", 
									"/Interface/btnFailRestartOver.png", 
									NULL);
	m_btnFailRestart->Hide();
	
	m_gameButtonLayer->AddButton(m_btnShowMenu);
	m_gameButtonLayer->AddButton(m_btnFinishLevel);
	m_gameButtonLayer->AddButton(m_btnFailRestart);

	
	

	
	m_menuButtonLayer = new hdUIContainer(NULL, interfaceAABB.lower, interfaceAABB.upper);
	m_btnBG = new hdButton("/Materials/s_glow_15.tga", 
						   "/Materials/s_glow_15.tga", 
						   "/Materials/s_glow_15.tga", 
						   NULL);
	m_btnBG->Disable();
	m_btnMenuSkip = new hdButton("/Interface/btnSkipLevelNormal.png", 
								 "/Interface/btnSkipLevelOver.png", 
								 "/Interface/btnSkipLevelNormal.png", 
								 NULL);
	
	m_btnMenuToggleMusic = new hdCheckbox("/Interface/btnMusicOnNormal.png", 
										  "/Interface/btnMusicOnOver.png",
										  "/Interface/btnMusicOffNormal.png", 
										  "/Interface/btnMusicOffOver.png",
										  NULL);
	
	
	m_btnMenuToggleSound = new hdCheckbox("/Interface/btnSoundOnNormal.png", 
										  "/Interface/btnSoundOnOver.png",
										  "/Interface/btnSoundOffNormal.png", 
										  "/Interface/btnSoundOffOver.png",
										  NULL);
	
	
	m_btnMenuRestart = new hdButton("/Interface/btnRestartLevelNormal.png", 
									"/Interface/btnRestartLevelOver.png", 
									"/Interface/btnRestartLevelNormal.png", 
									NULL);
	
	m_btnMenuQuit = new hdButton("/Interface/btnQuitLevelNormal.png", 
								 "/Interface/btnQuitLevelOver.png", 
								 "/Interface/btnQuitLevelNormal.png", 
								 NULL);
	
	m_btnMenuClose = new hdButton("/Interface/btnCloseNormal.png", 
								  "/Interface/btnCloseOver.png", 
								  "/Interface/btnCloseNormal.png", 
								  NULL);
	
	m_btnMenuToggleMusic->AddValueChangedListener(this, GameController::ToggleMusicCallback);
	m_btnMenuToggleSound->AddValueChangedListener(this, GameController::ToggleSoundCallback);
	
	if (hdPlayerConfig::GetValue("PlayMusic") != "On")
	{
		m_btnMenuToggleMusic->Toggle();
	}
	
	if (hdPlayerConfig::GetValue("PlaySounds") != "On")
	{
		m_btnMenuToggleSound->Toggle();
	}
	
	m_menuButtonLayer->Add(m_btnBG);
	m_menuButtonLayer->Add(m_btnMenuRestart);
	m_menuButtonLayer->Add(m_btnMenuQuit);
	m_menuButtonLayer->Add(m_btnMenuToggleMusic);
	m_menuButtonLayer->Add(m_btnMenuToggleSound);
	m_menuButtonLayer->Add(m_btnMenuSkip);
	m_menuButtonLayer->Add(m_btnMenuClose);
	
	m_menuButtonLayer->Hide(); 
	


	
	
	
	
	
	/*
	 * Callbacks
	 */
	m_btnFinishLevel->SetMouseDownListener((AppInterface *)this, AppInterface::GenericButtonDownCallback);
	m_btnFailRestart->SetMouseDownListener((AppInterface *)this, AppInterface::GenericButtonDownCallback);
	m_btnShowMenu->SetMouseDownListener((AppInterface *)this, AppInterface::GenericButtonDownCallback);
	
	m_btnShowMenu->SetMouseUpListener(this, GameController::btnShowMenuUpCallback);
	m_btnMenuClose->SetMouseUpListener(this, GameController::btnCloseMenuUpCallback);
	m_btnMenuRestart->SetMouseUpListener(this, GameController::btnMenuRestartCallback);
	m_btnMenuQuit->SetMouseUpListener(this, GameController::btnMenuQuitCallback);
	m_btnMenuSkip->SetMouseUpListener(this, GameController::btnMenuSkipCallback);
	
	DisableMenu();
}
 


void GameController::InitSounds()
{
	
	m_btnMenuClickDownSound = hdSoundManager::FindSound("Sounds/btnClickDown.caf", e_soundTypeNormal);
	m_btnMenuClickUpSound = hdSoundManager::FindSound("Sounds/btnClick01.caf", e_soundTypeNormal);
	m_btnMenuClickUpSoundDeep = hdSoundManager::FindSound("Sounds/btnClickBack01.caf", e_soundTypeNormal);
	
}


void GameController::Game_HandleOrientationChanged()
{
	this->RefreshLayout();
}


void GameController::RefreshLayout()
{
	hdAABB interfaceAABB;
	
	if (m_interfaceProjection != NULL)
	{
		delete m_interfaceProjection;
		m_interfaceProjection = NULL;
	}
	interfaceAABB.lower = hdVec3(0,0,100.0f);
	interfaceAABB.upper = hdVec3(m_PixelScreenWidth, m_PixelScreenHeight, -100.0f);
	m_interfaceProjection = new hdOrthographicProjection(NULL, interfaceAABB);
	
	/*
	 * Positioning.
	 */
	m_btnShowMenu->SetAs2DBox(2.0f, m_PixelScreenHeight - 40.0f, 50.0f, 40.0f);
	m_btnFinishLevel->SetAs2DBox((m_PixelScreenWidth - 200.0f)/2.0f, 0.0f, 200.0f, 80.0f);
	m_btnFailRestart->SetAs2DBox((m_PixelScreenWidth - 200.0f)/2.0f, 0.0f, 200.0f, 80.0f);
	
	
	
	if (m_orientation == e_hdInterfaceOrientationLandscape)
	{
		m_btnBG->SetAs2DBox((m_PixelScreenWidth - m_PixelScreenHeight)/2.0f, 0.0f, m_PixelScreenHeight, m_PixelScreenHeight);
		
		m_btnMenuClose->SetAs2DBox((m_PixelScreenWidth - 160.0f)/2.0f, 275.0f, 50.0f, 35.0f);
		
		m_btnMenuToggleMusic->SetAs2DBox((m_PixelScreenWidth - 160.0f)/2.0f, 210.0f, 160.0f, 55.0f);
		m_btnMenuToggleSound->SetAs2DBox((m_PixelScreenWidth - 160.0f)/2.0f, 145.0f, 160.0f, 55.0f);
		
		m_btnMenuRestart->SetAs2DBox((m_PixelScreenWidth - 160.0f)/2.0f, 80.0f, 160.0f, 55.0f);
		m_btnMenuQuit->SetAs2DBox((m_PixelScreenWidth - 160.0f)/2.0f, 15.0f, 160.0f, 55.0f);
		
#ifdef DEBUG
		m_btnMenuSkip->SetAs2DBox((m_PixelScreenWidth - 200.0f)/2.0f + 50.0f, 260.0f, 100.0f, 35.0f);
#else
		m_btnMenuSkip->Hide();
#endif
	}
	else
	{
		m_btnBG->SetAs2DBox(0.0f, (m_PixelScreenHeight - m_PixelScreenWidth)/2.0f, m_PixelScreenHeight, m_PixelScreenHeight);
		
		m_btnMenuClose->SetAs2DBox((m_PixelScreenWidth - 160.0f)/2.0f, 345.0f, 50.0f, 35.0f);
		
		m_btnMenuToggleMusic->SetAs2DBox((m_PixelScreenWidth - 160.0f)/2.0f, 290.0f, 160.0f, 55.0f);
		m_btnMenuToggleSound->SetAs2DBox((m_PixelScreenWidth - 160.0f)/2.0f , 225.0f, 160.0f, 55.0f);
		
		m_btnMenuRestart->SetAs2DBox((m_PixelScreenWidth - 160.0f)/2.0f, 165.0f, 160.0f, 55.0f);
		m_btnMenuQuit->SetAs2DBox((m_PixelScreenWidth - 160.0f)/2.0f, 100.0f, 160.0f, 55.0f);
		
		
#ifdef DEBUG
		m_btnMenuSkip->SetAs2DBox((m_PixelScreenWidth - 200.0f)/2.0f + 50.0f, 340.0f, 100.0f, 35.0f);
#else
		m_btnMenuSkip->Hide();
#endif
	}
}



void GameController::InitMusic()
{

}


void GameController::Game_ResetCurrentLevel() 
{
	//m_notificationPolygons->RemoveAll();
}


void GameController::Game_Step(double interval) 
{
	/*
	m_heavyCollisionCount = 0;
	
	// Auto evaluate the game state every 30 ticks.
	if (ticksSinceLastUpdate == 20)
	{
		ticksSinceLastUpdate = 0;
		//UpdateGameState();
		//ShowWinMessage();
	}
	else
	{
		++ticksSinceLastUpdate;
	}
	
	//UpdateGameInfo();
	 */
}


void GameController::Draw()
{	
	DrawInternal();
	
	
	
	hdglBindTexture(NULL);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	if (m_orientation == e_hdInterfaceOrientationLandscape)
	{
		glRotatef(m_landscapeRotationZValue, 0.0f, 0.0f, 1.0f);
	}
	glPushMatrix();
	hdglOrthof(m_interfaceProjection->GetAABB().lower.x, 
			 m_interfaceProjection->GetAABB().upper.x, 
			 m_interfaceProjection->GetAABB().lower.y, 
			 m_interfaceProjection->GetAABB().upper.y, 
			 m_interfaceProjection->GetAABB().lower.z, 
			 m_interfaceProjection->GetAABB().upper.z);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);
	if (m_menuButtonLayer->IsHidden())
	{
		m_gameButtonLayer->Draw();
		
		DrawInternalInterface();

		//uiImageContainer->Draw();
	} 
	else
	{
		m_menuButtonLayer->Draw();
	}
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	
	if (m_menuButtonLayer->IsHidden())
	{
		
		//m_animPolygons->Map(&hdReceiver::Draw);
		//m_notificationPolygons->Map(&totemPolygon::Draw);
	}
	glDisable(GL_BLEND);
	
	hdglBindTexture(NULL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
	if (m_fadeAnim->GetStatus() != e_animationFinished)
	{
		hdglBegin(GL_QUADS);
		hdglColor4f(0.0f, 0.0f, 0.0f, m_fadeAction->GetAlphaObject()->GetAlpha());
		hdglVertex2f(m_interfaceProjection->GetAABB().lower.x, m_interfaceProjection->GetAABB().lower.y);
		hdglVertex2f(m_interfaceProjection->GetAABB().lower.x, m_interfaceProjection->GetAABB().upper.y);
		hdglVertex2f(m_interfaceProjection->GetAABB().upper.x, m_interfaceProjection->GetAABB().upper.y);
		hdglVertex2f(m_interfaceProjection->GetAABB().upper.x, m_interfaceProjection->GetAABB().lower.y);
		hdglEnd();
	}
	glDisable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}


void GameController::AnimateShow()
{
	m_fadeAction->SetStartAlpha(1.0f);
	m_fadeAction->SetEndAlpha(0.0f);
	m_fadeAnim->StartAnimation();
}



const bool GameController::Game_HandleTapUp(float x, float y, int tapCount)
{	
	return false;
}



const bool GameController::Game_HandleTapDown(float x, float y, int tapCount)
{
	return false;
}


const bool GameController::Game_HandleSingleTap(float x, float y) 
{ 
	
	return true; 
}


const bool GameController::Game_HandleDoubleTap(float x, float y) 
{
	return false;
}


const bool GameController::Game_HandleTapMovedSingle(const float previousX, const float previousY, 
											   const float currentX, const float currentY)
{
	return false;
}



const bool GameController::Game_HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
						  const float aCurrentX, const float aCurrentY,
						  const float bPreviousX, const float bPreviousY,	
														  const float bCurrentX, const float bCurrentY) { return false; }


void GameController::btnShowMenuUpCallback(void *handler, void *sender)
{
	GameController *self = (GameController *)handler;
	if (!self) return;
	
	hdSoundManager::PlaySound(self->m_btnMenuClickUpSoundDeep);
	
	self->m_interactionState = e_interactionDisabled;
	self->m_gameButtonLayer->Hide();
	self->m_menuButtonLayer->Show();
	
	self->m_btnMenuClose->Enable();
	self->m_btnMenuRestart->Enable();
	self->m_btnMenuQuit->Enable();
	self->m_btnMenuSkip->Enable();
}


void GameController::btnCloseMenuUpCallback(void *handler, void *sender)
{
	GameController *self = (GameController *)handler;
	if (!self) return;
	
	hdSoundManager::PlaySound(self->m_btnMenuClickUpSoundDeep);
	
	// if the player has not yet started, then this cannot happen.
	//if (self->gameInfo.gameStarted)
	//{
		self->m_interactionState = e_waitingForInput;
	//}
	self->m_gameButtonLayer->Show();
	self->m_menuButtonLayer->Hide();
	
	self->DisableMenu();
}


void GameController::btnMenuRestartCallback(void *handler, void *sender)
{
	GameController *self = (GameController *)handler;
	if (!self) return;
	self->RestartClicked();
}
	

void GameController::btnMenuQuitCallback(void *handler, void *sender)
{
	GameController *self = (GameController *)handler;
	if (!self) return;
	self->QuitClicked();
}


void GameController::btnMenuSkipCallback(void *handler, void *sender)
{
	GameController *self = (GameController *)handler;
	if (!self) return;
	self->SkipClicked();
}





void GameController::Game_ShowMessage()
{
	m_interactionState == e_interactionDisabled;
}


void GameController::Game_HideMessage()
{
	m_interactionState = e_waitingForInput;
}


void GameController::UpdateGameState()
{
	
}


void GameController::UpdateGameInfo()
{
	/*
	if (ticksInfoCycle == 0)
	{

		currGoalInfoCycle = (currGoalInfoCycle + 1) % m_goalCount;
	}
	else if (ticksInfoCycle == 63)
	{
		m_fontStats->SetTextFormatted("POINTS: %d\n%s", 
				 gameInfo.gamePoints,
				 m_goals[currGoalInfoCycle].progressMessage);
		currGoalInfoCycle = (currGoalInfoCycle + 1) % m_goalCount;
	}
	
	ticksInfoCycle = (ticksInfoCycle + 1) & 127;
	 */
}



void GameController::ToggleMusicCallback(void *handler, void *sender)
{
	GameController *self = (GameController *)handler;
	
	if (self->m_btnMenuToggleMusic->IsOn())
	{
		hdSoundManager::SetMusicMaxVolume(1.0f);
		hdSoundManager::PlaySound(self->m_currBackgroundTrack);
		hdPlayerConfig::SetValue("PlayMusic", "On");
	}
	else
	{
		hdSoundManager::SetMusicMaxVolume(0.0f);
		hdSoundManager::StopSound(self->m_currBackgroundTrack);
		hdPlayerConfig::SetValue("PlayMusic", "Off");
	}
}


void GameController::ToggleSoundCallback(void *handler, void *sender)
{
	GameController *self = (GameController *)handler;
	
	if (self->m_btnMenuToggleSound->IsOn())
	{
		hdSoundManager::SetSoundMaxVolume(1.0f);
		hdPlayerConfig::SetValue("PlaySounds", "On");
	}
	else
	{
		hdSoundManager::SetSoundMaxVolume(0.0f);
		hdPlayerConfig::SetValue("PlaySounds", "Off");
	}
}





void GameController::FinishedClicked()
{
	SkipClicked();
}


void GameController::SkipClicked()
{
	DisableMenu();
	
	hdSoundManager::PlaySound(m_btnMenuClickUpSound);
	hdSoundManager::StopSound(m_currBackgroundTrack);
	
	// anim fade
	//m_fadeAction->SetStartAlpha(0.0f);
	//m_fadeAction->SetEndAlpha(1.0f);
	//m_fadeAnim->SetFinishedCallback(m_parentController, AppCallbackFunctions::StoppedCurrentLevelCallback);
	//m_fadeAnim->StartAnimation();
	
	m_interactionState = e_interactionDisabled;
}


void GameController::QuitClicked()
{
	DisableMenu();
	
	hdSoundManager::PlaySound(m_btnMenuClickUpSound);
	m_interactionState = e_interactionDisabled;
	
	hdSoundManager::StopSound(m_currBackgroundTrack);
	//m_fadeAction->SetStartAlpha(0.0f);
	//m_fadeAction->SetEndAlpha(1.0f);
	//m_fadeAnim->SetFinishedCallback(m_parentController, AppCallbackFunctions::QuitCurrentLevelCallback);
	//m_fadeAnim->StartAnimation();
	
	m_interactionState = e_interactionDisabled;
}


void GameController::RestartClicked()
{
	DisableMenu();
	
	hdSoundManager::PlaySound(m_btnMenuClickUpSound);
	m_interactionState = e_interactionDisabled;
	//hdSoundManager::StopSound(m_currBackgroundTrack);
	//m_fadeAction->SetStartAlpha(0.0f);
	//m_fadeAction->SetEndAlpha(1.0f);
	//m_fadeAnim->SetFinishedCallback(m_parentController, AppCallbackFunctions::RestartCurrentLevelCallback);
	//m_fadeAnim->StartAnimation();
	
	m_interactionState = e_interactionDisabled;
}


void GameController::DisableMenu()
{
	m_btnMenuClose->Disable();
	m_btnMenuSkip->Disable();
	m_btnMenuRestart->Disable();
	m_btnMenuQuit->Disable();
}
