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

#ifndef _HDK_CONTACT_MANAGER_H_
#define _HDK_CONTACT_MANAGER_H_

#include "Box2D.h"

const int32 k_maxContactPoints = 1024;

enum ContactState
{
    e_contactAdded,
    e_contactPersisted,
    e_contactRemoved,
};

struct ContactPoint
{
    b2Body* body1;
    b2Body* body2;
    b2Vec2 normal;
    b2Vec2 position;
    b2Vec2 velocity;
    b2ContactID id;
    ContactState state;
};


class totemContactManager : public b2ContactListener
{
public:
    totemContactManager() {}

    ~totemContactManager() {}

    void ResetContactPoints();

    // b2Contact Listener functions
    void Add(const b2ContactPoint* point);

    void Persist(const b2ContactPoint* point);

    void Remove(const b2ContactPoint* point);

    void Result(const b2ContactResult* point);

protected:

    ContactPoint m_contacts[k_maxContactPoints];
    
    int32 m_contactCount;
};


inline void totemContactManager::ResetContactPoints()
{
    m_contactCount = 0;
}

#endif
