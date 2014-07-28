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

    // Add the diff
    hdTranslateVertices(gameObject->GetVertices(), gameObject->GetVertexCount(), newPosition-current);
    gameObject->ResetAABB();
#ifdef ANIMATION_DEBUG
    hdPrintf("Name: %d, Elapsed: %f, Moving to: %f, %f, %f\n", gameObject->GetName(), elapsed,
             (float)newPosition.x, (float)newPosition.y, (float)newPosition.z);
#endif
}
