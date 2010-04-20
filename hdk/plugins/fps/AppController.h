/*
 *  AppController.h
 *  TotemGame
 *
 *  Created by david on 30/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef APP_CONTROLLER_H_
#define APP_CONTROLLER_H_


#include "hdCommon.h"
#include "hdInterface.h"
#include "hdGameEngine.h" 
#include "hdPlayerConfig.h"

#include "GameController.h"


//class TotemLevelController;
class GameController;

class AppController : public hdInterfaceController
{
public:	
	AppController();
	
	~AppController();
	
	void ShowGameController();
	
	void SaveGameState();
	
	void FreeIdleMemory();
	
	void LoadPlayerConfigFile();
	
	void TearDown();
	
	
	void Step(double interval);
	
	void Step(double sysInterval, double fixedInterval);
	
	void Draw();
	
	void AnimateShow() {}
	
	void HandleTapUp(float x, float y, int tapCount);	
	
	void HandleTapDown(float x, float y, int tapCount);
	
	void HandleSingleTap(float x, float y);
	
	void HandleDoubleTap(float x, float y);
	
	void HandleTapMovedSingle(const float previousX, const float previousY, 
							  const float currentX, const float currentY);
	
	void HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
							  const float aCurrentX, const float aCurrentY,
							  const float bPreviousX, const float bPreviousY,	
							  const float bCurrentX, const float bCurrentY);
	
	void HandleOrientationVectorUpdate(const float x, const float y, const float z);

	void HandleResize(const float newWidth, const float newHeight);
	
private:
	friend class hdAnimation;
	
	void FreeMemory();
	
	void EnsureGameController();
	
	GameController* gameController;
	
	hdInterfaceController* activeController;
};


#endif