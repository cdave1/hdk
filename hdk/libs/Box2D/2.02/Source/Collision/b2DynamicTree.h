/*
* Copyright (c) 2009 Erin Catto http://www.gphysics.com
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef B2_DYNAMIC_TREE_H
#define B2_DYNAMIC_TREE_H

#include "b2Collision.h"

/// A node in the dynamic tree. The client does not interact with this directly.
struct b2DynamicTreeNode
{
	// TODO_ERIN add filtering?

	bool IsLeaf() const
	{
		return children[1] == NULL;
	}

	b2AABB aabb;
	b2DynamicTreeNode* parent;

	union
	{
		b2DynamicTreeNode* children[2];
		void* userData;
	};
};

/// A callback for AABB queries.
class b2QueryCallback
{
public:
	~b2QueryCallback() {}

	/// This function is called for each overlapping AABB.
	/// @return true if the query should continue.
	virtual bool Process(void* userData) = 0;
};

/// Ray-cast input data.
struct b2RayCastInput
{
	b2Vec2 p1, p2;
	float32 maxFraction;
};

/// A callback for ray casts.
class b2RayCastCallback
{
public:
	~b2RayCastCallback() {}

	/// Process a ray-cast. This allows the client to perform an exact ray-cast
	/// against their object (found from the proxyUserData pointer).
	/// @param input the original ray-cast segment with an adjusted maxFraction.
	/// @param maxFraction the clipping parameter, the ray extends from p1 to p1 + maxFraction * (p2 - p1).
	/// @param userData user data associated with the current proxy.
	/// @return the new max fraction. Return 0 to end the ray-cast. Return the input maxFraction to
	/// continue the ray cast. Return a value less than maxFraction to clip the ray-cast.
	virtual float32 Process(const b2RayCastInput& input, void* userData) = 0;
};

/// A dynamic tree arranges data in a binary tree to accelerate
/// queries such as volume queries and ray casts. Leafs are proxies
/// with an AABB. In the tree we expand the proxy AABB by b2_fatAABBFactor
/// so that the proxy AABB is bigger than the client object. This allows the client
/// object to move by small amounts without triggering a tree update.
class b2DynamicTree
{
	/// Constructing the tree initializes the node pool.
	b2DynamicTree();

	/// Destroy the tree, freeing the node pool.
	~b2DynamicTree();

	/// Create a proxy. Provide a tight fitting AABB and a userData pointer.
	int32 CreateProxy(const b2AABB& aabb, void* userData);

	/// Destroy a proxy. This asserts if the id is invalid.
	void DestroyProxy(int32 proxyId);

	/// Move a proxy. If the proxy has moved outside of its fattened AABB,
	/// then the proxy is removed from the tree and re-inserted. Otherwise
	/// the function returns immediately.
	void MoveProxy(int32 proxyId, const b2AABB& aabb);

	/// Perform some iterations to rebalance the tree.
	void Rebalance(int32 iterations);

	/// Get proxy user data.
	/// @return the proxy user data or NULL if the id is invalid.
	void* GetProxy(int32 proxyId);

	/// Query an AABB for overlapping proxies. The callback class
	/// is called for each proxy that overlaps the supplied AABB.
	template <typename T>
	void Query(T* callback, const b2AABB& aabb) const;

	/// Ray-cast against the proxies in the tree. This relies on the callback
	/// to perform a exact ray-cast in the case were the proxy contains a shape.
	/// The callback also performs the any collision filtering. This has performance
	/// roughly equal to k * log(n), where k is the number of collisions and n is the
	/// number of proxies in the tree.
	/// @param input the ray-cast input data. The ray extends from p1 to p1 + maxFraction * (p2 - p1).
	/// @param callback a callback class that is called for each proxy that is hit by the ray.
	template <typename T>
	void RayCast(T* callback, const b2RayCastInput& input) const;

private:

	b2DynamicTreeNode* AllocateNode();
	void FreeNode(b2DynamicTreeNode* node);

	void InsertLeaf(b2DynamicTreeNode* node);
	void RemoveLeaf(b2DynamicTreeNode* node);

	b2DynamicTreeNode* m_root;

	b2DynamicTreeNode* m_pool;
	int32 m_poolCount;

	b2DynamicTreeNode* m_freeList;

	/// This is used incrementally traverse the tree for rebalancing.
	uint32 m_path;
};

template <typename T>
inline void b2DynamicTree::Query(T* callback, const b2AABB& aabb) const
{
	if (m_root == NULL)
	{
		return;
	}

	const int32 k_stackSize = 32;
	const b2DynamicTreeNode* stack[k_stackSize];

	int32 count = 0;
	stack[count++] = m_root;

	while (count > 0)
	{
		const b2DynamicTreeNode* node = stack[--count];

		if (b2TestOverlap(node->aabb, aabb))
		{
			if (node->IsLeaf())
			{
				callback->Process(node->userData);
			}
			else
			{
				b2Assert(count + 1 < k_stackSize);
				stack[count++] = node->children[0];
				stack[count++] = node->children[1];
			}
		}
	}
}

template <typename T>
inline void b2DynamicTree::RayCast(T* callback, const b2RayCastInput& input) const
{
	if (m_root == NULL)
	{
		return;
	}

	b2Vec2 p1 = input.p1;
	b2Vec2 p2 = input.p2;
	b2Vec2 r = p2 - p1;
	b2Assert(r.LengthSquared() > 0.0f);
	r.Normalize();

	// v is perpendicular to the segment.
	b2Vec2 v = b2Cross(1.0f, r);
	b2Vec2 abs_v = b2Abs(v);

	// Separating axis for segment (Gino, p80).
	// |dot(v, p1 - c)| > dot(|v|, h)

	float32 maxFraction = input.maxFraction;

	// Build a bounding box for the segment.
	b2AABB segmentAABB;
	{
		b2Vec2 t = p1 + maxFraction * (p2 - p1);
		segmentAABB.lowerBound = b2Min(p1, t);
		segmentAABB.upperBound = b2Max(p1, t);
	}

	const int32 k_stackSize = 32;
	b2DynamicTreeNode* stack[k_stackSize];

	int32 count = 0;
	stack[count++] = m_root;

	while (count > 0)
	{
		const b2DynamicTreeNode* node = stack[--count];

		if (b2TestOverlap(node->aabb, segmentAABB) == FALSE)
		{
			continue;
		}

		// Separating axis for segment (Gino, p80).
		// |dot(v, p1 - c)| > dot(|v|, h)
		b2Vec2 c = node->aabb.GetCenter();
		b2Vec2 h = node->aabb.GetExtents();
		float32 separation = b2Abs(b2Dot(v, p1 - c)) - b2Dot(abs_v, h)
			if (separation > 0.0f)
			{
				continue;
			}

			if (node->IsLeaf())
			{
				b2RayCastInput i;
				i.p1 = input.p1;
				i.p2 = input.p2;
				i.maxFraction = maxFraction;

				float32 newFraction = callback->Process(i, node->userData);

				if (newFraction != maxFraction)
				{
					b2Assert(newFraction < maxFraction);

					// Early exit.
					if (newFraction == 0.0f)
					{
						return;
					}

					maxFraction = newFraction;

					// Update segment bounding box.
					{
						b2Vec2 t = p1 + maxFraction * (p2 - p1);
						segmentAABB.lowerBound = b2Min(p1, t);
						segmentAABB.upperBound = b2Max(p1, t);
					}
				}
			}
			else
			{
				b2Assert(count + 1 < k_stackSize);
				stack[count++] = node->children[0];
				stack[count++] = node->children[1];
			}
	}
}

#endif
