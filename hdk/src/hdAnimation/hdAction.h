//
//  hdAction.h
//  AnimationEngine
//
//  Created by david on 14/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#ifndef _HD_ACTION_H_
#define _HD_ACTION_H_

#include "hdGame/hdGameObject.h"
#include "hdAnimation/hdAnimationTimingFunction.h"

class hdAction
{
public:
	hdAction();
	
	virtual ~hdAction();
	
	const bool isFinished();
	
	void AddToProgress(const hdTimeInterval elapsed);
	
	void FinishWithoutInterpolation();
	
	const hdTimeInterval ClampElapsed(const hdTimeInterval elapsed);
	
	virtual void Apply(hdTimeInterval elapsed, hdGameObject* gameObject) = 0;
	
	const hdTimeInterval GetProgress();
	
	void SetProgress(const hdTimeInterval progress);
	
	const hdTimeInterval GetDuration();
	
	void SetDuration(const hdTimeInterval duration);
	
	bool m_HACKHACKHACK_logTiming;
	
protected:
	hdAnimationTimingFunction* m_timingFunction;
	
	hdTimeInterval m_duration;
	
	hdTimeInterval m_progress;
	
	
	
};


inline const hdTimeInterval hdAction::GetProgress()
{
	return m_progress;
}


inline void hdAction::SetProgress(const hdTimeInterval progress)
{
	m_progress = progress;
}


inline const hdTimeInterval hdAction::GetDuration()
{
	return m_duration;
}


inline void hdAction::SetDuration(const hdTimeInterval duration)
{
	m_duration = duration;
}

#endif