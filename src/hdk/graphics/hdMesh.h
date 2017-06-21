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

/*
 *
 * In the 2D context of the hdEngine, a mesh has a set of boundary contours and a set of internal
 * polygons.
 *
 * The internal polygons are used to render the face, while the boundary is used to render the
 * extrusion.
 *
 * NOTE: the boundary property can be used to generate 2D
 * physics outlines from arbitrary meshes. We simply need
 * to render the triangles of the mesh with a z value of 0,
 * and then generate a boundary polygon.
 *
 * The mesh allows us to present any arbitrary shape, so in a way,
 * it is no longer absolutely neccessary to retain shape information
 * apart from cases where physics approximation is required (e.g just
 * represent a 10 point start with a circle).
 */

#ifndef _HD_MESH_H_
#define _HD_MESH_H_

#include <hdk/hdGraphicsLibrary/hdGraphicsInterface.h>
#include <hdk/hdGraphicsLibrary/hdPolygon.h>
#include <hdk/hdGraphicsLibrary/hdMeshPolygon.h>

#ifdef LEVEL_EDITOR
#include "glut/glut.h"
#else
#define GLenum unsigned int
#endif

#define HD_MESH_USER_TYPE 0xF00D

#define HD_MESH_POLYGON_COUNT 1024
#define MESH_TEMP_VERTEX_COUNT 128

typedef GLdouble gluVector_t[3];


class hdMeshPolygon;

class hdMesh : public hdPolygon
{
public:
    hdMesh();

    hdMesh(hdGameWorld* gameWorld);

    ~hdMesh();

    void Init(hdGameWorld *gameWorld);

    void Rotate(const hdVec3& rotationVector, const hdVec3& rotationAxis);

    void Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis);

    void MoveTo(const hdVec3& center);

    void AddPointInternal(const float x, const float y, const float z);

    const bool AddPolygonMesh(hdMeshPolygon *polygonMesh);

    void RemovePolygonMeshAtIndex(const uint32 index);

    hdPolygon * GenerateTesselation();

    const float * Combine(const float x, const float y, const float z);

    void DebugDraw();

    void Begin(GLenum meshType);

    void End();


private:

    hdMeshPolygon *m_currentMeshPolygon;

    hdTypedefList<hdMeshPolygon *, HD_MESH_POLYGON_COUNT> * m_polygons;

    hdTypedefList<hdVec3, MESH_TEMP_VERTEX_COUNT> * m_tempVertices;

    float m_tempVertex[3];
    
    gluVector_t * m_gluVectors;
    
};

#endif
