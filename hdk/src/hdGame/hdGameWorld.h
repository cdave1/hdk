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

#ifndef HD_GAMEWORLD_H
#define HD_GAMEWORLD_H

#include "hdMath.h"
#include "hdGame/hdGameObject.h"

class hdGameObject;

class hdGameWorld
{
public:
	// When the world is initialised it is given dimensions in terms
	// of a bounding box.
	hdGameWorld(const hdAABB& aabb);
		
	~hdGameWorld();
		
	bool AddGameObject(hdGameObject* gameObject);
		
	bool ContainsGameObject(const hdGameObject* gameObject);
	
	bool RemoveGameObject(hdGameObject* gameObject);
	
	void RemoveAllGameObjects();
		
	hdGameObject* GetGameObjectList();
		
	int32 GetGameObjectCount() const;
	
	const hdAABB GetAABB();
	
	void SetAABB(const hdAABB& aabb);
	
	void ResetAABB();
	
	
	/*
	 * Query for getting all game objects that are skewered by the ray
	 * along the vector described by point.
	 */
	const int PointQuery(const hdVec3& point, hdGameObject** objects, int maxObjects);
	
	
	/*
	 * All objects that have an aabb that overlaps the given aabb.
	 */
	const int AABBQuery(const hdAABB& aabb, hdGameObject** objects, int maxObjects);
	
	
	/*
	 
	 Return all objects that intersect the ray.
	 
	 Efficiency:
	 Make a bounding box with the ray between the origin and a point
	 along the slope that is further away than the bounds of any object.
	 Use this box as a fast scissor to quickly cull objects - any that
	 are outside the bounding box will also not need to be tested for
	 intersection with the ray.
	 
	 Sorting:
	 Ensure the top most value in object list has the highest z value.
	 
	 Future:
	 - Instead of sorting by z value, sort by proximity to camera location.
	 - Only sort by objects actually on screen, and then extend to other
	   objects iff nothing could be found.
	 
	 */
	const int RayQuery(const unsigned int objectFilter, const hdVec3* ray,
                       hdGameObject** objects, int maxObjects, hdVec3 &vOut);
	

	const int RayQuery___2(const unsigned int objectFilter, const hdVec3* ray,
                           hdGameObject** objects, int maxObjects, hdVec3 &vOut);

private:
		
	friend class hdGameObject;
		
	hdGameObject* m_gameObjectList;
		
	int32 m_gameObjectCount;
		
	hdAABB m_aabb;
		
	bool m_lock;

};


inline const hdAABB hdGameWorld::GetAABB()
{
	return m_aabb;
}


inline void hdGameWorld::SetAABB(const hdAABB& aabb)
{
	m_aabb = aabb;
}


inline int32 hdGameWorld::GetGameObjectCount() const
{
	return m_gameObjectCount;
}

#endif
