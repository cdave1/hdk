//
//  PipelineController.h
//  AnimationEngine
//
//  Created by david on 14/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

// Animations rules:
// - animations can be created by anything that has access to hdAnimationEngine.h
// - animations must be created via a private constructor that also destructs animations.
// - because of the "frequent reuse" nature of animations, there should perhaps be
//   a pool of animation objects available - the pool is simple recycled as animations
//   are created.
// - at no stage can an animation object be released.


#ifndef _HD_ANIMATION_CONTROLLER_H_
#define _HD_ANIMATION_CONTROLLER_H_

#include <list>

#include "hdAnimation/hdAnimation.h"
#include "hdAnimation/hdAction.h"

#define kPipelineSize 8196

class hdAnimation;

class hdAnimationController
{
private:
	friend class hdAnimation;	
	
	void AddAnimationStep(const hdAnimation* animation, int* stepId); // withFunction:(SEL)selector;
	
	bool DeleteAnimationStep(const hdAnimation* animation, const int id);
	
	/**********************************************************************/
	/* Animation pools */
	/*
	 
	 The grand idea with these animation pools is to have a set of
	 animations that can repeatedly be used by the game controller.
	 
	 The animation are used by various static animation handlers that get the 
	 next available animation, stop it, clear it of game objects, set
	 parameters, then start the animation. Any of the animations in the
	 pool may have an arbitrary callback function that re-uses the pool. If
	 this is the case, then 64 may not be enough room if there is a lot 
	 happening on the screen. The basic situation we want to avoid is
	 a sort of thrashing, where an animation in progress is halted 
	 because another animation needs its resources. Obviously growable
	 lists are out of the questions, so for these pools I need enough room
	 and no more. Because of this, the pools should only be used for
	 animations that have short durations - less than one second, say.
	 
	 UPDATE: After thinking about this more, it became clear that I only
	 need one animation pool.
	 
	 UPDATE: Fatal flaw in the pool design - it's fairly obvious that
	 dnagling pointers are a real issue. Two solutions:
	 - use a lock, where the client is responsible for locking
	   any volatile animations. There are still flaws with this, however. Who
	   is responsible for unlocking? What if the pool gets full of locked
	   animations? Why should the client be responsible for enforcing the rules
	   of a black box animation system?
	 - Use a list, and just keep adding to the list as anims are rquested. Flaw
	   is that we might never purge the animation list, or that tens of 
	   thousands of animations are created and never purged, exhausting available
	   memory.
	 
	 Going to keep the animation pool, increase it's capacity, but also add
	 a list. An overriden CreateAnimation function will have a "bool fromPool"
	 parameter; if this is false, then the animation will be put onto the 
	 separate list after being instantiated.
	 
	 There needs to a further function - PurgeAnimations(parent) - this only
	 removes the animations in the non-pooled list.
	 
	 These fixes are temporary. Really, clients should be able to create and
	 queue up, as well as destroy, as many animation objects as they see fit,
	 and the controller and animation queue should simply be able to deal with it.
	 You should be able to create an animation, with various actions, and be
	 able to hold on to it indefinitely.
	 
	 The current fatal design flaw is that hdAnimation objects are also responsible
	 for running/queuing themselves, but really this should be the responsibility
	 of a separate "animation job." The job can just die if the animation is nulled, 
	 whereas the current design will crash horribly if this happens because there's
	 a real possbility of a dangling function pointer sitting in the queue.
	 */
	/**********************************************************************/
#define kAnimationPoolCapacity 512
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