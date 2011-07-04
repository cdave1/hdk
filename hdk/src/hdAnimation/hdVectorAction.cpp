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
	float proportion = this->GetTimingFunctionProportion(elapsed);
	
	hdVec3 current = gameObject->GetWorldCenter();
	
	// distance between the destination and the current game object location.
	hdVec3 distanceToDest = m_destination - current;
	
	// Calculate the total distance travelled by dividing the distance
	// by the percentage of the original distance already travelled; this
	// allows us to perform the vector action without retaining a starting
	// position for the object being moved.
	float denom = (1.0f - proportion);
	if (denom <= 0.0f) return;
	distanceToDest *= (1.0f / denom);
	
	// Multiply this by the elapsed proportion of time
	distanceToDest *= (delta * (elapsed / m_duration));
	
	gameObject->Translate(distanceToDest, current);
	
#ifdef ANIMATION_DEBUG
	hdPrintf("Name: %d, Progress: %f, Elapsed: %f, Moving to: %f, %f, %f\n", gameObject->GetName(), m_progress, elapsed, (float)current.x, (float)current.y, (float)current.z);
#endif
}


