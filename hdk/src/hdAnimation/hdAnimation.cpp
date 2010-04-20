//
//  Animation.mm
//  AnimationEngine
//
//  Created by david on 14/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#include "hdAnimation.h"
#include "hdAnimationController.h"

hdAnimation::hdAnimation()
{
	m_callbackObj = NULL;
	
	m_status = e_animationStopped;
	
	m_actionCounter = -1;
	
	m_gameObjects = new hdTypedefList<hdGameObject*, 128>();
	
	m_actions = new hdPointerList<hdAction, 16>();
	
	m_id = -1;
	
	m_parent = NULL;
}


hdAnimation::~hdAnimation()
{
	//RemoveAllActions();
	if (hdAnimationController::Instance()->DeleteAnimationStep(this, m_id) == false)
	{
		hdPrintf("hdAnimation Destructor: could not remove animation job from pipeline.\n");
	}
	delete m_gameObjects;
	delete m_actions;
}



int hdAnimation::StartAnimation()
{
	m_actionCounter = m_actions->GetItemCount() - 1; //self.actions.count - 1;
	
	for (int i = 0; i < m_actions->GetItemCount(); ++i)
	{
		m_actions->GetItems()[i]->SetProgress(0.0);
	}
	
	if (m_actionCounter == -1) return 0;
	m_status = e_animationRunning;
	
	hdAnimationController::Instance()->AddAnimationStep(this, &m_id);
	return 1;
}


void hdAnimation::PauseAnimation()
{
	if (this->m_status != e_animationRunning) return;
	
	m_status = e_animationPaused;
}


void hdAnimation::ResumeAnimation()
{
	if (this->m_status != e_animationPaused) return;
	
	m_status = e_animationRunning;
}


void hdAnimation::StopAnimation(bool completeAnimation)
{
	if (completeAnimation) this->StopAnimationWithCompletion();
	else this->StopAnimationDead();
}


void hdAnimation::StopAnimation(bool completeAnimation, bool doCallback)
{
	this->StopAnimation(completeAnimation);
	if (doCallback) this->DoFinishedCallback();
}


void hdAnimation::StopAnimationDead()
{
	//hdAnimationController::Instance()->DeleteAnimationStep(this, m_id);
	hdAnimationController::Instance()->DeleteAnimationStep(this, m_id);
	m_id = -1;
	if (m_status == e_animationFinished) return;
	m_status = e_animationStopped;
}


void hdAnimation::StopAnimationWithCompletion()
{
	if (hdAnimationController::Instance()->DeleteAnimationStep(this, m_id) == false)
	{
		hdPrintf("hdAnimation: could not remove animation job from pipeline.\n");
		m_status = e_animationFinished;
	}
	
	m_id = -1;
	if (m_status == e_animationFinished) return;
	m_status = e_animationStopped;
	
	//if (actionCounter == -1) return;
	
	// Finish all of the remaining actions
	while (m_actionCounter > -1)
	{
		hdAction* action = m_actions->GetItems()[m_actionCounter];
		
		if (action != NULL)
		{
			hdTimeInterval remaining = (action->GetDuration() - action->GetProgress());
			
			remaining = action->ClampElapsed(remaining);
			action->AddToProgress(remaining);
			
			if (remaining > 0.0) 
			{
				for (int i = 0; i < m_gameObjects->GetItemCount(); ++i)
				{
					hdGameObject* obj = m_gameObjects->GetItems()[i];
					action->Apply(remaining, obj);
				}
			}
			action->SetProgress(0.0);
		}
		--m_actionCounter;
	}
	
	m_status = e_animationFinished;
}


void hdAnimation::Step(hdTimeInterval elapsed)
{
	if (m_actionCounter == -1) return;
	
#ifdef DEBUG
	hdAssert(m_status != e_animationFinished);
#else
	// TODO: Fix whatever causes this to happen:
	if (m_status == e_animationFinished)
	{
		hdPrintf("Caution: animation job continued even after removed from pipeline.\n");
		return;
	}
#endif

	
	if (m_status == e_animationStopped)
	{
		return;
	}
	
	hdAction* action = (hdAction *)m_actions->GetItems()[m_actionCounter]; // [self.actions objectAtIndex:actionCounter]; // head of queue
	if (action->isFinished()) 
	{
		//[self.actions removeObjectAtIndex:0];
		--m_actionCounter;
		if (m_actionCounter == -1) 
		{
			// The animation has run out of actions - we're done here.
			m_status = e_animationFinished;

			// Callback.
			this->DoFinishedCallback();
			m_id = -1;
			return;
		}
		else action =  m_actions->GetItems()[m_actionCounter]; // get another action
		// HACK HACK HACK - the fact we have to do this ^^^ here indicates something
		// wrong with the structure of this function.
		// TODO REFACTORME FIXME etc etc...
	}
	
	if (m_status == e_animationRunning)
	{
		hdTimeInterval elapsedTime = action->ClampElapsed(elapsed);
		action->AddToProgress(elapsedTime);
		
		// Apply the action to each game object
		for (int i = 0; i < m_gameObjects->GetItemCount(); ++i)
		{
			hdGameObject* obj = m_gameObjects->GetItems()[i];
			action->Apply(elapsedTime, obj);
		}
	}
	
	// Add this function to the pipeline so it is run on the next step.
	hdAnimationController::Instance()->AddAnimationStep(this, &m_id);
}


