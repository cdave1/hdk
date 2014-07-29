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

#ifndef _HD_GEOMETRY_H_
#define _HD_GEOMETRY_H_

#include <hdk/hdMath/hdMathCommon.h>
#include <hdk/hdMath/hdMatrix.h>

typedef struct hdTransform;

void hdScaleVertices(hdVec3* vertices, int vertexCount, const hdVec3& scale);
void hdRotateVertices(hdVec3* vertices, int vertexCount, const hdVec3& rotation);
void hdTranslateVertices(hdVec3* vertices, int vertexCount, const hdVec3& translation);

void hdTransformVertices(hdVec3* vertices, int vertexCount, const hdTransform& oldTransform, const hdTransform& newTransform);

struct hdAABB
{
	hdAABB() : lower(0, 0, 0), upper(0, 0, 0) {}
	
	const float Width()
	{
		return upper.x - lower.x;
	}
	
	const float Height()
	{
		return upper.y - lower.y;
	}
	
	hdVec3 lower;
	hdVec3 upper;
};


struct hdTransform
{
	hdTransform() : rotation(0,0,0), translation(0,0,0), scale(1.0f, 1.0f, 1.0f) {}
	
	hdVec3 rotation;
	hdVec3 translation;
	hdVec3 scale;
};


typedef enum
{
	e_obb2D,
	e_obb3D
} e_obbType;


struct hdOBB
{
	hdOBB() : 
	obbType(e_obb2D)
	{}
	
	void Transform(const hdTransform& trans)
	{
		if (obbType == e_obb2D)
		{
			hdTransformVertices(vertices, 4, transform, trans);
		} else {
			hdTransformVertices(vertices, 8, transform, trans);
		}
	}
	
	const int GetVertexCount() const
	{
		return (obbType == e_obb2D) ? 4 : 8;
	}
	
	e_obbType obbType; // 4 for 2D, 8 for 3D.
	hdTransform transform;
	hdVec3 vertices[8];
};


inline void hdTransformVertices(hdVec3* vertices, int vertexCount, const hdVec3& origin, const hdTransform& oldTransform, const hdTransform& newTransform)
{
	hdMatrix rot, trans, scale, res;
	MatrixIdentity(res);
	MatrixIdentity(rot);
	MatrixIdentity(trans);
	MatrixIdentity(scale);

	if ((oldTransform.rotation == newTransform.rotation) == false)
	{
		hdVec3 diff = newTransform.rotation - oldTransform.rotation;
		MatrixRotationXYZAxis(rot, diff.z, origin);
	}
	if ((oldTransform.translation == newTransform.translation) == false)
	{
		hdVec3 diff = newTransform.translation - oldTransform.translation;
		MatrixTranslation(trans, diff.x, diff.y, diff.z);
	}

	if ((oldTransform.scale == newTransform.scale) == false)
	{
		hdScaleVertices(vertices, vertexCount, newTransform.scale);
	}
	
	hdMatrixMultiply(res, trans, rot);
	hdMatrixMultiply(res, res, scale);
	
	for (int i = 0; i < vertexCount; ++i)
	{
		hdVec3 vert;
		MatrixVec4Multiply(vert, vertices[i], res);
		vertices[i] = vert;
	}
}


inline void hdTransformVertices(hdVec3* vertices, int vertexCount, const hdTransform& oldTransform, const hdTransform& newTransform)
{
	hdTransformVertices(vertices, vertexCount, hdVec3(0,0,0), oldTransform, newTransform);
}


inline void hdScaleVertices(hdVec3* vertices, int vertexCount, const hdVec3& scale)
{
	if (vertexCount == 0) return;
	for (int i = 0; i < vertexCount; ++i)
	{
		vertices[i].x *= scale.x;
		vertices[i].y *= scale.y;
		vertices[i].z *= scale.z;
	}
}


/*
 * Rotates vertices around origin.
 */
inline void hdRotateVertices(hdVec3* vertices, int vertexCount, const hdVec3& rotation)
{
#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1)
	hdMatrix rot;
	unsigned i;
	
	if (rotation.x == 0 && rotation.y == 0 && rotation.z == 0) return;
	
	MatrixRotationXYZOrigin(rot, -rotation);
	for (i = 0; i < vertexCount; ++i)
	{
		MatrixVec3Multiply(vertices[i], vertices[i], rot);
	}
#else
	hdVec3 vert;
	hdMatrix rot;
	unsigned i;
	
	if (rotation.x == 0 && rotation.y == 0 && rotation.z == 0) return;
	
	MatrixRotationXYZOrigin(rot, rotation);
	for (i = 0; i < vertexCount; ++i)
	{
		MatrixVec3Multiply(vert, vertices[i], rot);
		vertices[i].Set(vert.x, vert.y, vert.z);
	}
#endif
}


inline void hdTranslateVertices(hdVec3* vertices, int vertexCount, const hdVec3& translation)
{
	for (int i = 0; i < vertexCount; ++i)
	{
		vertices[i] += translation;
	}
}


/* 
 * via SuperCollider help files 
 */
