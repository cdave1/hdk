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

#include "hdOrthographicProjection.h"

hdOrthographicProjection::hdOrthographicProjection(hdGameWorld* world, const hdAABB& aabb) : hdGameObject(world)
{
    m_aabb = aabb;
    m_obb = hdOBB();

    // Set all 8 of the obb vertices.
    m_obb.obbType = e_obb3D;
    m_obb.vertices[0].Set(aabb.lower.x, aabb.lower.y, aabb.lower.z);
    m_obb.vertices[1].Set(aabb.lower.x, aabb.upper.y, aabb.lower.z);
    m_obb.vertices[2].Set(aabb.upper.x, aabb.upper.y, aabb.lower.z);
    m_obb.vertices[3].Set(aabb.upper.x, aabb.lower.y, aabb.lower.z);

    m_obb.vertices[4].Set(aabb.lower.x, aabb.lower.y, aabb.upper.z);
    m_obb.vertices[5].Set(aabb.lower.x, aabb.upper.y, aabb.upper.z);
    m_obb.vertices[6].Set(aabb.upper.x, aabb.upper.y, aabb.upper.z);
    m_obb.vertices[7].Set(aabb.upper.x, aabb.lower.y, aabb.upper.z);

    m_obb.transform.rotation.Set(0,0,0);
    m_obb.transform.translation.Set(0,0,0);
    m_obb.transform.scale.Set(1,1,1);
    
    m_transform = m_obb.transform;
}


void hdOrthographicProjection::ResetOBB()
{
}
