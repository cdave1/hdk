//
//  hdScaleAction.mm
//  AnimationEngine
//
//  Created by david on 16/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#include "hdScaleAction.h"


hdScaleAction::hdScaleAction()
{
	m_scale = hdVec3(0,0,0);
}


void hdScaleAction::Apply(hdTimeInterval elapsed, hdGameObject* gameObject)
{
	hdAssert(m_duration >= 0.0);
	hdAssert(elapsed > 0.0);
	
	hdVec3 current = gameObject->GetTransform().scale; 
	//->GetBoundingBox();
	hdVec3 identity = hdVec3(1.0, 1.0, 1.0);
	
	hdVec3 step = m_scale;
	step -= identity;
	
	// Work out original scale: vecOrig = vecCur/(1+(scale * progress))
	fixed32 xOrig = current.x * (1.0 / (1.0 + (step.x * ((m_progress - elapsed) / m_duration))));
	fixed32 yOrig = current.y * (1.0 / (1.0 + (step.y * ((m_progress - elapsed) / m_duration))));
	fixed32 zOrig = current.z * (1.0 / (1.0 + (step.z * ((m_progress - elapsed) / m_duration))));
	
	hdVec3 scale0 = hdVec3(xOrig, yOrig, zOrig);
	
	// Multiply the step factor by the current progress.
	step *= (m_progress / m_duration);
	
	scale0.x = scale0.x + (scale0.x * step.x);
	scale0.y = scale0.y + (scale0.y * step.y);
	scale0.z = scale0.z + (scale0.z * step.z);

	hdVec3 center = gameObject->GetWorldCenter();
	hdTranslateVertices(gameObject->GetVertices(), gameObject->GetVertexCount(), -center);
	hdScaleVertices(gameObject->GetVertices(), gameObject->GetVertexCount(), scale0);
	hdTranslateVertices(gameObject->GetVertices(), gameObject->GetVertexCount(), center);
	
	gameObject->GetTransform().scale = scale0;
	gameObject->ResetAABB();
#ifdef ANIMATION_DEBUG
	hdPrintf("Name: %d, Elapsed: %f, Scaled to: %f, %f, %f\n", gameObject->GetName(), elapsed, (float)scale0.x, (float)scale0.y, (float)scale0.z);
#endif
}

