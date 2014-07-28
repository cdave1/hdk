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