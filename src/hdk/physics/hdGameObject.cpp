//
//  hdGameObject.mm
//  AnimationEngine
//
//  Created by david on 14/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#include "hdGameObject.h"
#include <hdk/physics/hdGameWorld.h>

hdGameObject::hdGameObject()
{
    m_isHidden = false;
    m_userType = 0;
    m_tag = 0;
    m_alpha = 1.0f;
    m_world = NULL;
}


hdGameObject::hdGameObject(hdGameWorld* world)
{
    m_userType = 0;
    m_tag = 0;
    m_isHidden = false;
    if (world != NULL)
    {
        m_world = world;
        world->AddGameObject(this);
    }
    else
    {
        m_world = NULL;
    }
    m_alpha = 1.0f;
}


hdGameObject::hdGameObject(hdGameWorld* world, const hdAABB& aabb)
{
    m_alpha = 1.0f;
    m_tag = 0;
    m_isHidden = false;
    if (world != NULL)
    {
        m_world = world;
        world->AddGameObject(this);
    }
    else
    {
        m_world = NULL;
    }

    m_aabb = aabb;

    m_obb.vertices[0].Set(m_aabb.lower.x, m_aabb.lower.y, 0.0f);
    m_obb.vertices[1].Set(m_aabb.lower.x, m_aabb.upper.y, 0.0f);
    m_obb.vertices[2].Set(m_aabb.upper.x, m_aabb.upper.y, 0.0f);
    m_obb.vertices[3].Set(m_aabb.upper.x, m_aabb.lower.y, 0.0f);

    // It's really rather silly that the scale transform is part of the obb
    // and not part of the game object itself.
    m_obb.transform.scale.Set(1.0f, 1.0f, 1.0f);
}


hdGameObject::~hdGameObject()
{
    //if (m_world != NULL)
    //{
    //	m_world->RemoveGameObject(this);
    //}
}


void hdGameObject::Init(hdGameWorld* world)
{
    m_world = world;
    world->AddGameObject(this);
}


hdGameObjectDef hdGameObject::GetCurrentDefinition() const
{
    hdGameObjectDef def;
    return def;
}


void hdGameObject::ResetAABB()
{
    hdFindAABB(this->GetVertices(), this->GetVertexCount(), &m_aabb);
}


bool hdGameObject::AABBIntersection(const hdAABB& aabb)
{
    return hdAABBIntersection(aabb, this->GetAABB());
}


bool hdGameObject::RayIntersection(const hdVec3* ray, hdVec3& vOut)
{
    return hdRayIntersectsVertices(this->GetVertices(), this->GetVertexCount(), this->GetWorldCenter(), ray, vOut);
}


void hdGameObject::Translate(const hdVec3& translationVector, const hdVec3& translationAxis)
{
    hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), translationVector);
    this->GetTransform().translation = translationAxis + translationVector;
    this->ResetAABB();
}


/*
 #include <new>


 hdGameObject* hdGameObject::Create(const hdGameObjectDef* def, hdGameWorld* world)
 {
	switch(def->type)
	{
 case e_gameObject:
 {
 //m_gameObjectCount++;
 void* mem = hdAlloc(sizeof(hdGameObject));
 return new (mem) hdGameObject(def, world);
 }
 default:
 return NULL;
	}
 }


 void hdGameObject::Destroy(hdGameObject* gameObject)
 {
	switch(gameObject->GetType())
	{
 case e_gameObject:
 {
 gameObject->~hdGameObject();
 hdFree(gameObject);
 break;
 }
 default:
 hdAssert(false);
	}
 }
 */
