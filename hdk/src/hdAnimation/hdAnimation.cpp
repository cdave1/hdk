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
	if (hdAnimationController::Instance()->DeleteAnimationStep(this, m_id) == false)
	{
		hdPrintf("hdAnimation Destructor: could not remove animation job from pipeline.\n");
	}
	delete m_gameObjects;
	delete m_actions;
}


int hdAnimation::StartAnimation()
{
	m_actionCounter = m_actions->GetItemCount() - 1;
	
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
	
	hdAction* action = (hdAction *)m_actions->GetItems()[m_actionCounter];
	if (action->isFinished()) 
	{
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
		else action =  m_actions->GetItems()[m_actionCounter];
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
