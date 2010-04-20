/*
 *  hdPauseAction.h
 *  Skeleton
 *
 * Pauses the animation for a specific period.
 *
 *  Created by david on 23/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_PAUSE_ACTION_H_
#define _HD_PAUSE_ACTION_H_

#include "hdAnimation/hdAction.h"

class hdPauseAction : public hdAction
{
public:
	hdPauseAction() {}
	
	~hdPauseAction() {}
	
	void Apply(hdTimeInterval elapsed, hdGameObject* gameObject);
	
};


inline void hdPauseAction::Apply(hdTimeInterval elapsed, hdGameObject* gameObject)
{
#ifdef ANIMATION_DEBUG
	hdPrintf("hdPauseAction->Apply: Name: %d, Progress: %f, Elapsed: %f\n", gameObject->GetName(), m_progress, elapsed, m_func);
#endif			
}


#endif