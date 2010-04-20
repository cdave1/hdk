/*
 *  GameController.h
 *  TotemGame
 *
 *  Created by david on 21/03/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

/*
 *
 * CONTROLLER FOR IN-GAME STUFF.
 *
 */


#ifndef GAME_CONTROLLER_H_
#define GAME_CONTROLLER_H_


#include "hdCommon.h"
#include "hdInterface.h"
#include "hdGameEngine.h"
#include "hdPolygon.h"
#include "AppCallbackFunctions.h"
#include "hdAnimationEngine.h"

#include "AppLevelController.h"
#include "hdSoundManager.h"




#define GAME_CONTROLLER_STATS_FONT "Fonts/collegiate9.f"

#define GAME_CONTROLLER_NOTE_FONT "Fonts/collegiate8.f"
#define GAME_CONTROLLER_PERCENT_FONT "Fonts/collegiate24.f" 
#define GAME_CONTROLLER_POINTS_FONT "Fonts/bebas12.f"
#define GAME_CONTROLLER_SMALL_FONT "Fonts/collegiate9.f"
#define GAME_CONTROLLER_MED_FONT "Fonts/bebas16.f"
#define GAME_CONTROLLER_LARGE_FONT "Fonts/bebas24.f"
#define kTotemBlockMax 512

#define ICON_POSITION_LEFT 0
#define ICON_POSITION_CENTER 1
#define ICON_POSITION_RIGHT 2


class GameController : public AppLevelController
{
public:	
	GameController(const hdInterfaceController* parentController);
	
	~GameController();
	
	//void Step(double interval);
	
	void Draw();
	
	void AnimateShow();
	
	void FinishedClicked();
	
	void SkipClicked();
	
	void QuitClicked();
	
	void RestartClicked();

	
protected:
	void Game_Step(double interval);
	
	void Game_ResetCurrentLevel();
	
	const bool Game_HandleTapUp(float x, float y, int tapCount);
	
	const bool Game_HandleTapDown(float x, float y, int tapCount);
	
	const bool Game_HandleSingleTap(float x, float y);
	
	const bool Game_HandleDoubleTap(float x, float y);
	
	const bool Game_HandleTapMovedSingle(const float previousX, const float previousY, 
										   const float currentX, const float currentY);
	
	const bool Game_HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
										   const float aCurrentX, const float aCurrentY,
										   const float bPreviousX, const float bPreviousY,	
										   const float bCurrentX, const float bCurrentY);
	
	void Game_ShowMessage();
	
	void Game_HideMessage();
	
	void Game_HandleOrientationChanged();
	
	/*
	 * Callback handlers
	 */
	static void btnShowMenuUpCallback(void *handler, void *sender);
	static void btnCloseMenuUpCallback(void *handler, void *sender);
	static void btnMenuRestartCallback(void *handler, void *sender);
	static void btnMenuQuitCallback(void *handler, void *sender);
	static void btnMenuSkipCallback(void *handler, void *sender);
	
	
	// User interface and menus
	hdButton* m_btnShowMenu;
	hdButton* m_btnFinishLevel;
	hdButton* m_btnFailRestart;
	hdButton *m_btnBG;
	
	hdButton* m_btnMenuRestart;
	hdCheckbox* m_btnMenuToggleMusic;
	hdCheckbox* m_btnMenuToggleSound;
	hdButton* m_btnMenuSkip;
	hdButton* m_btnMenuQuit;
	hdButton* m_btnMenuClose;
	
	
	
	static void ToggleMusicCallback(void *handler, void *sender);
	static void ToggleSoundCallback(void *handler, void *sender);
	
		
	hdOrthographicProjection* m_interfaceProjection;
	
		
private:
	friend class hdAnimation;
	
	void InitInterface();
	
	void InitAnimations();
	
	void InitSounds();
	
	void InitMusic();
	
	void InitLevelInterface();
	
	void RefreshLayout();
	
	void DisableMenu();
	
	void UpdateGameState();
	
	void UpdateGameInfo();
	
	
	/**********************************************************************/
	/* Animation Stuff */
	/**********************************************************************/ 
	hdAnimation* m_fadeAnim;
	
	hdAlphaAction* m_fadeAction;
	
	
	
	/**********************************************************************/
	/* Interface Layer */
	/**********************************************************************/ 
	hdInterfaceLayer* m_gameButtonLayer;
	
	hdUIContainer* m_menuButtonLayer;
	
	
	
	/**********************************************************************/
	/* Music */
	/**********************************************************************/ 
	hdSound *m_currBackgroundTrack;
	
	
	/**********************************************************************/
	/* Sound */
	/**********************************************************************/
	hdSound *m_btnMenuClickDownSound;
	hdSound *m_btnMenuClickUpSound;
	hdSound *m_btnMenuClickUpSoundDeep;
	
	
	/**********************************************************************/
	/* Fonts */
	/**********************************************************************/

	
	
	/**********************************************************************/
	/* Images */
	/**********************************************************************/

	
	
	/**********************************************************************/
	/* Misc */
	/**********************************************************************/
	
	
	/**********************************************************************/
	/* Textures */
	/**********************************************************************/


};


#endif