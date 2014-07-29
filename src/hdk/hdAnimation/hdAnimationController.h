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

/* 
 * Animations rules:
 * - animations can be created by anything that has access to hdAnimationEngine.h
 * - animations must be created via a private constructor that also destructs animations.
 * - because of the "frequent reuse" nature of animations, there should perhaps be
 *   a pool of animation objects available - the pool is simple recycled as animations
 *   are created.
 * - at no stage can an animation object be released.
 */


#ifndef _HD_ANIMATION_CONTROLLER_H_
#define _HD_ANIMATION_CONTROLLER_H_

#include <list>
#include "hdCollections.h"

#define kPipelineSize 8196

#define kAnimationPoolCapacity 512

class hdAnimation;

class hdAnimationController
{
private:
	friend class hdAnimation;	
	
	void AddAnimationStep(const hdAnimation* animation, int* stepId);
	
	bool DeleteAnimationStep(const hdAnimation* animation, const int id);

	hdTypedefList<hdAnimation*, kAnimationPoolCapacity> *m_animationPool;
	
	short m_nextAnimIndex;

	hdAnimation* m_pipeline[kPipelineSize];
	
	hdAnimation* m_HACKHACKHACK_tmpPipeline[kPipelineSize];
	
	void * m_lock;
	
	hdTimeInterval m_updateInterval;
	
	hdTimeInterval m_lastStepInterval;
	
	int m_pipelineCount;
	
	std::list<hdAnimation*> m_nonPooledAnimations;


public:

	static hdAnimationController* Instance();
	
	static void TearDown();
	
	static hdAnimation* CreateAnimation(const void *parent);
	
	static hdAnimation* CreateAnimation(const void *parent, const bool isTemporary);
	
	void Start();
	
	void Step();
	
	void StepWithInterval(const hdTimeInterval elapsed);
	
	void StopAnimationController();
	
	void StopAnimationsWithCallbackObject(void *obj);
	
	void StopAnimations(void *parent);
	
	void PurgeAnimations(void *parent);


protected:

    hdAnimationController();
	
	~hdAnimationController();
	
	hdAnimation* GetNextAnimationFromPool();
	
	hdAnimation* CreateNewAnimation();

};

#endif