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

#include "totemJack.h"

#include "Box2d.h"

totemJack::totemJack() : hdPolygon()
{
    m_userType = (int)e_totemTypeJack;
    m_physicsBody = NULL;

    m_texture = hdTextureManager::Instance()->FindTexture("/Textures/jack01.tga", TT_Wall);

    m_colorTint[0] = 1.0f;
    m_colorTint[1] = 1.0f;
    m_colorTint[2] = 0.1f;
    m_colorTint[3] = 1.0f;
}


totemJack::totemJack(hdGameWorld *gameWorld,
                     b2World *physicsWorld,
                     const hdVec2& position) : hdPolygon(gameWorld)
{
    m_userType = (int)e_totemTypeJack;
    m_physicsBody = NULL;
    m_aabb.lower = hdVec2toVec3(position);
    m_aabb.upper = m_aabb.lower + hdVec3(0.32f, 0.32f, 0.0f);

    this->SetAs2DBox(m_aabb.lower, m_aabb.upper);

    m_texture = hdTextureManager::Instance()->FindTexture("/Textures/jack01.tga", TT_Wall);

    m_colorTint[0] = 1.0f;
    m_colorTint[1] = 1.0f;
    m_colorTint[2] = 0.1f;
    m_colorTint[3] = 1.0f;

    this->Init(gameWorld, physicsWorld);
}


void totemJack::Init(hdGameWorld *gameWorld,
                     b2World *physicsWorld)
{
    ((hdPolygon *)this)->Init(gameWorld);
    m_userType = (int)e_totemTypeJack;
    m_physicsWorld = physicsWorld;

    this->ResetAABB();
    this->ResetOBB();
    this->CreatePhysicsBody();
}


totemJack::~totemJack()
{
    this->DestroyPhysicsBody();
}


void totemJack::Step()
{
    if (m_physicsBody == NULL) return;

    b2XForm xform = m_physicsBody->GetXForm();

    hdTransform transform;

    // Translate to origin, rotate, translate back again.
    hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -m_obb.transform.translation);
    hdRotateVertices(this->GetVertices(), this->GetVertexCount(), hdVec3(0.0f, 0.0f, m_physicsBody->GetAngle()) - m_obb.transform.rotation);
    hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), m_obb.transform.translation + (hdVec3(xform.position.x, xform.position.y, 0.0f) - m_obb.transform.translation));

    m_obb.transform.translation = hdVec3(xform.position.x, xform.position.y, 0.0f);
    m_obb.transform.rotation =  hdVec3(0.0f, 0.0f, m_physicsBody->GetAngle());

    this->ResetAABB();
}


void totemJack::CreatePhysicsBody()
{
    hdVec3 box = (m_aabb.upper - m_aabb.lower);
    hdVec3 center = m_aabb.lower + (0.5 * box);
    b2PolygonShape *shape = NULL;

    hd2DSingleShapePhysicsBody(m_physicsWorld, &m_physicsBody, &shape, center,
                               this->GetVertices(), this->GetVertexCount(),
                               false, false, 0, 0, 0);

    m_physicsBody->SetUserData((hdGameObject *)this);
}


void totemJack::Draw() const
{
    DrawPolygonWithTint((hdPolygon *)this, m_texture, m_colorTint, m_alpha);

#ifdef LEVEL_EDITOR
    hdglColor4f(0.0f, 0.0f, 0.0f, 1.0f);
    hdglBegin(GL_LINE_LOOP);
    for (int i = 0; i < this->GetVertexCount(); i++)
    {
        hdglVertex2f(m_vertices->GetItems()[i].x, m_vertices->GetItems()[i].y);
    }
    hdglEnd();
    
    hdglBegin(GL_LINE_LOOP);
    hdglVertex2f(m_aabb.lower.x, m_aabb.lower.y);
    hdglVertex2f(m_aabb.lower.x, m_aabb.upper.y);
    hdglVertex2f(m_aabb.upper.x, m_aabb.upper.y);
    hdglVertex2f(m_aabb.upper.x, m_aabb.lower.y);
    hdglEnd();
#endif
}
