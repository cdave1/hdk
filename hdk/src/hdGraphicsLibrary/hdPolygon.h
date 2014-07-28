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

/*
 *
 * A polygon shape that uses hdDrawable and hdGameObject for
 * storage. It is an abstract class that stores vertex and
 * texture coordinates, but doesn't bother with drawing or
 * handling texture drawing. Main reason for no texture handling
 * is that many classes want vertices and tex coords taken care of,
 * but will use textures in their own way (example: animated sprites
 * and buttons).
 *
 * Texture coordinates are automatically calculated and then 
 * multiplied by the x and y repeat factors.
 *
 */

#ifndef _HD_POLYGON_H_
#define _HD_POLYGON_H_

#include "hdGame.h"
#include "hdCollections.h"

//#define kMaxPolygonVertices 128
#define kMaxPolygonVertices 96


class hdPolygon : public hdGameObject
{
public:
	hdPolygon();
	
	hdPolygon(hdGameWorld* gameWorld);
	
	virtual ~hdPolygon();
	
	void SetDimensions(int x, int y, int w, int h);
	
	void SetAs2DBox(const float& x, const float& y, const float& w, const float& h);
	
	void SetAs2DBox(const hdVec2& aa, const hdVec2& bb);
	
	void SetAs2DBox(const hdVec3& aa, const hdVec3& bb);
	
	void SetAs2DCylinder(const hdVec2& aa, const hdVec2& bb, const float radius, const int segments);
	
	/*
	 * 5 point star with tips sitting on circle with radius
	 */
	void SetAs2DStar(const hdVec2& aa, const hdVec2& bb, 
					 const float radius, const float innerRadius, 
					 const unsigned int circleSegments);
		
	const int AddPoint(const float x, const float y);
	
	const int AddPoint(const float x, const float y, const float z);
	
	const int AddPoint(const hdVec3& vertex);
	
	const bool RemovePoint(const int index);
	
	void RemoveAllPoints();
	
	void ResetAABB();
	
	void ResetOBB();
	
	hdVec3* GetVertices();
	
	const int GetVertexCount() const;
	
	void Rotate(const hdVec3& rotationVector, const hdVec3& rotationAxis);
	
	void Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis);
	
	
	/*
	 * DEPRECATED: Not used.
	 */
	void ApplyTransform(const hdTransform& transform);
	
	
	
	hdVec2* GetTextureCoordinates();
	
	void SetStartingVerticesToCurrent();
	
	void ResetVertices();
	
	void ResetTextureCoords();
	
	void ResetTextureCoords(const hdAABB& aabb);
	
	void ResetTextureCoords2();
	
	const float GetTextureRepeatX() const;
	
	const float GetTextureRepeatY() const;
	
	void SetTextureRepeatX(const float repeatX);
	
	void SetTextureRepeatY(const float repeatY);
	
	void MoveTo(const hdVec3& center);
	
	hdAABB GetStartingAABB() const;
	
	hdVec3* GetStartingVertices();
	
	hdVec2 GetTextureCoordForPoint(const hdAABB& aabb, const hdVec3& point);
	
	void SetUserType(uint32 type);	
	
protected:

	void ResetGameObjectBounds();
	
	hdTypedefList<hdVec3, kMaxPolygonVertices>* m_vertices;
	
	hdTypedefList<hdVec2, kMaxPolygonVertices>* m_texCoords;
	
	hdTypedefList<hdVec3, kMaxPolygonVertices>* m_startingVertices;
	
	float m_texRepeatX;
	
	float m_texRepeatY;
	
	
private:

	friend class boost::serialization::access;
	
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		ar & boost::serialization::base_object<hdGameObject>(*this);

		int vertexCount = m_startingVertices->GetItemCount();
		//ar & m_textureName;
		ar & vertexCount;
		for (int i = 0; i < m_startingVertices->GetItemCount(); i++)
		{
			ar & m_startingVertices->GetItems()[i].x;
			ar & m_startingVertices->GetItems()[i].y;
			ar & m_startingVertices->GetItems()[i].z;
			
			if (version >= 1)
			{
				ar & m_texCoords->GetItems()[i].x;
				ar & m_texCoords->GetItems()[i].y;
			}
		}
		
		if (version >= 2)
		{
			ar & m_texRepeatX;
			ar & m_texRepeatY;
		}
	}
	
	
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<hdGameObject>(*this);
		
		int vertexCount = 0;
		//ar & m_textureName;
		ar & vertexCount;
		
		float x, y, z, s, t;
		for (int i = 0; i < vertexCount; i++)
		{
			ar & x;
			ar & y;
			ar & z;
			m_startingVertices->Add(hdVec3(x, y, z));
			m_vertices->Add(hdVec3(x,y,z));
			
			if (version >= 1)
			{
				ar & s;
				ar & t;
				m_texCoords->Add(hdVec2(s, t));
			}
		}
		
		if (version >= 2)
		{
			ar & m_texRepeatX;
			ar & m_texRepeatY;
		}
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	
	
};


BOOST_CLASS_VERSION(hdPolygon, 2)


inline void hdPolygon::SetAs2DBox(const hdVec3& aa, const hdVec3& bb)
{
	this->SetAs2DBox(hdVec3toVec2(aa), hdVec3toVec2(bb));
}


inline hdVec3* hdPolygon::GetVertices()
{
	return m_vertices->GetItems();
}


inline const int hdPolygon::GetVertexCount() const
{
	return m_vertices->GetItemCount();
}


inline void hdPolygon::ApplyTransform(const hdTransform& transform)
{
	m_obb.transform = transform;
	ResetOBB();
}


inline hdVec2* hdPolygon::GetTextureCoordinates()
{
	return m_texCoords->GetItems();
}


inline const float hdPolygon::GetTextureRepeatX() const
{
	return m_texRepeatX;
}


inline const float hdPolygon::GetTextureRepeatY() const
{
	return m_texRepeatY;
}


inline void hdPolygon::SetTextureRepeatX(const float repeatX)
{
	m_texRepeatX = repeatX;
}


inline void hdPolygon::SetTextureRepeatY(const float repeatY)
{
	m_texRepeatY = repeatY;
}


inline void hdPolygon::SetUserType(uint32 userType)
{
	m_userType = userType;
}


inline hdAABB hdPolygon::GetStartingAABB() const
{
	hdAABB startingAABB;
	hdFindAABB(m_startingVertices->GetItems(), this->GetVertexCount(), &startingAABB);
	return startingAABB;
}


inline hdVec3* hdPolygon::GetStartingVertices()
{
	return m_startingVertices->GetItems();
}

#endif
