//
//  hdRotationAction.mm
//  AnimationEngine
//
//  Created by david on 16/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#include "hdRotationAction.h"
#define ClampAngle(angle) (angle >= (2.0 * hd_pi)) ? angle - (2.0 * hd_pi) : angle

hdRotationAction::hdRotationAction()
{
	m_rotation = hdVec3(0,0,0);
}


void hdRotationAction::Apply(hdTimeInterval elapsed, hdGameObject* gameObject)
{
	if (gameObject == NULL) return;
	//hdAssert(self.duration >= 0.0);
	//hdAssert(elapsed > 0.0);
	
	// Relative rotation.
	hdVec3 current = gameObject->GetTransform().rotation; // GetAngle();
	
	hdVec3 rotationStep = m_rotation;
	
	rotationStep *= (elapsed / m_duration);
	
	current += rotationStep;
	current.x = ClampAngle(current.x);  
	current.y = ClampAngle(current.y); 
	current.z = ClampAngle(current.z);
	
	//gameObject->SetAngle(current);
	//hdTranslateVertices(gameObject->GetVertices(), gameObject->GetVertexCount(), distanceToDest);
	//gameObject->GetTransform().translation = current+distanceToDest;
	
	
	
	hdTranslateVertices(gameObject->GetVertices(), gameObject->GetVertexCount(), -gameObject->GetWorldCenter());
	
	hdRotateVertices(gameObject->GetVertices(), gameObject->GetVertexCount(), rotationStep);
	
	hdTranslateVertices(gameObject->GetVertices(), gameObject->GetVertexCount(), gameObject->GetWorldCenter());
	
	
	
	
	
	gameObject->GetTransform().rotation = current;
	gameObject->ResetAABB();
	
#ifdef ANIMATION_DEBUG
	hdPrintf("Name: %d, Elapsed: %f, Rotated to: %f, %f, %f\n", gameObject->GetName(), elapsed, (float)current.x, (float)current.y, (float)current.z);
#endif
}
