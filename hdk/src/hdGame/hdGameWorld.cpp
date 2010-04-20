//
//  hdGameWorld.mm
//  AnimationEngine
//
//  Created by david on 14/02/09.
//  Copyright 2009 n/a. All rights reserved.
//



#include "hdGameWorld.h"


hdGameWorld::hdGameWorld(const hdAABB& aabb)
{
	m_aabb = aabb;
	
	
	
	m_gameObjectCount = 0;
	m_gameObjectList = NULL;
	m_lock = false;
}


hdGameWorld::~hdGameWorld() {}


bool hdGameWorld::AddGameObject(hdGameObject* gameObject)
{
	hdAssert(m_lock == false);
	if (m_lock == true)
	{
		return false;
	}
	
	// What if we already have the game object?
	if (this->ContainsGameObject(gameObject)) return false;
	
	hdPrintf("Adding Game Object: %d Type: %d\n", (int)gameObject, gameObject->GetUserType());
	
	gameObject->m_prev = NULL;
	gameObject->m_next = m_gameObjectList;
	if (m_gameObjectList)
	{
		m_gameObjectList->m_prev = gameObject;
	}
	m_gameObjectList = gameObject;
	
	++m_gameObjectCount;
	
	return true;
}


bool hdGameWorld::ContainsGameObject(const hdGameObject* gameObject)
{
	hdGameObject* obj = m_gameObjectList;
	while (obj != NULL) 
	{
		if (obj == gameObject) break;
		obj = obj->m_next;
	}
	return (obj != NULL);
}


bool hdGameWorld::RemoveGameObject(hdGameObject* gameObject)
{
	
	hdAssert(m_lock == false);	
		if (m_lock == true)
	{
		return false;
	}

	// What if the pointer is retained somewhere else?
	// Well, we fucking have to deal with it don't we!
	// Makes removing, on average, an O(nlogn) algo rather
	// than O(n). Fuck.
	if (!this->ContainsGameObject(gameObject)) return false;
	
	hdAssert(m_gameObjectCount > 0);
	
	hdPrintf("Removing Game Object: %d\n", (int)gameObject);
	
	if (gameObject->m_prev)
	{
		gameObject->m_prev->m_next = gameObject->m_next;
	}
	
	if (gameObject->m_next)
	{
		gameObject->m_next->m_prev = gameObject->m_prev;
	}
	
	if (gameObject == m_gameObjectList)
	{
		m_gameObjectList = gameObject->m_next;
	}
	
	gameObject->SetWorld(NULL);
	
	--m_gameObjectCount;
	return true;
}



// For a given point, find all the game objects that contain 
// that point.
const int hdGameWorld::PointQuery(const hdVec3& point, hdGameObject** objects, int maxObjects)
{
	int count = 0;
	
	hdGameObject* object = m_gameObjectList;
	
	while (object != NULL)
	{
		/*
		hdVec3 vertices[4] = { 
			hdVec3(object->GetAABB().lower.x, object->GetAABB().lower.y, 0.0), 
			hdVec3(object->GetAABB().lower.x, object->GetAABB().upper.y, 0.0),
			hdVec3(object->GetAABB().upper.x, object->GetAABB().upper.y, 0.0),
			hdVec3(object->GetAABB().upper.x, object->GetAABB().lower.y, 0.0)
		};
		
		if (hdPolygonContainsPoint(vertices, 4, point))
		{
			objects[(count++)%maxObjects] = object;
		}*/
		if (hdPointInsideAABB(point, object->GetAABB()))
		{
			objects[(count++)%maxObjects] = object;
		}
		
		object = object->m_next;
	}
	return count;
}


const int hdGameWorld::AABBQuery(const hdAABB& aabb, hdGameObject** objects, int maxObjects)
{
	int count = 0;
	
	hdGameObject* object = m_gameObjectList;
	
	while (object != NULL)
	{
		if (object->AABBIntersection(aabb))
		{
			objects[(count++)%maxObjects] = object;
		}
		
		object = object->m_next;
	}
	return count;
}



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
 Instead of sorting by z value, sort by proximity to camera location.
 
 */
const int hdGameWorld::RayQuery(const unsigned int objectFilter, const hdVec3* ray, hdGameObject** objects, int maxObjects, hdVec3 &vOut)
{
	int count = 0;
	hdAABB rayAABB;
	hdVec3 maxZIntersection(0,0,-100.0f);
	hdGameObject* tmp;
	
	rayAABB.lower.Set(ray[1].x - 1.0f, ray[1].y - 1.0f, -20.0f); 
	rayAABB.upper.Set(ray[1].x + 1.0f, ray[1].y + 1.0f, 20.0f); 
	
	hdGameObject* object = m_gameObjectList;
	
	while (object != NULL)
	{
		if (count == maxObjects) break;

		if ((((unsigned int)object->GetUserType() & objectFilter) == (unsigned int)object->GetUserType()) 
			&& object->GetUserType() != 0
			&& object->AABBIntersection(rayAABB))
		{
			if (object->RayIntersection(ray, vOut))
			{
				objects[count] = object;
				++count;
				
				// Remember: we only want to make sure
				// that the top most item is the nearest.
				if (vOut.z > maxZIntersection.z)
				{
					if (count >= 2)
					{
						// Move each item down to make way for the new big cheese.
						tmp = objects[count-1];
						for(int i = count - 2; i >= 0 && i < maxObjects; i--)
						{
							objects[i+1] = objects[i];
						}
						objects[0] = tmp;
					}
					maxZIntersection.Set(vOut.x, vOut.y, vOut.z);
				}
			}
		}
		object = object->m_next;
	}
	vOut.Set(maxZIntersection.x, maxZIntersection.y, maxZIntersection.z);
	return count;
}



