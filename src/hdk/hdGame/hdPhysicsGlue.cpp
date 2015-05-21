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

#include "hdPhysicsGlue.h"
#include "Box2D.h"

void h2CirclePhysicsBody2(b2World* physicsWorld, b2Body** outBody,
                          const hdVec3& position, float radius,
                          const float friction, const float density,
                          const float restitution, bool isStatic,
                          bool isSensor, int groupIndex,
                          int categoryBits, int maskBits)
{
    if (radius <= B2_FLT_EPSILON)
    {
        hdPrintf("h2CirclePhysicsBody: Can't create circular physics body with a radius smaller \
                 than FLT_EPSILON", radius);
        *outBody = NULL;
        return;
    }

    b2BodyDef bd;
    b2CircleDef cd;

    cd.radius = radius;
    cd.isSensor = isSensor;
    cd.density = density;
    cd.friction = friction;
    cd.restitution = restitution;

    if (groupIndex != 0) cd.filter.groupIndex = groupIndex;
    if (categoryBits > 0) cd.filter.categoryBits = categoryBits;
    if (maskBits > 0) cd.filter.maskBits = maskBits;
    if (isSensor) cd.isSensor = true;

    bd.position = b2Vec2(position.x, position.y);
    *outBody = physicsWorld->CreateBody(&bd);

    (*outBody)->CreateShape(&cd);

    if (isStatic == false)
    {
        (*outBody)->SetMassFromShapes();
    }
}


void h2CirclePhysicsBody(b2World* physicsWorld, b2Body** outBody,
                         const hdVec3& position, float radius,
                         const float friction, const float density,
                         bool isStatic, bool isSensor, int groupIndex,
                         int categoryBits, int maskBits)
{
    if (radius <= B2_FLT_EPSILON)
    {
        hdPrintf("h2CirclePhysicsBody: Can't create circular physics body with a radius smaller \
                 than FLT_EPSILON", radius);
        *outBody = NULL;
        return;
    }

    b2BodyDef bd;
    b2CircleDef cd;

    cd.radius = radius;
    cd.isSensor = isSensor;
    cd.density = density;
    if (groupIndex != 0) cd.filter.groupIndex = groupIndex;
    if (categoryBits > 0) cd.filter.categoryBits = categoryBits;
    if (maskBits > 0) cd.filter.maskBits = maskBits;
    if (isSensor) cd.isSensor = true;

    bd.position = b2Vec2(position.x, position.y);
    *outBody = physicsWorld->CreateBody(&bd);

    (*outBody)->CreateShape(&cd);

    if (isStatic == false)
    {
        (*outBody)->SetMassFromShapes();
    }
}


void hd2DMultiShapePhysicsBody(b2World* physicsWorld, b2Body** outBody,
                               const hdVec3& position, const hdVec3* vertices,
                               const int vertexCount, const float friction,
                               const float density, const float restitution,
                               bool isStatic, bool isSensor, int groupIndex,
                               int categoryBits, int maskBits)
{
    b2BodyDef bd;
    b2PolygonDef sd;
    b2Vec2 bodyCenter;
    hdVec2 a, b, pivot;

    if (vertexCount <= 2)
    {
        hdPrintf("hd2DMultiShapeBodyFromVertices: Physics Body not created because the number of \
                 vertices specified (%d) was not in range", vertexCount);
        *outBody = NULL;
        return;
    }

    bd.position = b2Vec2(position.x, position.y);
    *outBody = physicsWorld->CreateBody(&bd);

    bodyCenter = b2Vec2(position.x, position.y);
    pivot = hdVec3toVec2(vertices[0]);

    // Create a triangle fan from the coordinates.
    for (int i = 1; i < vertexCount-1; ++i)
    {
        a = hdVec3toVec2(vertices[i]);
        b = hdVec3toVec2(vertices[i+1]);

        sd.vertexCount = 3;
        sd.isSensor = isSensor;

        if (groupIndex != 0) sd.filter.groupIndex = groupIndex;
        if (categoryBits > 0) sd.filter.categoryBits = categoryBits;
        if (maskBits > 0) sd.filter.maskBits = maskBits;
        sd.vertices[0] = b2Vec2(pivot.x, pivot.y) - bodyCenter;
        sd.vertices[1] = b2Vec2(vertices[i].x, vertices[i].y) - bodyCenter;
        sd.vertices[2] = b2Vec2(vertices[i+1].x, vertices[i+1].y) - bodyCenter;

        sd.friction = friction;
        sd.density = density;
        sd.restitution = restitution;

        if (hdIsCentroidValid(sd) == false) {
            sd.vertices[1] = b2Vec2(vertices[i+1].x, vertices[i+1].y) - bodyCenter;
            sd.vertices[2] = b2Vec2(vertices[i].x, vertices[i].y) - bodyCenter;
        }

        if (hdIsTriangleValid(sd))
        {
            (*outBody)->CreateShape(&sd);
        }
    }

    if (isStatic == false)
    {
        (*outBody)->SetMassFromShapes();
    }
}



