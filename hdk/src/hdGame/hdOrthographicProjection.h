/*
 *  hdOrthographicProjection.h
 *  hdGameEngine
 *
 *  Created by david on 16/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

// Describes a generalised orthographic projection.

#ifndef _HD_ORTHOGRAPHIC_PROJECTION_H_
#define _HD_ORTHOGRAPHIC_PROJECTION_H_

#include "hdGame/hdGameWorld.h"
#include "hdGame/hdGameObject.h"

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