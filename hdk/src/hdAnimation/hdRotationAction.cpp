/*
 * Copyright (c) 2014 Hackdirt Ltd.
 * Author: David Petrie (david@davidpetrie.com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software. Permission is granted to anyone to use this software for
 * any purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 * that you wrote the original software. If you use this software in a product, an
 * acknowledgment in the product documentation would be appreciated but is not
 * required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "hdRotationAction.h"
#define ClampAngle(angle) (angle >= (2.0 * hd_pi)) ? angle - (2.0 * hd_pi) : angle

hdRotationAction::hdRotationAction()
{
	m_rotation = hdVec3(0,0,0);
}


void hdRotationAction::Apply(hdTimeInterval elapsed, hdGameObject* gameObject)
{
	if (gameObject == NULL) return;
	
	// Relative rotation.
	hdVec3 current = gameObject->GetTransform().rotation;
	
	hdVec3 rotationStep = m_rotation;
	
	rotationStep *= (elapsed / m_duration);
	
	current += rotationStep;
	current.x = ClampAngle(current.x);  
	current.y = ClampAngle(current.y); 
	current.z = ClampAngle(current.z);

	hdTranslateVertices(gameObject->GetVertices(), gameObject->GetVertexCount(), -gameObject->GetWorldCenter());
	hdRotateVertices(gameObject->GetVertices(), gameObject->GetVertexCount(), rotationStep);
	hdTranslateVertices(gameObject->GetVertices(), gameObject->GetVertexCount(), gameObject->GetWorldCenter());

	gameObject->GetTransform().rotation = current;
	gameObject->ResetAABB();
	
#ifdef ANIMATION_DEBUG
	hdPrintf("Name: %d, Elapsed: %f, Rotated to: %f, %f, %f\n", gameObject->GetName(), elapsed, (float)current.x, (float)current.y, (float)current.z);
#endif
}