inline float hdFastHypotenuse(const float x, const float y)
{
	const float sqrt2 = 1.41421;
	return fabs(x) + fabs(y) - ((sqrt2 - 1) * hdMin(fabs(x), fabs(y)));
}


inline float Get2DNormal(const hdVec3& a, const hdVec3& b)
{
	hdVec3 diff;
	float normal;
	
	diff = b - a;
	if (diff.x == 0) 
	{
		normal = 0.0f;
	}
	else if (diff.y == 0)
	{
		normal = (diff.x) > 0.0f ? 1.0f : -1.0f;
		
	}
	else
	{
		normal = diff.x / hdFastHypotenuse(diff.x, diff.y);
	}
	return normal;
}


/*
 * Vector product at point b, from a to b to c
 */
inline float hdVectorProduct(const hdVec2& a, const hdVec2& b, const hdVec2& c)
{
	return (a.x * (b.y - c.y)) + (b.x * (c.y - a.y)) + (c.x * (a.y - b.y));
}


inline float hdVectorProduct(const hdVec3& a, const hdVec3& b, const hdVec3& c)
{
	return (a.x * (b.y - c.y)) + (b.x * (c.y - a.y)) + (c.x * (a.y - b.y));
}


/*
 * Returns 1 if c is CCW of vector a-b, -1 if CW, and 0 if parallel
 */
inline signed int hdVectorProductSign(const hdVec2& a, const hdVec2& b, const hdVec2& c)
{
	float vectorProduct = hdVectorProduct(a,b,c);
	if (vectorProduct > 0.0f) return 1;
	return (vectorProduct < 0.0f) ? -1 : 0;
}


inline signed int hdCrossProductSign(const hdVec3& a, const hdVec3& b, const hdVec3&c)
{
	// a to b, b to c
	hdVec3 ab = b - a;
	hdVec3 bc = c - b;
	hdVec3 res = hdCross(ab, bc);
	return (res.z > 0.0) ? 1 : (res.z < 0.0f) ? -1 : 0; 
}


inline float hdAABBArea(const hdAABB& aabb)
{
	hdVec3 box = aabb.upper - aabb.lower;
	return box.x * box.y;
}


inline float hdAABBArea(const hdVec3& v1, const hdVec3& v2)
{
	hdAABB aabb;
	aabb.lower = hdMin(v1, v2);
	aabb.upper = hdMax(v1, v2);
	return hdAABBArea(aabb);
}


inline void hdFindAABB(const hdVec3* vertices, int vertexCount, hdAABB* aabb)
{
	if (vertexCount < 2)
	{
		aabb->lower.SetZero();
		aabb->upper.SetZero();
	} 
	else 
	{
		aabb->lower = vertices[0];
		aabb->upper = vertices[0];	

		for (int i = 0; i < vertexCount; ++i)
		{
			aabb->lower = hdMin(aabb->lower, vertices[i]);
			aabb->upper = hdMax(aabb->upper, vertices[i]);
		}
	}
}


/*
 * Another method (maybe faster?):
 * - Subtract point from lower, upper, and check signs of result
 * - signs should all be negative.
 */
inline bool hdPointInsideAABB(const hdVec3& point, const hdAABB& aabb)
{
	return  ((point.x < aabb.lower.x || point.x > aabb.upper.x) == false) &&
			((point.y < aabb.lower.y || point.y > aabb.upper.y) == false) &&
		    ((point.z < aabb.lower.z || point.z > aabb.upper.z) == false);
}


inline bool hdAABBIntersection(const hdAABB& aabb1, const hdAABB& aabb2)
{
	return (aabb1.lower.x < aabb2.upper.x) &&
			(aabb1.upper.x > aabb2.lower.x) &&
			(aabb1.lower.y < aabb2.upper.y) &&
			(aabb1.upper.y > aabb2.lower.y) &&
			(aabb1.lower.z < aabb2.upper.z) &&
			(aabb1.upper.z > aabb2.lower.z);
	
}


/*
 * Assume, for now, that point is on the same plane as points in polygon.
 */
inline bool hdPolygonContainsPoint(const hdVec3* vertices, const int vertexCount, const hdVec3& point)
{
	if (vertexCount == 1) return point == vertices[0];
	
	signed int contains = 0;
	for (int i = 0; i < vertexCount; ++i)
	{
		contains += hdCrossProductSign(vertices[i], vertices[(i+1)%vertexCount], point); 
	}
	
	return abs(contains) == vertexCount;	
}


/*
 * Determines if the polygon contains the point.
 *
 * Assumes that the polygon points are arranged in counter clockwise order.
 */
inline bool hdPolygonContainsPoint(const hdVec2* polygon, const int polygonCount, const hdVec2& point)
{
	if (polygonCount == 1) return point == polygon[0];
	
	signed int contains = 0x0001;
	for (int i = 0; i < polygonCount; ++i)
	{
		contains |= hdVectorProductSign(polygon[i], polygon[(i+1)%polygonCount], point); 
	}
	
	return contains == 0x0001;	
}


/*
 * Assuming the vertices define the hull of a solid shape, this function
 * returns true as soon as a triangle can be found that contains the given point.
 *
 * Use sparingly.
 */
