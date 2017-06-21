/*
 *  totemShape.h
 *  LevelEditor
 *
 *  Created by david on 27/03/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_TOTEM_SHAPE_
#define _HD_TOTEM_SHAPE_

#include "Box2D.h"

#ifdef LEVEL_EDITOR
#include "hdOpenGL.h"
#else
#include "hdOpenGLES.h"
#endif

#include "hdGameEngine.h"
#include "hdGameWorld.h"
#include "hdMathSerializable.h"
#include "totemEnums.h"
#include "totemConstants.h"
#include "totemGameObject.h"
#include "hdPhysicsObject.h"

class totemShape : public hdPhysicsObject, public hdDrawable, public hdGameObject
{
public:
	totemShape();
	
	totemShape(hdGameWorld *gameWorld, 
			   b2World *physicsWorld,
			   const hdVec2& center,
			   const int name);
	
	~totemShape();
	
	void Init(hdGameWorld *gameWorld, b2World *physicsWorld);
	
	void Draw() const;
	
	void Step();
	
	void CreatePhysicsBody();
	
	void Reset();
	
	const int AddPoint(const float x, const float y);
	
	const int AddPoint(const float x, const float y, const float z);
	
	const int AddPoint(const hdVec3& vertex);
	
	const bool RemovePoint(const int index);
	
	
protected:
	void ResetGameObjectBounds();
	
	
private:
	int m_vertexCount;
	
	hdVec3 m_vertices[kMaxShapeVertices];
	
	friend class boost::serialization::access;
	
	
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & boost::serialization::base_object<hdGameObject>(*this);
		ar & m_vertexCount;
		for (int i = 0; i < m_vertexCount; i++)
		{
			ar & m_vertices[i].x;
			ar & m_vertices[i].y;
			ar & m_vertices[i].z;
		}
	}
	
	
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<hdGameObject>(*this);
		ar & m_vertexCount;
		for (int i = 0; i < m_vertexCount; i++)
		{
			float32 x, y, z;
			ar & x;
			ar & y;
			ar & z;
			m_vertices[i].x = fixed32(x);
			m_vertices[i].y = fixed32(y);
			m_vertices[i].z = fixed32(z);
		}
			
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
};

#endif