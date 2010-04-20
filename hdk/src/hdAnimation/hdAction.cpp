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
	m_duration = 1.0;
	m_timingFunction = NULL;
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
	m_progress += elapsed;
	if (m_progress > m_duration) m_progress = m_duration;
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
