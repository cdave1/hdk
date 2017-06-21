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

#ifndef HD_MESH_POLYGON_H
#define HD_MESH_POLYGON_H

#include <hdk/hdMath.h>
#include <hdk/hdGraphicsLibrary.h>

class hdMeshPolygon : public hdPolygon
{
public:
    hdMeshPolygon();

    ~hdMeshPolygon();

    const uint32 GetPolygonType() const;

    void SetPolygonType(const uint32 polygonType);

protected:

    uint32 m_polygonType;

private:
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<hdPolygon>(*this);
        ar & m_polygonType;
    }
};


inline const uint32 hdMeshPolygon::GetPolygonType() const
{
    return m_polygonType;
}


inline void hdMeshPolygon::SetPolygonType(const uint32 polygonType)
{
    m_polygonType = polygonType;
}


#endif