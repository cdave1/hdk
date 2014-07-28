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

#include "AppCallbackFunctions.h"

/*
 * Static callback functions
 */

static AppController *currentController = NULL;

void AppCallbackFunctions::DoLoadingScreenTransition(void (*func)(void *, hdAnimation *), void * object)
{
	AppController *self = (AppController *)object;
	//self->SetLoading();
	
	hdPauseAction *pause = new hdPauseAction();
	pause->SetDuration(0.05f);
	
	hdAnimation *pauseAnim = hdAnimationController::CreateAnimation(self);
	pauseAnim->AddAction(pause);
	pauseAnim->SetFinishedCallback(self, func);
	pauseAnim->StartAnimation();	
}


/*
void AppCallbackFunctions::IntroFinishedCallback(void * object, hdAnimation * anim)
{
#if 1
	anim->RemoveCallbackFunctions();
	AppController *self = (AppController *)object;
	DoLoadingScreenTransition(AppCallbackFunctions::IntroFinished_Thread, self);

#else
	currentController = (AppController *)object;
	currentController->SetLoading();
	boost::thread thrd(boost::ref(AppCallbackFunctions::IntroFinished_Thread), boost::ref(currentController));
    thrd.join();
#endif
}*/


void AppCallbackFunctions::IntroFinished_Thread(void * object, hdAnimation * anim)
{
#if 1
	anim->RemoveCallbackFunctions();
	AppController *self = (AppController *)object;
	//self->IntroFinished();
#else
	//void *threadTicket = OSFunctions_PrepareThreadResources();
	//AppController *self = (AppController *)object;
	//self->IntroFinished();
	//OSFunctions_TearDownThreadResources(threadTicket);
#endif
}


/*
 * Player finishes, goes back to level picker.
 */ 
void AppCallbackFunctions::StoppedCurrentLevelCallback(void * object, hdAnimation * anim)
{
	//anim->RemoveCallbackFunctions();
	//DoLoadingScreenTransition(AppCallbackFunctions::StoppedCurrentLevel_Thread, ((AppController *)object));
}


/*
 * Transition from Game controller to level picker controller.
 */
/*
void AppCallbackFunctions::ShowLevelPickerControllerCallback(void * object, hdAnimation * anim)
{
	anim->RemoveCallbackFunctions();
	AppController *self = (AppController *)object;
	DoLoadingScreenTransition(AppCallbackFunctions::ShowLevelPickerController_Thread, self);
}*/


/*
 * Transition when player clicks on a level picker level
 */
/*
void AppCallbackFunctions::LevelPickerLevelClickedCallback(void *object, hdAnimation *anim)
{
	anim->RemoveCallbackFunctions();
	AppController *self = (AppController *)object;
	DoLoadingScreenTransition(AppCallbackFunctions::LevelPickerLevelClicked_Thread, self);
}*/


/*
 * From lev picker back to menu
 */
/*
void AppCallbackFunctions::LevelPickerBackCallback(void *object, hdAnimation *anim)
{
	anim->RemoveCallbackFunctions();
	DoLoadingScreenTransition(AppCallbackFunctions::LevelPickerBack_Thread, ((AppController *)object));
}
*/

/*
 * Game controller quitting back to level picker
 */
void AppCallbackFunctions::QuitCurrentLevelCallback(void *object, hdAnimation *anim)
{
	//anim->RemoveCallbackFunctions();
	//DoLoadingScreenTransition(AppCallbackFunctions::QuitCurrentLevel_Thread, ((AppController *)object));
}

