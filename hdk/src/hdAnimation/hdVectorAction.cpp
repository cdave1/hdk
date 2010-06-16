//
//  hdVectorAction.mm
//  AnimationEngine
//
//  Created by david on 16/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#include "hdVectorAction.h"


hdVectorAction::hdVectorAction()
{
	m_destination = hdVec3(0, 0, 0);
}


void hdVectorAction::Apply(hdTimeInterval elapsed, hdGameObject* gameObject)
{
	if (gameObject == NULL) return;
	
	float delta = this->GetTimingFunctionDelta(elapsed);
	
	hdVec3 current = gameObject->GetWorldCenter(); //GetTransform().translation; // GetPosition();
	
	hdVec3 distanceToDest = m_destination;
	
	// distance between the destination and the current game object location.
	distanceToDest -= current;
	
	// Calculate the total distance travelled by dividing the distance
	// by the percentage to travel.
	float denom = (1.0f - ((m_progress - elapsed) / m_duration));
	if (denom <= 0.0f) return;
	distanceToDest *= (1.0f / denom);
	// Multiply this by the elapsed proportion of time
	distanceToDest *= (delta * (elapsed / m_duration));
	
	//hdTranslateVertices(gameObject->GetVertices(), gameObject->GetVertexCount(), distanceToDest);
	//gameObject->GetTransform().translation = current+distanceToDest;
	//gameObject->ResetAABB();
	
	gameObject->Translate(distanceToDest, current);
	
#ifdef ANIMATION_DEBUG
	hdPrintf("Name: %d, Progress: %f, Elapsed: %f, Moving to: %f, %f, %f\n", gameObject->GetName(), m_progress, elapsed, (float)current.x, (float)current.y, (float)current.z);
#endif
}


