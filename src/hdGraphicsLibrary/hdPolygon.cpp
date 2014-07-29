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

#include "hdPolygon.h"

hdPolygon::hdPolygon() : hdGameObject()
{
	m_vertices = new hdTypedefList<hdVec3, kMaxPolygonVertices>();
	m_startingVertices = new hdTypedefList<hdVec3, kMaxPolygonVertices>();
	m_texCoords = new hdTypedefList<hdVec2, kMaxPolygonVertices>();
	m_texRepeatX = 1.0f;
	m_texRepeatY = 1.0f;
	m_isHidden = false;
}


hdPolygon::hdPolygon(hdGameWorld* gameWorld) : hdGameObject(gameWorld)
{
	m_vertices = new hdTypedefList<hdVec3, kMaxPolygonVertices>();
	m_startingVertices = new hdTypedefList<hdVec3, kMaxPolygonVertices>();
	m_texCoords = new hdTypedefList<hdVec2, kMaxPolygonVertices>();
	m_texRepeatX = 1.0f;
	m_texRepeatY = 1.0f;
	m_isHidden = false;
}


hdPolygon::~hdPolygon()
{
	if (m_vertices)
		delete m_vertices;
	
	if (m_startingVertices)
		delete m_startingVertices;
	
	if (m_texCoords)
		delete m_texCoords;
}


void hdPolygon::ResetAABB()
{
	hdAssert(m_vertices != NULL && m_startingVertices != NULL);
	hdAssert(m_vertices->GetItemCount() == m_startingVertices->GetItemCount());
	
	((hdGameObject *)this)->ResetAABB();
}


void hdPolygon::SetStartingVerticesToCurrent()
{
	hdAssert(m_vertices != NULL && m_startingVertices != NULL);
	hdAssert(m_vertices->GetItemCount() == m_startingVertices->GetItemCount());
	
	// copy current vertices to starting vertices.
	memcpy(m_startingVertices->GetItems(), m_vertices->GetItems(), sizeof(hdVec3) * kMaxPolygonVertices);
	((hdGameObject *)this)->ResetAABB();
#ifndef LEVEL_EDITOR
	this->ResetOBB();
#endif
}


// Sets current vertices to starting...
void hdPolygon::ResetVertices()
{
	hdAssert(m_vertices != NULL && m_startingVertices != NULL);
	hdAssert(m_vertices->GetItemCount() == m_startingVertices->GetItemCount());
	
	// copy current vertices to starting vertices.
	memcpy(m_vertices->GetItems(), m_startingVertices->GetItems(), sizeof(hdVec3) * kMaxPolygonVertices);
	((hdGameObject *)this)->ResetAABB();
	this->ResetOBB();
}


void hdPolygon::ResetTextureCoords()
{
	this->ResetTextureCoords(m_aabb);
}


// rotate polygon into a plane, then
// work out from there.
void hdPolygon::ResetTextureCoords2()
{
	hdVec3 v;
	
	m_texCoords->RemoveAll();
	
	// Recalculate texture coords.
	hdVec3 box = m_aabb.upper - m_aabb.lower;
	
	float longest = hdMax(box.x, box.y); // longest side
	longest = hdMax(longest, box.z);
	
	if (this->GetVertexCount() == 4)
	{
		m_texCoords->Add(hdVec2(0, m_texRepeatY));
		m_texCoords->Add(hdVec2(0, 0));
		m_texCoords->Add(hdVec2(m_texRepeatX, 0));
		m_texCoords->Add(hdVec2(m_texRepeatX, m_texRepeatY));
	}
}


void hdPolygon::ResetTextureCoords(const hdAABB& aabb)
{
	float x, y;
	hdVec3 v;
	
	m_texCoords->RemoveAll();
	
	// Recalculate texture coords.
	hdVec3 box = aabb.upper - aabb.lower;
	if (box.x == 0.0 || box.y == 0.0) return;
	
#if 1
	// Scale the texture to the longest
	// side of the aabb, but clip it to the
	// aabb (unstretched).
	
	float longest = hdMax(box.x, box.y); // longest side
	
	for (int i = 0; i < this->GetVertexCount(); ++i)
	{
		v = m_vertices->GetItems()[i];
		x = m_texRepeatX * ((v.x - aabb.lower.x)/longest);
		y = m_texRepeatY * (1.0f - ((v.y - aabb.lower.y)/longest));
		
		m_texCoords->Add(hdVec2(x, y));
	}
	
#else
	
	for (int i = 0; i < this->GetVertexCount(); ++i)
	{
		v = m_vertices->GetItems()[i];
		x = m_texRepeatX * ((v.x - aabb.lower.x)/box.x);
		y = m_texRepeatY * (1.0f - ((v.y - aabb.lower.y)/box.y));
		
		m_texCoords->Add(hdVec2(x, y));
	}
#endif
}


hdVec2 hdPolygon::GetTextureCoordForPoint(const hdAABB& aabb, const hdVec3& point)
{
	float x, y;
	hdVec3 box = aabb.upper - aabb.lower;
	if (box.x == 0.0 || box.y == 0.0) return hdVec2(NULL, NULL);
	
	float longest = hdMax(box.x, box.y); // longest side
		
	x = m_texRepeatX * ((point.x - aabb.lower.x)/longest);
	y = m_texRepeatY * (1.0f - ((point.y - aabb.lower.y)/longest));
		
	return hdVec2(x, y);
}



