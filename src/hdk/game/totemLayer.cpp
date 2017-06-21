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

#include "totemLayer.h"


totemLayer::totemLayer()
{
    m_polygons = NULL;
    m_scale = 100;
    m_scaleFactor = 1.0f;

#ifdef LEVEL_EDITOR
    m_polygons = new hdSerializablePointerList<totemPolygon, kMaxLayerPolygons>();
#endif
}


totemLayer::totemLayer(const totemLevel* level, const int scale)
{
    m_polygons = NULL;
    m_level = (totemLevel *)level;
    m_scale = scale;
    m_scaleFactor = m_scale / 100.0f;

#ifdef LEVEL_EDITOR
    m_polygons = new hdSerializablePointerList<totemPolygon, kMaxLayerPolygons>();
#endif
}


totemLayer::totemLayer(hdGameWorld* world, const totemLevel* level, const int scale, const hdOrthographicProjection* relativeProjection) : hdGameObject(world)
{
    m_polygons = NULL;
    m_level = (totemLevel *)level;
    m_scale = scale;
    m_scaleFactor = m_scale / 100.0f;
    m_relativeProjection = (hdOrthographicProjection *)relativeProjection;

#ifdef LEVEL_EDITOR
    m_polygons = new hdSerializablePointerList<totemPolygon, kMaxLayerPolygons>();
#endif
}


totemLayer::~totemLayer()
{
    hdPrintf("Deleting totemLayer\n");
    m_polygons->RemoveAll();
    delete m_polygons;
}


void totemLayer::DestroyGameObjects()
{
    for (int i = 0; i < m_polygons->GetItemCount(); i++)
    {
        m_world->RemoveGameObject(m_polygons->GetItems()[i]);
    }
}


void totemLayer::Init(hdGameWorld* world, const hdOrthographicProjection* relativeProjection)
{
    ((hdGameObject *)this)->Init(world);
    m_relativeProjection = (hdOrthographicProjection *)relativeProjection;

    m_scaleFactor = m_scale / 100.0f;
    // initialise the game objects
    for (int i = 0; i < m_polygons->GetItemCount(); i++)
    {
        totemPolygon* item = (totemPolygon *)m_polygons->GetItems()[i];
        if (item != NULL)
        {
            item->Init(world);
            item->ResetAABB();
        }
    }
}


void totemLayer::Init(hdGameWorld* world, const totemLevel* level, const hdOrthographicProjection* relativeProjection)
{
    m_level = (totemLevel *)level;
    this->Init(world, relativeProjection);
}


bool totemLayer::RemovePolygon(const totemPolygon* polygon)
{
    this->DestroyGameObjects();
    if (false == m_polygons->Remove(polygon)) return false;
    for (int i = 0; i < m_polygons->GetItemCount(); i++)
    {
        totemPolygon* item = (totemPolygon *)m_polygons->GetItems()[i];
        if (item != NULL)
        {
            item->Init(m_world);
        }
    }
    return true;
}


bool totemLayer::ContainsPolygonAtPoint(const hdGameObject* polygon, const hdVec3& point)
{
    if (!m_polygons->Contains((totemPolygon *)polygon)) return false;

    return hdConcavePolygonContainsPoint(((hdGameObject *)polygon)->GetVertices(),
                                         ((hdGameObject *)polygon)->GetVertexCount(),
                                         ((hdGameObject *)polygon)->GetWorldCenter(),
                                         point);
}


void totemLayer::Draw() const
{
    assert(m_world != NULL);
    assert(m_polygons != NULL);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
#ifndef LEVEL_EDITOR
    glTranslatef(0.0f, 0.0f, -(float)(3 * m_scale));
#else
    glTranslatef(0.0f, 0.0f, 1.1f - m_scaleFactor);
#endif

    m_polygons->Map(&totemPolygon::Draw);

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

#ifdef LEVEL_EDITOR
    // Draw level aabb

    hdglBegin(GL_LINE_LOOP);
    hdglColor4f(0.0f, 1.0f, 0.0f, 1.0f);
    hdglVertex2f(m_level->GetAABB().lower.x / m_scaleFactor, m_level->GetAABB().lower.y / m_scaleFactor);
    hdglVertex2f(m_level->GetAABB().lower.x / m_scaleFactor, m_level->GetAABB().upper.y / m_scaleFactor);
    hdglVertex2f(m_level->GetAABB().upper.x / m_scaleFactor, m_level->GetAABB().upper.y / m_scaleFactor);
    hdglVertex2f(m_level->GetAABB().upper.x / m_scaleFactor, m_level->GetAABB().lower.y / m_scaleFactor);
    hdglEnd();
#endif
}
