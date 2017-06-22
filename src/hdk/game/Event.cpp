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

#include "Event.h"

Event::Event() : hdPolygon()
{
    m_userType = (int)e_hdkTypeEvent;
    m_firstObject = NULL;
    m_secondObject = NULL;
}


Event::Event(hdGameWorld *gameWorld,
                       e_hdkEventType eventType,
                       hdGameObject* firstObject,
                       hdGameObject* secondObject) : hdPolygon(gameWorld)
{
    m_userType = (int)e_hdkTypeEvent;
    m_eventType = eventType;

    m_firstObject = NULL;
    m_secondObject = NULL;

    this->SetFirstObject(firstObject);
    this->SetSecondObject(secondObject);

    m_aabb.lower = firstObject->GetWorldCenter() - hdVec3(0.2f, 0.2f, 0.0f);
    m_aabb.upper = firstObject->GetWorldCenter() + hdVec3(0.2f, 0.2f, 0.0f);
    this->SetAs2DBox(m_aabb.lower, m_aabb.upper);

    switch (eventType) {
        case e_hdkEventTypeFireGun:
            m_colorTint[0] = 0.0f;
            m_colorTint[1] = 0.75f;
            m_colorTint[2] = 0.0f;
            m_colorTint[3] = 0.5f;
            break;
        case e_hdkEventTypeShowMessage:
            m_colorTint[0] = 0.0f;
            m_colorTint[1] = 0.6f;
            m_colorTint[2] = 1.0f;
            m_colorTint[3] = 0.5f;
            break;
        default:
            m_colorTint[0] = 1.0f;
            m_colorTint[1] = 0.6f;
            m_colorTint[2] = 0.0f;
            m_colorTint[3] = 0.5f;
            break;
    }
}


Event::~Event()
{
}


void Event::Init(hdGameWorld *gameWorld)
{
    ((hdPolygon *)this)->Init(gameWorld);

    if (m_firstObject != NULL)
    {
        m_aabb.lower = m_firstObject->GetWorldCenter() - hdVec3(0.2f, 0.2f, 0.0f);
        m_aabb.upper = m_firstObject->GetWorldCenter() + hdVec3(0.2f, 0.2f, 0.0f);
        this->SetAs2DBox(m_aabb.lower, m_aabb.upper);
    }

    this->ResetAABB();
    this->ResetOBB();
}


const int Event::SetFirstObject(const hdGameObject* body)
{
    if (body == m_secondObject) return -1;
    if (body == NULL) return -1;
    m_firstObject = (hdGameObject*)body;
    return 0;
}


const int Event::SetSecondObject(const hdGameObject* body)
{
    if (body == m_firstObject) return -1;
    if (body == NULL) return -1;
    m_secondObject = (hdGameObject*)body;
    return 0;
}


void Event::Step()
{
}


#ifdef LEVEL_EDITOR
#include <hdk/hdk.h>

bool Event::AABBIntersection(const hdAABB& aabb)
{
    hdAABB depthAABB;
    if (m_secondObject == NULL)
    {
        depthAABB.lower.Set(m_aabb.lower.x, m_aabb.lower.y, m_aabb.lower.z - 0.05f);
        depthAABB.upper.Set(m_aabb.upper.x, m_aabb.upper.y, m_aabb.upper.z + 0.05f);
    }
    else
    {
        depthAABB.lower = hdMin(m_firstObject->GetAABB().lower, m_secondObject->GetAABB().lower);
        depthAABB.upper = hdMax(m_firstObject->GetAABB().upper, m_secondObject->GetAABB().upper);
    }

    return hdAABBIntersection(aabb, depthAABB);
}
#endif



void Event::Draw() const
{
#ifdef LEVEL_EDITOR
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 1.0f);

    if (m_secondObject != NULL)
    {
        ((hdPolygon*)this)->SetAs2DCylinder(hdVec3toVec2(m_secondObject->GetWorldCenter() - hdVec3(0.06f, 0.06f, 0.0f)),
                                            hdVec3toVec2(m_secondObject->GetWorldCenter() + hdVec3(0.06f, 0.06f, 0.0f)),
                                            0.06f, 12);

        DrawPolygonWithTint((hdPolygon *)this, NULL, m_colorTint, m_alpha);
    }

    if (m_firstObject != NULL)
    {
        ((hdPolygon*)this)->SetAs2DCylinder(hdVec3toVec2(m_firstObject->GetWorldCenter() - hdVec3(0.2f, 0.2f, 0.0f)),
                                            hdVec3toVec2(m_firstObject->GetWorldCenter() + hdVec3(0.2f, 0.2f, 0.0f)),
                                            0.2f, 12);

        DrawPolygonWithTint((hdPolygon *)this, NULL, m_colorTint, m_alpha);
    }

    if (m_firstObject != NULL && m_secondObject != NULL)
    {
        hdglBegin(GL_LINES);
        hdglColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        hdglVertex2f(m_firstObject->GetWorldCenter().x,  m_firstObject->GetWorldCenter().y);
        hdglVertex2f(m_secondObject->GetWorldCenter().x, m_secondObject->GetWorldCenter().y);
        hdglEnd();
    }
    glPopMatrix();
#endif
}
