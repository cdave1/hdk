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

#ifndef APP_CALLBACK_FUNCTIONS_H_
#define APP_CALLBACK_FUNCTIONS_H_

//#include <boost/thread.hpp>

#include "hdk.h"
#include "AppController.h"



class AppCallbackFunctions
{
public:
	//friend class TotemGameController;
	
	static void ShowLevelControllerCallback(void * object, hdAnimation * anim);
	
	static void IntroFinishedCallback(void * object, hdAnimation * anim);
	
	
	
	static void LevelMenuFinishedCallback(void * object, hdAnimation * anim);
	
//	static void HandleLevelClickCallback(void * object, hdAnimation * anim);
	
	static void StoppedCurrentLevelCallback(void * object, hdAnimation * anim);
	
	static void RestartCurrentLevelCallback(void * object, hdAnimation * anim);
	
	static void ShowLevelPickerControllerCallback(void * object, hdAnimation * anim);
	
	static void LevelPickerLevelClickedCallback(void *object, hdAnimation *anim);
	
	static void LevelPickerBackCallback(void *object, hdAnimation *anim);
	
	static void QuitCurrentLevelCallback(void *object, hdAnimation *anim);
	
protected:
	friend class hdAnimation;
	
	static void IntroFinished_Thread(void * object, hdAnimation * anim);
	
	static void StoppedCurrentLevel_Thread(void * object, hdAnimation * anim);
	
	static void ShowLevelPickerController_Thread(void * object, hdAnimation * anim);
	
	static void LevelPickerLevelClicked_Thread(void *object, hdAnimation *anim);
	
	static void LevelPickerBack_Thread(void *object, hdAnimation *anim);
	
	static void QuitCurrentLevel_Thread(void *object, hdAnimation *anim);
	
	
	static void DoLoadingScreenTransition(void (*func)(void *, hdAnimation *), void * object);
};



#endif