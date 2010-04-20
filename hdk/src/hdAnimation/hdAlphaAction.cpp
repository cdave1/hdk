/*
 *  hdAlphaAction.cpp
 *  TotemGame
 *
 *  Created by david on 23/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdAlphaAction.h"
#include "hdGraphicsLibrary/hdPolygon.h"

hdAlphaAction::hdAlphaAction()
{
	m_startAlpha = 0.0f;
	m_endAlpha = 0.0f;
	m_alphaObject = new hdPolygon();
}


hdAlphaAction::~hdAlphaAction() 
{
	delete m_alphaObject;
}


void hdAlphaAction::Apply(hdTimeInterval elapsed, hdGameObject* gameObject)
{
	float current;
	if (m_endAlpha > m_startAlpha) 
	{
		current = (m_endAlpha - m_startAlpha) * ((m_progress + elapsed) / m_duration);
	} 
	else
	{
		current = 1.0f + (m_endAlpha - m_startAlpha) * ((m_progress + elapsed) / m_duration);
	}
	
	current = hdClamp(current, 0.0f, 1.0f);
	
	gameObject->SetAlpha(current);
	
#ifdef ANIMATION_DEBUG
	hdPrintf("Alpha Action:: Name: %d, Progress: %f, Elapsed: %f, New Alpha: %f\n", gameObject->GetName(), m_progress, elapsed, (float)current);
#endif
}