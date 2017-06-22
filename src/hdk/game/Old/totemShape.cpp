/*
 *  totemShape.cpp
 *  LevelEditor
 *
 *  Created by david on 27/03/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "totemShape.h"


totemShape::totemShape() 
{
	memset(m_vertices, 0, sizeof(m_vertices));
	m_vertexCount = 0;
	m_physicsBody = NULL;
}


totemShape::totemShape(hdGameWorld *gameWorld, 
					   b2World *physicsWorld,
					   const hdVec2& center,
					   const int name)
{
	memset(m_vertices, 0, sizeof(m_vertices));
	m_vertexCount = 0;
	
	hdGameObjectDef def;
	def.position.Set(center.x, center.y, 0.0f);
	def.boundingBox.Set(1.0f, 1.0f, 1.0f);
	def.type = e_gameObject;
	def.name = name;
	m_def = def;
	
	((hdGameObjectProxy *)this)->Init(gameWorld, def);
	
	m_physicsWorld = physicsWorld;
}


totemShape::~totemShape()
{
	this->DestroyPhysicsBody();
}


void totemShape::Init(hdGameWorld *gameWorld, 
					 b2World *physicsWorld)
{
	((hdGameObjectProxy *)this)->Init(gameWorld);
	m_physicsWorld = physicsWorld;
	this->CreatePhysicsBody();
}


const int totemShape::AddPoint(const float x, const float y)
{
	return this->AddPoint(x,y,0.0f);
}


// Appends a point to the list of points.
const int totemShape::AddPoint(const float x, const float y, const float z)
{
	return this->AddPoint(hdVec3(x,y,z));
}


const int totemShape::AddPoint(const hdVec3& vertex)
{
	hdAssert(m_vertexCount >= 0 && m_vertexCount <= HDK_GAME_MaxShapeVertices);
	if (m_vertexCount == HDK_GAME_MaxShapeVertices) return -1;
	
	// Make room for the new level
	m_vertices[m_vertexCount] = vertex;
	++m_vertexCount;
	
	this->Reset();
	this->ResetGameObjectBounds();
	
	
	return m_vertexCount;
}



const bool totemShape::RemovePoint(const int index)
{
	hdAssert(m_vertexCount >= 0 && m_vertexCount <= HDK_GAME_MaxShapeVertices);
	if (m_vertexCount == 0) return false;
	if (index > m_vertexCount) return false;
	if (index < 0) return false;
	
	m_vertices[index] = hdVec3(0, 0, 0);
	
	// move everything down
	for (int i = index; i < m_vertexCount-1; i++)
	{
		m_vertices[i] = m_vertices[i+1];
	}
	m_vertices[m_vertexCount] = hdVec3(0,0,0);
	--m_vertexCount;
	this->ResetGameObjectBounds();
	return true;
}


// HACK HACK HACK
//
// determine if the co-ordinates in the polygon will result in
// a valid b2 centroid.
//
// This code ripped from box2D
bool totemIsCentroidValid(b2PolygonDef &def)
{
	if (def.vertexCount < 3) return false;
	
	//b2Assert(count >= 3);
	
	b2Vec2 c; c.Set(0.0f, 0.0f);
	float32 area = 0.0f;
	
	// pRef is the reference point for forming triangles.
	// It's location doesn't change the result (except for rounding error).
	b2Vec2 pRef(0.0f, 0.0f);

	//const float32 inv3 = 1.0f / 3.0f;
	
	for (int32 i = 0; i < def.vertexCount; ++i)
	{
		// Triangle vertices.
		b2Vec2 p1 = pRef;
		b2Vec2 p2 = def.vertices[i];
		b2Vec2 p3 = i + 1 < def.vertexCount ? def.vertices[i+1] : def.vertices[0];
		
		b2Vec2 e1 = p2 - p1;
		b2Vec2 e2 = p3 - p1;
		
		float32 D = b2Cross(e1, e2);
		
		float32 triangleArea = 0.5f * D;
		area += triangleArea;
		
		// Area weighted centroid
		//c += triangleArea * inv3 * (p1 + p2 + p3);
	}
	
	return (area > B2_FLT_EPSILON);
}


// Create three point polygon shapes in a
// triangle fan type configuration.
void totemShape::CreatePhysicsBody()
{
	
	
	//m_physicsBody->SetMassFromShapes();	
	GameObject* physicsUserObject = new GameObject(e_hdkTypeShape, this);
	m_physicsBody->SetUserData(physicsUserObject);
}


void totemShape::Reset()
{
	this->Reset();
	this->DestroyPhysicsBody();
	this->CreatePhysicsBody();
}



// Find the position
void totemShape::ResetGameObjectBounds()
{
	hdVec3 aa, bb, middle;
	aa = m_vertices[0];
	bb = m_vertices[0];
	//hdVec3 pos = m_gameObject->GetPosition();
	
	for (int i = 0; i < m_vertexCount; i++)
	{
		aa = hdMin(aa, m_vertices[i]);
	}
	
	for (int i = 0; i < m_vertexCount; i++)
	{
		bb = hdMax(bb, m_vertices[i]);
	}
	
	middle = aa + (0.5f * (bb - aa));
	
	m_gameObject->SetPosition(middle);
}


void totemShape::Step()
{
	if (m_physicsBody == NULL) return;
	// As per physics update code....
}


const hdVec3* totemShape::GetAABB()
{
	
	if (m_vertexCount < 2) 
	{
		m_aabb[0].SetZero();
		m_aabb[1].SetZero();
	} 
	else 
	{
		m_aabb[0] = m_vertices[0];
		m_aabb[1] = m_vertices[0];	
		for (int i = 0; i < m_vertexCount; i++)
		{
			m_aabb[0] = hdMin(m_aabb[0], m_vertices[i]);
		}
		
		for (int i = 0; i < m_vertexCount; i++)
		{
			m_aabb[1] = hdMax(m_aabb[1], m_vertices[i]);
		}
	}
	return m_aabb;
}


void totemShape::Draw() const
{
	
	
	glPushMatrix();
	
	if (m_gameObject->GetAngle().x != 0.0) glRotatef(m_gameObject->GetAngle().x, 1.0, 0, 0);
	if (m_gameObject->GetAngle().y != 0.0) glRotatef(m_gameObject->GetAngle().y, 0, 1.0, 0);
	if (m_gameObject->GetAngle().z != 0.0) glRotatef(m_gameObject->GetAngle().z, 0, 0, 1.0);
	if ((m_gameObject->GetBoundingBox() == hdVec3(1.0f, 1.0f, 1.0f)) == false)
	{
		glScalef(m_gameObject->GetBoundingBox().x, m_gameObject->GetBoundingBox().y, m_gameObject->GetBoundingBox().z);
	}
	glPushMatrix();
	
#ifdef LEVEL_EDITOR
	hdglColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	hdglBegin(GL_POINTS);
	hdglVertex2f(m_gameObject->GetPosition().x, m_gameObject->GetPosition().y);
	hdglEnd();
#endif
//#endif
	
	hdVec3 center(0.0f, 0.0f, 0.0f);

	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	hdglColor4f(0.0f, 0.5f, 1.0f, 1.0f);
	hdglBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < m_vertexCount; i++)
	{
		hdVec3 v = center + m_vertices[i];
		hdglVertex2f(v.x, v.y);
	}
	hdglEnd();
	glDisable(GL_BLEND);
#ifdef LEVEL_EDITOR	
	hdglBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < m_vertexCount; i++)
	{
		hdVec3 v = center + m_vertices[i];
		hdglVertex2f(v.x, v.y);
	}
	hdglEnd();
#endif
	glPopMatrix();
	glPopMatrix();
}
