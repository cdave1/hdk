//
//  hdAnimationController.mm
//  AnimationEngine
//
//  Created by david on 14/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#include "hdAnimationController.h"

using namespace std;

static hdAnimationController* m_animationControllerInstance = NULL;


hdAnimationController* hdAnimationController::Instance()
{
	if (m_animationControllerInstance == NULL)
	{
		m_animationControllerInstance = new hdAnimationController();
	}
	return m_animationControllerInstance;
}


void hdAnimationController::TearDown()
{
	delete m_animationControllerInstance;
	m_animationControllerInstance = NULL;
}


hdAnimationController::hdAnimationController()
{
	memset(m_pipeline, 0, sizeof(m_pipeline));
	memset(m_HACKHACKHACK_tmpPipeline, 0, sizeof(m_HACKHACKHACK_tmpPipeline));
	m_pipelineCount = 0;
	m_updateInterval = 1.0 / 60;
	m_lastStepInterval = m_updateInterval;
	m_lock = malloc(sizeof(void *));  //[[NSObject alloc] init];
	
	
	m_animationPool = new hdTypedefList<hdAnimation*, kAnimationPoolCapacity>();
	// Create the animations in the pool:
	for (int i = 0; i < kAnimationPoolCapacity; i++)
	{
		hdAnimation* anim = new hdAnimation();
		m_animationPool->Add(anim);
	}
	m_nextAnimIndex = 0;
}



hdAnimationController::~hdAnimationController()
{
	free(m_lock);
	
	hdAnimation *anim;
	if (m_animationPool != NULL)
	{
		while (!m_animationPool->IsEmpty())
		{
			anim = m_animationPool->GetItems()[0];
			anim->StopAnimationDead();
			anim->RemoveAllActions();
			delete anim;
			m_animationPool->RemoveAtIndex(0);
		}
		
		delete m_animationPool;
		m_animationPool = NULL;
	}
	
	list<hdAnimation *>::iterator it = m_nonPooledAnimations.begin();
	while(it != m_nonPooledAnimations.end())
	{
		delete (*it);
		it++;
	}
}


hdAnimation* hdAnimationController::CreateAnimation(const void *parent)
{
	return CreateAnimation(parent, true);
}


hdAnimation* hdAnimationController::CreateAnimation(const void *parent, const bool isTemporary)
{
	hdAnimation *anim;
	
	if (isTemporary)
	{
		anim = hdAnimationController::Instance()->GetNextAnimationFromPool();
	}
	else
	{
		anim = hdAnimationController::Instance()->CreateNewAnimation();
	}
	
	anim->m_parent = (void*)(parent);
	return anim;
}


hdAnimation* hdAnimationController::GetNextAnimationFromPool()
{
	hdAssert(m_nextAnimIndex < m_animationPool->GetItemCount());
	hdAnimation* anim = m_animationPool->GetItems()[m_nextAnimIndex];
	hdAssert(anim != NULL);
	anim->StopAnimation(true, false);
	anim->RemoveCallbackFunctions();
	anim->RemoveAllGameObjects();
	anim->RemoveAllActions();
	
	m_nextAnimIndex = (m_nextAnimIndex + 1)%(kAnimationPoolCapacity-1);
	return anim;
}


hdAnimation* hdAnimationController::CreateNewAnimation()
{
	hdAnimation *anim = new hdAnimation();
	m_nonPooledAnimations.push_back(anim);
	return anim;
}


void hdAnimationController::StopAnimations(void *obj)
{
	for (int i = 0; i < m_animationPool->GetItemCount(); ++i)
	{
		if (m_animationPool->GetItems()[i]->m_parent == obj)
		{
			m_animationPool->GetItems()[i]->StopAnimationDead();
			m_animationPool->GetItems()[i]->RemoveCallbackFunctions();
			m_animationPool->GetItems()[i]->RemoveAllGameObjects();
			m_animationPool->GetItems()[i]->RemoveAllActions();
		}
	}
}


void hdAnimationController::StopAnimationsWithCallbackObject(void *obj)
{
	for (int i = 0; i < m_animationPool->GetItemCount(); ++i)
	{
		if (m_animationPool->GetItems()[i]->m_callbackObj == obj)
		{
			m_animationPool->GetItems()[i]->RemoveCallbackFunctions();
			m_animationPool->GetItems()[i]->StopAnimationDead();
		}
	}
}