const int hdGameWorld::RayQuery___2(const unsigned int objectFilter, const hdVec3* ray, hdGameObject** objects, int maxObjects, hdVec3 &vOut)
{
	int count = 0;
	hdAABB rayAABB;
	hdVec3 maxZIntersection(0,0,-100.0f);
	hdGameObject* tmp;
	
	rayAABB.lower.Set(ray[1].x - 1.0f, ray[1].y - 1.0f, -20.0f); 
	rayAABB.upper.Set(ray[1].x + 1.0f, ray[1].y + 1.0f, 20.0f); 
	
	hdGameObject* object = m_gameObjectList;
	
	while (object != NULL)
	{
		if (count == maxObjects) break;
		
		if ((((unsigned int)object->GetUserType() & objectFilter) == (unsigned int)object->GetUserType()) 
			&& object->GetUserType() != 0)
		{
			if (object->RayIntersection(ray, vOut))
			{
				objects[count] = object;
				++count;
				
				// Remember: we only want to make sure
				// that the top most item is the nearest.
				if (vOut.z > maxZIntersection.z)
				{
					if (count >= 2)
					{
						// Move each item down to make way for the new big cheese.
						tmp = objects[count-1];
						for(int i = count - 2; i >= 0 && i < maxObjects; i--)
						{
							objects[i+1] = objects[i];
						}
						objects[0] = tmp;
					}
					maxZIntersection.Set(vOut.x, vOut.y, vOut.z);
				}
			}
		}
		object = object->m_next;
	}
	vOut.Set(maxZIntersection.x, maxZIntersection.y, maxZIntersection.z);
	return count;
}



void hdGameWorld::RemoveAllGameObjects()
{	
	hdGameObject* obj = m_gameObjectList;
	while (obj != NULL) 
	{
		obj->SetWorld(NULL);
		obj = obj->m_next;
	}
	m_gameObjectCount = 0;
	m_gameObjectList = NULL;
}


/*
hdGameWorld::hdGameWorld(const hdVec3& boundingBox, const hdVec3& projection)
{
	m_boundingBox = boundingBox;
	
	hdGameObjectDef def;
	def.position = hdVec3(0, 0, 0);
	def.boundingBox = projection;
	def.angle = hdVec3(0, 0, 0);
	def.type = e_gameObject;
	
	m_projection = hdGameObject::Create(&def, NULL);
	
	m_gameObjectCount = 0;
	m_gameObjectList = NULL;
	m_lock = false;
}


hdGameWorld::~hdGameWorld()
{
	m_projection->~hdGameObject();
	hdFree(m_projection);
}


hdGameObject* hdGameWorld::CreateGameObject(const hdGameObjectDef* def)
{
	hdAssert(m_lock == false);
	if (m_lock == true)
	{
		return NULL;
	}

	hdGameObject* gameObject = hdGameObject::Create(def, this);
	
	gameObject->m_prev = NULL;
	gameObject->m_next = m_gameObjectList;
	if (m_gameObjectList)
	{
		m_gameObjectList->m_prev = gameObject;
	}
	m_gameObjectList = gameObject;
	
	++m_gameObjectCount;
	
	return gameObject;
}


void hdGameWorld::DestroyGameObject(hdGameObject* gameObject)
{
	hdAssert(m_gameObjectCount > 0);
	hdAssert(m_lock == false);
	if (m_lock == true)
	{
		return;
	}
	
	if (gameObject->m_prev)
	{
		gameObject->m_prev->m_next = gameObject->m_next;
	}
	
	if (gameObject->m_next)
	{
		gameObject->m_next->m_prev = gameObject->m_prev;
	}
	
	if (gameObject == m_gameObjectList)
	{
		m_gameObjectList = gameObject->m_next;
	}
	
	--m_gameObjectCount;
	hdGameObject::Destroy(gameObject);
}


// For a given point, find all the game objects that contain 
// that point.
const int hdGameWorld::PointQuery(const hdVec2& point, hdGameObject** objects, int maxObjects)
{
	void **res = (void **)malloc(sizeof(void *) * maxObjects);
	
	int count = 0;
	
	hdGameObject* object = m_gameObjectList;
	
	while (object != NULL)
	{
		object = object->m_next;
	}
	
	for (int i = 0; i < count; i++)
	{
		objects[i] = (hdGameObject*)res[i];
	}
	free(res);
	return count;
	
}

*/