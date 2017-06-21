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

#ifndef _HD_PHYSICS_GLUE_H_
#define _HD_PHYSICS_GLUE_H_

#include <hdk/math.h>

class b2World;
class b2Body;
class b2PolygonShape;
struct b2PolygonDef;
struct b2Vec2;

extern void h2CirclePhysicsBody2(b2World* physicsWorld, b2Body** outBody,
                                 const hdVec3& position, float radius,
                                 const float friction, const float density, const float restitution,
                                 bool isStatic, bool isSensor, int groupIndex,
                                 int categoryBits, int maskBits);


extern void h2CirclePhysicsBody(b2World* physicsWorld, b2Body** outBody,
                                const hdVec3& position, float radius,
                                const float friction, const float density,
                                bool isStatic, bool isSensor, int groupIndex,
                                int categoryBits, int maskBits);


/*
 * Creates physics body made up of multiple shapes - triangles in a triangle fan configuration - 
 * from the vertices given.
 *
 * The given vertices do not need to form a convex hull - can contain caves.
 *
 * Automagically ensures that the triangles are in CCW order.
 */
extern void hd2DMultiShapePhysicsBody(b2World* physicsWorld, b2Body** outBody,
                                      const hdVec3& position, const hdVec3* vertices,
                                      const int vertexCount, const float friction,
                                      const float density, const float restitution,
                                      bool isStatic, bool isSensor, int groupIndex,
                                      int categoryBits, int maskBits);


/*
 * Creates physics body made up of triangles in a triangle strip configuration.
 *
 * The given vertices do not need to form a convex hull - can contain caves.
 *
 * Automagically ensures that the triangles are in CCW order.
 */
extern void hd2DTriangleStripPhysicsBody(b2World* physicsWorld, b2Body** outBody,
                                         const hdVec3& position, const hdVec3* vertices,
                                         const int vertexCount, const float friction,
                                         const float density, bool isStatic,
                                         bool isSensor, int groupIndex,
                                         int categoryBits, int maskBits);


/*
 * Single shape from vertices means that vertices must be in CCW order and form a convex hull. Returns nulls if
 * not CCW or contains a cave. Also returns a null body if the
 *
 * Out variables are the body and shape resulting from the box2D shape creation.
 */
extern void hdMake2DSingleShapePhysicsBody(b2World* physicsWorld, b2Body** outBody,
                                           const hdVec3& position, const hdVec3* vertices,
                                           const int vertexCount, const float friction,
                                           const float density, const float restitution,
                                           bool isStatic, bool isSensor, int groupIndex,
                                           int categoryBits, int maskBits);


/*
 * DEPRECATED: use hdMake2DSingleShapePhysicsBody instead.
 * REASON: this function never had parameters for friction/density/restitution. Also
 * outShape was never used.
 *
 * this function now just calls hdMake2DSingleShapePhysicsBody with majick numbers for the
 * friction, density, and restitution values.
 */
extern void hd2DSingleShapePhysicsBody(b2World* physicsWorld, b2Body** outBody,
                                       b2PolygonShape **outShape, const hdVec3& position,
                                       const hdVec3* vertices, const int vertexCount,
                                       bool isStatic, bool isSensor, int groupIndex,
                                       int categoryBits, int maskBits);

bool hdIsCentroidValid(b2Vec2& c, b2PolygonDef &def);

bool hdIsCentroidValid(b2PolygonDef &def);

bool hdIsTriangleValid(b2PolygonDef &def);

#endif