void hd2DTriangleStripPhysicsBody(b2World* physicsWorld, b2Body** outBody,
                                  const hdVec3& position, const hdVec3* vertices,
                                  const int vertexCount, const float friction,
                                  const float density, bool isStatic,
                                  bool isSensor, int groupIndex,
                                  int categoryBits, int maskBits)
{
    b2BodyDef bd;
    b2PolygonDef sd;
    b2Vec2 bodyCenter;

    if (vertexCount <= 2)
    {
        hdPrintf("hd2DTriangleStripPhysicsBody: Physics Body not created because the number of \
                 vertices specified (%d) was not in range", vertexCount);
        *outBody = NULL;
        return;
    }

    bd.position = b2Vec2(position.x, position.y);
    *outBody = physicsWorld->CreateBody(&bd);
    bodyCenter = b2Vec2(position.x, position.y);

    // Create a triangle STRIP from the coordinates.
    for (int i = 0; i < vertexCount-2; ++i)
    {
        sd.vertexCount = 3;
        sd.isSensor = isSensor;

        if (groupIndex != 0) sd.filter.groupIndex = groupIndex;
        if (categoryBits > 0) sd.filter.categoryBits = categoryBits;
        if (maskBits > 0) sd.filter.maskBits = maskBits;

        sd.vertices[0] = b2Vec2(vertices[i].x, vertices[i].y) - bodyCenter;
        sd.vertices[1] = b2Vec2(vertices[i+1].x, vertices[i+1].y) - bodyCenter;
        sd.vertices[2] = b2Vec2(vertices[i+2].x, vertices[i+2].y) - bodyCenter;

        sd.density = density;
        sd.restitution = 0.15f;
        if (hdIsCentroidValid(sd) == false) {
            sd.vertices[1] = b2Vec2(vertices[i+2].x, vertices[i+2].y) - bodyCenter;
            sd.vertices[2] = b2Vec2(vertices[i+1].x, vertices[i+1].y) - bodyCenter;
        }

        if (hdIsTriangleValid(sd))
        {
            (*outBody)->CreateShape(&sd);
        }
    }

    if (isStatic == false)
    {
        (*outBody)->SetMassFromShapes();
    }
}



void hd2DSingleShapePhysicsBody(b2World* physicsWorld, b2Body** outBody,
                                b2PolygonShape **outShape, const hdVec3& position,
                                const hdVec3* vertices, const int vertexCount,
                                bool isStatic, bool isSensor, int groupIndex,
                                int categoryBits, int maskBits)
{
    hdMake2DSingleShapePhysicsBody(physicsWorld, outBody, position, vertices, vertexCount,
                                   0.6f, 0.25f, 0.25f,
                                   isStatic, isSensor, groupIndex, categoryBits, maskBits);
}




void hdMake2DSingleShapePhysicsBody(b2World* physicsWorld, b2Body** outBody,
                                    const hdVec3& position,
                                    const hdVec3* vertices, const int vertexCount,
                                    const float friction, const float density, const float restitution,
                                    bool isStatic, bool isSensor, int groupIndex,
                                    int categoryBits, int maskBits)
{
    if (vertexCount < 3 || vertexCount > b2_maxPolygonVertices)
    {
        hdPrintf("hd2DBodyFromVertices: Physics Body not created because the number of vertices \
                 specified (%d) was not in range", vertexCount);
        *outBody = NULL;
        return;
    }

    b2BodyDef bd;
    bd.position = b2Vec2(position.x, position.y);
    *outBody = physicsWorld->CreateBody(&bd);

    b2PolygonDef sd;
    sd.isSensor = isSensor;
    if (groupIndex != 0) sd.filter.groupIndex = groupIndex;
    if (categoryBits > 0) sd.filter.categoryBits = categoryBits;
    if (maskBits > 0) sd.filter.maskBits = maskBits;
    sd.vertexCount = vertexCount;

    // Process the vertices in CCW order:
    if (hdIsConvexCCW(vertices, vertexCount))
    {
        hdPrintf("hd2DBodyFromVertices: CCW\n");
        for (int i = 0; i < vertexCount; ++i)
        {
            sd.vertices[i] = b2Vec2(vertices[i].x, vertices[i].y) - bd.position;
        }
    }
    else if (hdIsConvexCW(vertices, vertexCount))
    {
        hdPrintf("hd2DBodyFromVertices: NOT CCW\n");
        // reverse them so they are in convex order.
        for (int i = 0; i < vertexCount; ++i)
        {
            sd.vertices[vertexCount - 1 - i] = b2Vec2(vertices[i].x, vertices[i].y) - bd.position;
        }
    }
    else
    {
        *outBody = NULL;
        return;
    }

    sd.friction = friction;
    sd.density = density;
    sd.restitution = restitution;

    // Double check if the created centroid is broken - may
    // be the case with concave polygons.
    if (!hdIsTriangleValid(sd))
    {
        hdPrintf("hd2DBodyFromVertices: INVALID Triangle. Coordinates:\n");
        for (int i = 0; i < vertexCount; ++i)
        {
            hdPrintf("\t%d: (x=%f, y=%f, z=%f)\n", i, vertices[i].x, vertices[i].y, vertices[i].z);
        }
        *outBody = NULL;
        return;
    }

    (*outBody)->CreateShape(&sd);
    if (isStatic == false)
    {
        (*outBody)->SetMassFromShapes();
    }
}