class purgeAnimsPredicate
{
public:
	void *parentToCompare;
	bool operator () (const hdAnimation* anim) 
	{ 
		bool purge;
		purge = anim->m_parent == parentToCompare;
		if (purge)
		{
			((hdAnimation *)anim)->StopAnimationDead();
			((hdAnimation *)anim)->RemoveCallbackFunctions();
			((hdAnimation *)anim)->RemoveAllGameObjects();
			((hdAnimation *)anim)->RemoveAllActions();
			delete anim;
		}
		return purge;
	}
};


void hdAnimationController::PurgeAnimations(void *parent)
{
	//purgeAnimsPredicate *pred = new purgeAnimsPredicate();
	//pred->parentToCompare = parent;
	//m_nonPooledAnimations.remove_if(*pred);
	
	hdTypedefList<hdAnimation *, 1024>* toDelete;
	list<hdAnimation *>::iterator it;
	hdAnimation *a;
	
	toDelete = new hdTypedefList<hdAnimation *, 1024>();

	it = m_nonPooledAnimations.begin();
	while(it != m_nonPooledAnimations.end())
	{
		if ((*it)->m_parent == parent)
		{
			toDelete->Add(*it);
		}
		it++;
	}
	
	while(!toDelete->IsEmpty())
	{
		a = toDelete->GetItems()[0];
		a->StopAnimationDead();
		a->RemoveCallbackFunctions();
		a->RemoveAllGameObjects();
		a->RemoveAllActions();
		m_nonPooledAnimations.remove(a);
		delete a;
		
		toDelete->RemoveAtIndex(0);
	}
	
	delete toDelete;
}


void hdAnimationController::Step()
{
	this->StepWithInterval(m_updateInterval);
}


// HACK HACK HACK - We must guarantee the thread safety of both this Step function and 
//
// Perform all of the selectors on the pipeline.
//
// elapsed - time elapsed since the last step.
void hdAnimationController::StepWithInterval(hdTimeInterval elapsed)
{
	if (m_pipelineCount > 0)
	{
		int p = 0;
		//@synchronized(m_lock)
		{
#ifdef ANIMATION_DEBUG
			hdPrintf("Pipeline Step: %d animations requiring steps\n", m_pipelineCount);
#endif
			p = m_pipelineCount;
			m_pipelineCount = 0;
			
			// for (int i = 0; i < p; ++i)
			// {
			// HACKHACKHACK_tmpPipeline[i] = pipeline[i];
			// HACKHACKHACK_tmpPipeline[i].selector = pipeline[i].selector;
			// }
			//
			// memcpy(m_HACKHACKHACK_tmpPipeline, m_pipeline, sizeof(hdAnimation*) * kPipelineSize);
			memcpy(m_HACKHACKHACK_tmpPipeline, m_pipeline, sizeof(hdAnimation*) * p);
			memset(m_pipeline, 0, sizeof(m_pipeline));
		}

		for(int i = 0; i < p; ++i)
		{
			hdAnimation* receiver = m_HACKHACKHACK_tmpPipeline[i];
			
			if (receiver != NULL && 
				receiver->m_id >= -1 && 
				receiver->m_id <= kPipelineSize && 
				(receiver->GetStatus() == e_animationRunning ||
				 receiver->GetStatus() == e_animationPaused))
			{
				receiver->Step(elapsed);
			} 
			else 
			{
#ifdef ANIMATION_DEBUG
				hdPrintf("### NULL Pointer in pipeline at %d\n", i);
#endif
			}
		}
	}
}


void hdAnimationController::AddAnimationStep(const hdAnimation* animation, int* stepId)
{
	if (animation == NULL) return;
	if (animation->GetStatus() == e_animationFinished || animation->GetStatus() == e_animationStopped) return;
	
	*stepId = m_pipelineCount;
	
#ifdef ANIMATION_DEBUG
	hdPrintf("\t\t[+] Added anim step at %d with id of %d\n", animation, *stepId);
#endif
	
	m_pipeline[m_pipelineCount] = (hdAnimation *)animation;
	++m_pipelineCount;
}


bool hdAnimationController::DeleteAnimationStep(const hdAnimation* animation, const int stepId)
{
	if (stepId < 0 || stepId >= kPipelineSize) return false;
	if (m_pipeline[stepId] == NULL) return false;
	if (animation->GetStatus() == e_animationFinished || animation->GetStatus() == e_animationStopped) return false;
	if (m_pipeline[stepId] != animation) return false;
	
	//hdAssert(m_pipeline[stepId] == animation);
	
#ifdef ANIMATION_DEBUG
	hdPrintf("\t\t[-] Delete anim step at %d with id of %d\n", animation, stepId);
#endif
	
	m_pipeline[stepId] = NULL;
	return true;
}


void hdAnimationController::StopAnimationController()
{
	m_pipelineCount = 0;
}

