/*
 *  hdPhysicsObject.h
 *  AnimationEngine
 *
 *  Created by david on 31/03/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_PHYSICSOBJECT_H
#define HD_PHYSICSOBJECT_H

#include "Box2D.h"

class b2World;
class b2Body;

class hdPhysicsObject 
{
public:
	hdPhysicsObject() { m_physicsEnabled = true; m_physicsBody = NULL; }
	
	void DestroyPhysicsBody() 
	{
		//if (m_physicsUserObject != NULL) delete m_physicsUserObject;
		if (m_physicsBody != NULL) 
		{
			hdPrintf("Destroying physics body\n");
			m_physicsWorld->DestroyBody(m_physicsBody);
			m_physicsBody = NULL;
		}
	}
	
	void ResetPhysicsBody()
	{
		this->DestroyPhysicsBody();
		this->CreatePhysicsBody();
	}
	
	
	const b2Body* GetPhysicsBody()
	{
		return m_physicsBody;
	}
	
	
protected:
	b2World* m_physicsWorld;
	
	b2Body* m_physicsBody;
	
	//totemGameObject *m_physicsUserObject;
	
	virtual void CreatePhysicsBody() = 0;
	
	bool m_physicsEnabled;

};


#endif