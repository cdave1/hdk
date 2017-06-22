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

#ifndef _HDK_GAME_LAYER_H_
#define _HDK_GAME_LAYER_H_

#include "Box2D.h"
#include <hdk/graphics.h>
#include "Polygon.h"

#define kMaxLayerPolygons 256

class Level;
class FloorInfo;

class Layer : public hdDrawable, public hdGameObject
{
public:
    Layer();

    Layer(const Level* level, const int scale);

    Layer(hdGameWorld* world, const Level* level, const int scale, const hdOrthographicProjection* relativeProjection);

    ~Layer();

    void Init(hdGameWorld* world, const hdOrthographicProjection* relativeProjection);

    void Init(hdGameWorld* world, const Level* level, const hdOrthographicProjection* relativeProjection);

    void DestroyGameObjects();


    void ResetOBB() {}

    hdVec3* GetVertices();

    const int GetVertexCount() const;

    void ApplyTransform(const hdTransform& transform);

    void Draw() const;

    bool ContainsPolygonAtPoint(const hdGameObject* polygon, const hdVec3& point);

    void Rotate(const hdVec3& rotationVector, const hdVec3& rotationAxis) {}

    void Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis) {}

    int AddPolygon(const Polygon* polygon);

    bool RemovePolygon(const Polygon* polygon);

    void SetLevel(const Level* level);

    void SetScale(const int scale);

    const int GetScale() const;


private:

    friend class Level;

    friend class boost::serialization::access;


    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & boost::serialization::base_object<hdGameObject>(*this);
        ar & m_scale;
        ar & m_polygons;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<hdGameObject>(*this);
        ar & m_scale;
        ar & m_polygons;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    Polygon* m_polygon;

    Level* m_level;

    hdOrthographicProjection* m_relativeProjection;

    const Level* GetLevel() const
    {
        return m_level;
    }

    // real world size compared to containing level. Treated as
    // a percentage.
    // The higher this amount, the further away
    // the layer will appear. Can be any number greater than 0.
    unsigned int m_scale;

    float m_scaleFactor;

    hdSerializablePointerList<Polygon, kMaxLayerPolygons>* m_polygons;
};


inline hdVec3* Layer::GetVertices()
{
    return m_obb.vertices;
}


inline const int Layer::GetVertexCount() const
{
    return m_obb.GetVertexCount();
}


inline void Layer::ApplyTransform(const hdTransform& transform)
{
    m_obb.transform = transform;
    ResetOBB();
}


inline int Layer::AddPolygon(const Polygon* polygon)
{
    return m_polygons->Add(polygon);
}


inline void Layer::SetLevel(const Level* level) 
{
    m_level = (Level*)level; 
}


inline void Layer::SetScale(const int scale)
{
    m_scale = scale;
    m_scaleFactor = scale / 100.0f;
}


inline const int Layer::GetScale() const
{
    return m_scale;
}

#endif
