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
    hdPrintf("Name: %d, Elapsed: %f, Scaled to: %f, %f, %f\n", gameObject->GetName(), elapsed,
             (float)scale0.x, (float)scale0.y, (float)scale0.z);
#endif
}
