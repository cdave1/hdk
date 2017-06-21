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

#include "totemContactManager.h"

void totemContactManager::Add(const b2ContactPoint* point)
{
    if (m_contactCount == k_maxContactPoints)
    {
        return;
    }

    ContactPoint* cp = m_contacts + m_contactCount;
    cp->body1 = point->shape1->GetBody();
    cp->body2 = point->shape2->GetBody();
    cp->position = point->position;
    cp->normal = point->normal;
    cp->id = point->id;
    cp->state = e_contactAdded;

    ++m_contactCount;
}


void totemContactManager::Persist(const b2ContactPoint* point)
{
    if (m_contactCount == k_maxContactPoints)
    {
        return;
    }

    ContactPoint* cp = m_contacts + m_contactCount;
    cp->body1 = point->shape1->GetBody();
    cp->body2 = point->shape2->GetBody();
    cp->position = point->position;
    cp->normal = point->normal;
    cp->id = point->id;
    cp->state = e_contactPersisted;

    ++m_contactCount;
}


void totemContactManager::Remove(const b2ContactPoint* point)
{
}


void totemContactManager::Result(const b2ContactResult* point)
{
}
