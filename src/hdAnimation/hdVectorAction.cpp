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
    hdPrintf("Name: %d, Progress: %f, Elapsed: %f, Moving to: %f, %f, %f\n", gameObject->GetName(),
             m_progress, elapsed, (float)current.x, (float)current.y, (float)current.z);
#endif
}
