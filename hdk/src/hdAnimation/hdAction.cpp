//
//  hdAction.m
//  AnimationEngine
//
//  Created by david on 14/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#include "hdAction.h"

hdAction::hdAction()
{
	m_duration = 0.0;
	m_timingFunction = hdAnimationTimingFunctionLinear;
	m_progress = 0.0;
	m_HACKHACKHACK_logTiming = false;
}


hdAction::~hdAction()
{
	// delete the timing function
}
	


const bool hdAction::isFinished()
{
	return ((m_duration - m_progress) < 0.000001);
}


void hdAction::AddToProgress(const hdTimeInterval elapsed)
{
#if 0
	m_progress += (this->GetTimingFunctionDelta(elapsed) * elapsed);
	if (m_progress > m_duration) m_progress = m_duration;	
#else
	m_progress += elapsed;
	if (m_progress > m_duration) m_progress = m_duration;
#endif
}


void hdAction::FinishWithoutInterpolation()
{
	if (m_progress < m_duration) m_progress = m_duration;
}


const hdTimeInterval hdAction::ClampElapsed(const hdTimeInterval elapsed)
{
	if (m_progress + elapsed > m_duration) 
		return (m_duration - m_progress);
	else return elapsed;
}


const float hdAction::GetTimingFunctionDelta(const hdTimeInterval elapsed)
{
	if (m_duration == 0.0f) return 1.0f;
	
	if (m_timingFunction == hdAnimationTimingFunctionDecelerate)
	{
		float x = hdClamp((m_progress + elapsed)/m_duration, 0.0f, 1.0f);
		if (x == 0.0f) return 1.0f;
		
		float s = 1.0f - (sinf(x * hd_half_pi));
		hdPrintf("%3.5f\n",s/x);
		return hdClamp(2.0f * (s/x), 1.0f, 10.0f);
	}
	return 1.0f; 
}