bool hdIsTriangleValid(b2PolygonDef &def)
{
    b2Vec2 _normals[b2_maxPolygonVertices];
    b2Vec2 _vertices[b2_maxPolygonVertices];
    b2Vec2 _centroid; _centroid.Set(0,0);
    int _vertexCount;

    _vertexCount = def.vertexCount;

    // Copy vertices.
    for (int32 i = 0; i < _vertexCount; ++i)
    {
        _vertices[i] = def.vertices[i];
    }

    for (int32 i = 0; i < _vertexCount; ++i)
    {
        int32 i1 = i;
        int32 i2 = i + 1 < _vertexCount ? i + 1 : 0;
        b2Vec2 edge = _vertices[i2] - _vertices[i1];

        if (edge.LengthSquared() <= B2_FLT_EPSILON * B2_FLT_EPSILON) return false;

        _normals[i] = b2Cross(edge, 1.0f);
        _normals[i].Normalize();
    }

    // Compute the polygon centroid.
    if (!hdIsCentroidValid(_centroid, def)) return false;

    // Create core polygon shape by shifting edges inward.
    // Also compute the min/max radius for CCD.
    for (int32 i = 0; i < _vertexCount; ++i)
    {
        int32 i1 = i - 1 >= 0 ? i - 1 : _vertexCount - 1;
        int32 i2 = i;

        b2Vec2 n1 = _normals[i1];
        b2Vec2 n2 = _normals[i2];
        b2Vec2 v = _vertices[i] - _centroid;;

        b2Vec2 d;
        d.x = b2Dot(n1, v) - b2_toiSlop;
        d.y = b2Dot(n2, v) - b2_toiSlop;

        // Shifting the edge inward by b2_toiSlop should
        // not cause the plane to pass the centroid.
        if((d.x >= 0.0f) == false) return false;
        if ((d.y >= 0.0f) == false) return false;
    }
    return true;
}


bool hdIsCentroidValid(b2PolygonDef &def)
{
    b2Vec2 centroid; centroid.Set(0,0);
    return hdIsCentroidValid(centroid, def);
}


// HACK HACK HACK
//
// determine if the co-ordinates in the polygon will result in
// a valid b2 centroid.
//
// This code ripped from box2D
bool hdIsCentroidValid(b2Vec2& c, b2PolygonDef &def)
{
    if (def.vertexCount < 3) return false;
    float32 area = 0.0f;

    // pRef is the reference point for forming triangles.
    // It's location doesn't change the result (except for rounding error).
    b2Vec2 pRef(0.0f, 0.0f);

    const float32 inv3 = 1.0f / 3.0f;

    for (int32 i = 0; i < def.vertexCount; ++i)
    {
        // Triangle vertices.
        b2Vec2 p1 = pRef;
        b2Vec2 p2 = def.vertices[i];
        b2Vec2 p3 = i + 1 < def.vertexCount ? def.vertices[i+1] : def.vertices[0];

        b2Vec2 e1 = p2 - p1;
        b2Vec2 e2 = p3 - p1;

        float32 D = b2Cross(e1, e2);

        float32 triangleArea = 0.5f * D;
        area += triangleArea;
        
        // Area weighted centroid
        c += triangleArea * inv3 * (p1 + p2 + p3);
    }
    
    c *= 1.0f / area;
    return (area > B2_FLT_EPSILON);
}