inline bool hdConcavePolygonContainsPoint(const hdVec3* vertices, const int vertexCount, const hdVec3& center, const hdVec3& point)
{
	if (vertexCount == 1) return point == vertices[0];
	if (vertexCount == 2) return false; // lazy
	
	hdVec3 triangle[3];
	for (int i = 0; i < vertexCount; ++i)
	{
		triangle[0] = center;
		triangle[1] = vertices[i];
		triangle[2] = vertices[(i+1) % vertexCount];
		if (hdPolygonContainsPoint(triangle, 3, point)) return true;
	}
	
	return false;
}


/*
 * Return the angle between v1 and v2 using ref
 */
inline float hdGetAngle(const hdVec2& v1, const hdVec2& v2, const hdVec2& ref)
{
	hdVec2 _v1 = v1 - ref;
	hdVec2 _v2 = v2 - ref;
	
	_v1 = hdNormalize(_v1);
	_v2 = hdNormalize(_v2);
	
	float angle = atan2f(_v2.y, _v2.x) - atan2f(_v1.y, _v1.x);
	return angle;
}



inline float hdGetAngle(const hdVec3& v1, const hdVec3& v2, const hdVec3& ref)
{
	return hdGetAngle(hdVec3toVec2(v1), hdVec3toVec2(v2), hdVec3toVec2(ref));
}


/*
 * Ray intersection function.
 *
 * Adapted from algorithm 6 from softsurfer.com
 */
inline bool hdRayIntersectsTriangle(const hdVec3 *triangle, const hdVec3 *ray, hdVec3 &vOut)
{
	hdVec3 u, v, n;
	hdVec3 dir, w0, w;
	float r, a, b;
	
	u = triangle[2] - triangle[0];
	v = triangle[1] - triangle[0];
	n = hdCross(u, v);
	
	dir = ray[1] - ray[0];
	w0 = ray[0] - triangle[0];
	
	a = -(n * w0); // dot product
	b = n * dir; // dot product
	
	if (fabs(b) < 0.0000001f)
	{
		return false;
	}
	
	// Get intersected point of ray with triangles plane
	r = a / b;
	if (r < 0.0f) return false;
	
	dir *= r;
	vOut = ray[0] + dir;

	float uu, uv, vv, wu, wv, D;
	
	uu = u * u;
	uv = u * v;
	vv = v * v;
	
	w = vOut - triangle[0];
	wu = w * u;
	wv = w * v;
	
	D = (uv * uv) - (uu * vv);
	
	float s, t;
	s = (uv * wv - vv * wu) / D;
	if (s < 0.0 || s > 1.0) return false;
	
	t = (uv * wu - uu * wv) / D;
	if (t < 0.0 || (s + t) > 1.0f) return false;
	
	return true;
}


inline bool hdRayIntersectsVertices(const hdVec3 *vertices, const int vertexCount, const hdVec3& center, const hdVec3 *ray,  hdVec3 &vOut)
{
	if (vertexCount < 3) return false;
	
	hdVec3 triangle[3];
	for (int i = 0; i < vertexCount; ++i)
	{
		triangle[0] = center;
		triangle[1] = vertices[i];
		triangle[2] = vertices[(i+1) % vertexCount];
		if (hdRayIntersectsTriangle(triangle, ray, vOut)) return true;
	}
	
	return false;
}


inline float hdPolygonArea(const hdVec3 *vertices, const int vertexCount)
{
	if (vertexCount < 3) return 0;
	
	float accum = 0.0f;
	float area;
	
	hdVec3 tri[3];
	for (int i = 0; i < (vertexCount - 2); ++i)
	{
		tri[0] = vertices[i];
		tri[1] = vertices[(i+1) % vertexCount];
		tri[2] = vertices[(i+2) % vertexCount];
		
		area = (tri[0].x * (tri[1].y - tri[2].y)) 
				+ (tri[1].x * (tri[2].y - tri[0].y)) 
				+ (tri[2].x * (tri[0].y - tri[1].y));
		accum += (fabs(area) * 0.5f);
	}
	
	return accum;
}


/*
 * Return the "most counter clockwise" point of a and b in around pivot.
 *
 * Returns true if sum of cross product signs is positive and equal to the number of
 * vertices.
 */
inline bool hdIsConvexCCW(const hdVec3* vertices, const int vertexCount)
{
	if (vertexCount == 1) return false;
	
	signed int contains = 0;
	for (int i = 0; i < vertexCount; ++i)
	{
		contains += hdCrossProductSign(vertices[i], vertices[(i+1)%vertexCount], vertices[(i+2)%vertexCount]); 
	}
	
	return contains == vertexCount;
}


inline bool hdIsConvexCW(const hdVec3* vertices, const int vertexCount)
{
	if (vertexCount == 1) return false;
	
	signed int contains = 0;
	for (int i = 0; i < vertexCount; ++i)
	{
		contains += hdCrossProductSign(vertices[i], vertices[(i+1)%vertexCount], vertices[(i+2)%vertexCount]); 
	}
	
	return contains == -vertexCount;
}

#endif
