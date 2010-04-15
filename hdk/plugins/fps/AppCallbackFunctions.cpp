/*
 *  AppCallbackFunctions.cpp
 *  TotemGame
 *
 *  Created by david on 23/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
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