void hdPolygon::ResetOBB()
{
	m_obb.transform.translation = this->GetWorldCenter();
	m_obb.transform.rotation.Set(0,0,0);
} 


void hdPolygon::SetDimensions(int x, int y, int w, int h)
{
	this->SetAs2DBox(x, y, w, h);
}


void hdPolygon::SetAs2DBox(const float& x, const float& y, const float& w, const float& h)
{
	this->SetAs2DBox(hdVec3(x,y,0), hdVec3(x+w, y+h, 0));
}


void hdPolygon::SetAs2DBox(const hdVec2& aa, const hdVec2& bb)
{
	m_vertices->RemoveAll();
	m_startingVertices->RemoveAll();
	
	this->AddPoint(aa.x, aa.y);
	this->AddPoint(aa.x, bb.y);
	this->AddPoint(bb.x, bb.y);
	this->AddPoint(bb.x, aa.y);
	((hdGameObject *)this)->ResetAABB();
	this->ResetTextureCoords();
}


void hdPolygon::MoveTo(const hdVec3& center)
{
	hdVec3 current = this->GetWorldCenter();
	
	hdVec3 distanceToDest = center;
	
	// distance between the destination and the current game object location.
	distanceToDest -= current;
	
	hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), distanceToDest);
	
	this->GetTransform().translation = current+distanceToDest;
	this->ResetAABB(); 
}


void hdPolygon::SetAs2DCylinder(const hdVec2& aa, const hdVec2& bb, const float radius, const int segments)
{
	m_vertices->RemoveAll();
	m_startingVertices->RemoveAll();
	
	hdVec2 center = aa + (0.5 * (bb - aa));
	
	const float k_increment = 2.0f * hd_pi / segments;
	float theta = 0.0f;
	
	for (int i = 0; i < segments; ++i)
	{
		hdVec2 v = center + radius * hdVec2(cosf(theta), sinf(theta));
		this->AddPoint(v.x, v.y);
		theta -= k_increment;
	}
	((hdGameObject *)this)->ResetAABB();
	this->ResetTextureCoords();
}


#define CIRCLE_SEGMENTS 5
void hdPolygon::SetAs2DStar(const hdVec2& aa, const hdVec2& bb, 
							const float radius, const float innerRadius,
							const unsigned int circleSegments)
{
	m_vertices->RemoveAll();
	m_startingVertices->RemoveAll();
	
	hdVec2 center = aa + (0.5 * (bb - aa));
	
	const float k_increment = (2.0f * hd_pi) / circleSegments;
	float theta = 0.0f;
	
	this->AddPoint(center.x, center.y);
	for (unsigned int i = 0; i < circleSegments; ++i)
	{
		hdVec2 v = center + radius * hdVec2(sinf(theta), cosf(theta));
		
		hdVec2 inner = center + innerRadius * 
						hdVec2(sinf(theta+(0.5f * k_increment)), 
							   cosf(theta+(0.5f * k_increment)));
		
		this->AddPoint(v.x, v.y);
		this->AddPoint(inner.x, inner.y);
		theta += k_increment;
	}
	hdVec2 endVertex = center + radius * hdVec2(sinf(0.0), cosf(0.0));
	this->AddPoint(endVertex.x, endVertex.y);
	((hdGameObject *)this)->ResetAABB();
	this->ResetTextureCoords();
}


const int hdPolygon::AddPoint(const float x, const float y)
{
	return this->AddPoint(hdVec3(x, y, 0.0f));
}


const int hdPolygon::AddPoint(const float x, const float y, const float z)
{
	return this->AddPoint(hdVec3(x, y, z));
}


const int hdPolygon::AddPoint(const hdVec3& vertex)
{
	int idx1 = m_startingVertices->Add(vertex);
	int idx2 = m_vertices->Add(vertex);
	hdAssert(idx1 == idx2);
	return idx1;
}


const bool hdPolygon::RemovePoint(const int index)
{
	bool b1 = m_startingVertices->RemoveAtIndex(index);
	bool b2 = m_vertices->RemoveAtIndex(index);
	hdAssert(b1 == b2);
	return b1;
}


void hdPolygon::RemoveAllPoints()
{
	m_startingVertices->RemoveAll();
	m_vertices->RemoveAll();
}


void hdPolygon::Rotate(const hdVec3& rotationVector, const hdVec3& rotationAxis)
{
	hdTranslateVertices(m_vertices->GetItems(), 
						m_vertices->GetItemCount(), 
						-rotationAxis);
	
	hdRotateVertices(m_vertices->GetItems(), 
					 m_vertices->GetItemCount(), 
					 rotationVector);
	
	hdTranslateVertices(m_vertices->GetItems(), 
						m_vertices->GetItemCount(), 
						rotationAxis);
	
	this->ResetAABB();
	this->SetStartingVerticesToCurrent();
}


void hdPolygon::Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis)
{	
	hdTranslateVertices(m_vertices->GetItems(), 
						m_vertices->GetItemCount(), 
						-scaleAxis);
	
	hdScaleVertices(m_vertices->GetItems(), 
					 m_vertices->GetItemCount(), 
					 scaleVector);
	
	hdTranslateVertices(m_vertices->GetItems(), 
						m_vertices->GetItemCount(), 
						scaleAxis);
	
	this->ResetAABB();
	this->SetStartingVerticesToCurrent();
}
