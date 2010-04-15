/*
 *  hdOrthographicProjection.cpp
 *  hdGameEngine
 *
 *  Created by david on 16/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
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
	// Get the difference between the rotation of the obb
	// transform and then apply.

}