/*
 *  GameObject.cpp
 *  LevelEditor
 *
 *  Created by david on 27/03/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "GameObject.h"

GameObject::GameObject() 
{
	m_gameObject = NULL;
	m_def = hdGameObjectDef();
}


void GameObject::Init(hdGameWorld *gameWorld, 
					  const hdGameObjectDef def)
{
	if (m_gameObject == NULL)
	{
		m_gameWorld = gameWorld;
	
		// Create the game object:
		m_gameObject = m_gameWorld->CreateGameObject(&def);
	}
}



GameObject::~GameObject()
{
	m_gameWorld->DestroyGameObject(m_gameObject);
}


void GameObject::DestroyGameObject()
{
	if (m_gameObject != NULL)
	{
		m_gameWorld->DestroyGameObject(m_gameObject);
		m_gameObject = NULL;
	}
}


hdGameObjectDef GameObject::GetCurrentDefinition() const
{
	hdGameObjectDef def;
	def.position.Set(m_gameObject->GetPosition().x,
					 m_gameObject->GetPosition().y,
					 m_gameObject->GetPosition().z);
	def.boundingBox.Set(m_gameObject->GetBoundingBox().x,
						m_gameObject->GetBoundingBox().y,
						m_gameObject->GetBoundingBox().z);
	def.angle.Set(m_gameObject->GetAngle().x,
				  m_gameObject->GetAngle().y,
				  m_gameObject->GetAngle().z);
	
	def.type = m_gameObject->GetType();
	def.name = m_gameObject->GetName();
	return def;
}

