/*
 * Copyright (c) 2014 Hackdirt Ltd.
 * Author: David Petrie (david@davidpetrie.com)
 * 
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the 
 * use of this software. Permission is granted to anyone to use this software for
 * any purpose, including commercial applications, and to alter it and 
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim 
 * that you wrote the original software. If you use this software in a product, an 
 * acknowledgment in the product documentation would be appreciated but is not 
 * required.
 * 2. Altered source versions must be plainly marked as such, and must not be 
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef APP_CONTROLLER_H_
#define APP_CONTROLLER_H_


#include "hdk.h"

#include "GameController.h"


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