//
//  hdTranslationAction.m
//  AnimationEngine
//
//  Created by david on 14/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#include "hdTranslationAction.h"


hdTranslationAction::hdTranslationAction()
{
	m_start = hdVec3(0, 0, 0);
	m_destination = hdVec3(0, 0, 0);
}


void hdTranslationAction::Apply(hdTimeInterval elapsed, hdGameObject* gameObject)
{
	hdVec3 current = gameObject->GetTransform().translation;
	
	hdVec3 stepDistance = m_destination;
	stepDistance -= m_start;
	stepDistance *= (m_progress / m_duration);
	
	hdVec3 newPosition = m_start;
	newPosition += stepDistance;
	
	//gameObject->SetPosition(newPosition);
	// Add the diff
	hdTranslateVertices(gameObject->GetVertices(), gameObject->GetVertexCount(), newPosition-current);
	gameObject->ResetAABB();
#ifdef ANIMATION_DEBUG
	hdPrintf("Name: %d, Elapsed: %f, Moving to: %f, %f, %f\n", gameObject->GetName(), elapsed, (float)newPosition.x, (float)newPosition.y, (float)newPosition.z);
#endif
}

