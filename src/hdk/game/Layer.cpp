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

#include "Layer.h"


Layer::Layer()
{
    m_polygons = NULL;
    m_scale = 100;
    m_scaleFactor = 1.0f;

#ifdef LEVEL_EDITOR
    m_polygons = new hdSerializablePointerList<Polygon, kMaxLayerPolygons>();
#endif
}


Layer::Layer(const Level* level, const int scale)
{
    m_polygons = NULL;
    m_level = (Level *)level;
    m_scale = scale;
    m_scaleFactor = m_scale / 100.0f;

#ifdef LEVEL_EDITOR
    m_polygons = new hdSerializablePointerList<Polygon, kMaxLayerPolygons>();
#endif
}


Layer::Layer(hdGameWorld* world, const Level* level, const int scale, const hdOrthographicProjection* relativeProjection) : hdGameObject(world)
{
    m_polygons = NULL;
    m_level = (Level *)level;
    m_scale = scale;
    m_scaleFactor = m_scale / 100.0f;
    m_relativeProjection = (hdOrthographicProjection *)relativeProjection;

#ifdef LEVEL_EDITOR
    m_polygons = new hdSerializablePointerList<Polygon, kMaxLayerPolygons>();
#endif
}


Layer::~Layer()
{
    hdPrintf("Deleting Layer\n");
    m_polygons->RemoveAll();
    delete m_polygons;
}


void Layer::DestroyGameObjects()
{
    for (int i = 0; i < m_polygons->GetItemCount(); i++)
    {
        m_world->RemoveGameObject(m_polygons->GetItems()[i]);
    }
}


void Layer::Init(hdGameWorld* world, const hdOrthographicProjection* relativeProjection)
{
    ((hdGameObject *)this)->Init(world);
    m_relativeProjection = (hdOrthographicProjection *)relativeProjection;

    m_scaleFactor = m_scale / 100.0f;
    // initialise the game objects
    for (int i = 0; i < m_polygons->GetItemCount(); i++)
    {
        Polygon* item = (Polygon *)m_polygons->GetItems()[i];
        if (item != NULL)
        {
            item->Init(world);
            item->ResetAABB();
        }
    }
}


void Layer::Init(hdGameWorld* world, const Level* level, const hdOrthographicProjection* relativeProjection)
{
    m_level = (Level *)level;
    this->Init(world, relativeProjection);
}


bool Layer::RemovePolygon(const Polygon* polygon)
{
    this->DestroyGameObjects();
    if (false == m_polygons->Remove(polygon)) return false;
    for (int i = 0; i < m_polygons->GetItemCount(); i++)
    {
        Polygon* item = (Polygon *)m_polygons->GetItems()[i];
        if (item != NULL)
        {
            item->Init(m_world);
        }
    }
    return true;
}


bool Layer::ContainsPolygonAtPoint(const hdGameObject* polygon, const hdVec3& point)
{
    if (!m_polygons->Contains((Polygon *)polygon)) return false;

    return hdConcavePolygonContainsPoint(((hdGameObject *)polygon)->GetVertices(),
                                         ((hdGameObject *)polygon)->GetVertexCount(),
                                         ((hdGameObject *)polygon)->GetWorldCenter(),
                                         point);
}


void Layer::Draw() const
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

    m_polygons->Map(&Polygon::Draw);

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
