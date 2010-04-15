/*
 *  hdPhysicsJoint.h
 *  LevelEditor
 *
 *  Created by david on 29/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_PHYSICS_JOINT_H
#define HD_PHYSICS_JOINT_H

#include "Box2D.h"

class b2World;
class b2Body;

class hdPhysicsJoint
	{
	public:
		hdPhysicsJoint() { m_physicsEnabled = true; m_joint = NULL; }
		
		virtual void CreateJoint() = 0;
		
		const b2Joint* GetPhysicsJoint() const;
		
	protected:
		b2World* m_physicsWorld;
		
		b2Joint* m_joint;
		
		bool m_physicsEnabled;
		
	};

inline const b2Joint* hdPhysicsJoint::GetPhysicsJoint() const
{
	return m_joint;
}

#endif