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

// Describes a generalised orthographic projection.

#ifndef _HD_ORTHOGRAPHIC_PROJECTION_H_
#define _HD_ORTHOGRAPHIC_PROJECTION_H_

#include <hdk/physics/hdGameWorld.h>
#include <hdk/physics/hdGameObject.h>

class hdOrthographicProjection : public hdGameObject
{
public:
    hdOrthographicProjection();

    hdOrthographicProjection(hdGameWorld* world, const hdAABB& aabb);

    void ResetOBB();

    void Rotate(const hdVec3& rotationVector, const hdVec3& rotationAxis) {}

    void Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis) {}

    void SetAABB(const hdVec3& lower, const hdVec3& upper);

    hdVec3* GetVertices()
    {
        return m_obb.vertices;
    }

    const int GetVertexCount() const
    {
        return m_obb.GetVertexCount();
    }

    void ApplyTransform(const hdTransform& transform)
    {
        m_transform = transform;
        ResetOBB();
    }


private:
    hdTransform m_transform;
    
};


inline void hdOrthographicProjection::SetAABB(const hdVec3& lower, const hdVec3& upper)
{
    m_aabb.lower = lower;
    m_aabb.upper = upper;
}

#endif
