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

#include "b2DynamicTree.h"

#include <string.h>
#include <float.h>

b2DynamicTree::b2DynamicTree()
{
	m_root = NULL;
	m_poolCount = b2Max(b2_nodePoolSize, 1);
	m_pool = (b2DynamicTreeNode*)b2Alloc(m_poolCount * sizeof(b2DynamicTreeNode));
	memset(m_pool, 0, m_poolCount * sizeof(b2DynamicTreeNode));

	// Build a linked list for the free list. The parent
	// pointer becomes the "next" pointer.
	for (int32 i = 0; i < m_poolCount - 1; ++i)
	{
		m_pool[i].parent = m_pool + i + 1;
	}
	m_pool[m_poolCount-1].parent = NULL;
	m_freeList = m_pool;

	m_path = 0;
}

b2DynamicTree::~b2DynamicTree()
{
	// This frees the entire tree in one shot.
	b2Free(m_pool);
}

// Allocate a node from the pool. Grow the pool if necessary.
b2DynamicTreeNode* b2DynamicTree::AllocateNode()
{
	// Peel a node off the free list.
	if (m_freeList != NULL)
	{
		b2DynamicTreeNode* node = m_freeList;
		m_freeList = m_freeList->parent;
		return node;
	}

	// The free list is empty. Rebuild a bigger pool.
	int32 newPoolCount = 2 * m_poolCount;
	b2DynamicTreeNode* newPool = (b2DynamicTreeNode*)b2Alloc(newPoolCount * sizeof(b2DynamicTreeNode));
	memcpy(newPool, m_pool, m_poolCount * sizeof(b2DynamicTreeNode));
	memset(newPool + m_poolCount, 0, (newPoolCount - m_poolCount) * sizeof(b2DynamicTreeNode));

	// Build a linked list for the free list. The parent
	// pointer becomes the "next" pointer.
	for (int32 i = m_poolCount; i < newPoolCount - 1; ++i)
	{
		newPool[i].parent = newPool + i + 1;
	}
	newPool[newPoolCount-1].parent = NULL;
	m_freeList = newPool + m_poolCount;

	b2Free(m_pool);
	m_pool = newPool;
	m_poolCount = newPoolCount;

	// Finally peel a node off the new free list.
	b2DynamicTreeNode* node = m_freeList;
	m_freeList = m_freeList->parent;
	return node;
}

// Return a node to the pool.
void b2DynamicTree::FreeNode(b2DynamicTreeNode* node)
{
	node->parent = m_freeList;
	m_freeList = node;
}

// Create a proxy in the tree as a leaf node. We return the index
// of the node instead of a pointer so that we can grow
// the node pool.
int32 b2DynamicTree::CreateProxy(const b2AABB& aabb, void* userData)
{
	b2DynamicTreeNode* node = AllocateNode();

	// Fatten the aabb.
	b2Vec2 center = aabb.GetCenter();
	b2Vec2 extents = b2_fatAABBFactor * aabb.GetExtents();
	node->aabb.lowerBound = center - extents;
	node->aabb.upperBound = center + extents;

	node->userData = userData;

	InsertLeaf(node);

	return (int32)(node - m_pool);
}

void b2DynamicTree::DestroyProxy(int32 proxyId)
{
	b2Assert(0 <= proxyId && proxyId < m_poolCount);

	b2DynamicTreeNode* node = m_pool + proxyId;

	b2Assert(node->IsLeaf());

	RemoveLeaf(node);
	FreeNode(node);
}


void b2DynamicTree::MoveProxy(int32 proxyId, const b2AABB& aabb)
{
	b2Assert(0 <= proxyId && proxyId < m_poolCount);

	b2DynamicTreeNode* node = m_pool + proxyId;

	b2Assert(node->IsLeaf());

	if (node->aabb.Contains(aabb))
	{
		return;
	}

	RemoveLeaf(node);

	b2Vec2 center = aabb.GetCenter();
	b2Vec2 extents = b2_fatAABBFactor * aabb.GetExtents();

	node->aabb.lowerBound = center - extents;
	node->aabb.upperBound = center + extents;

	InsertLeaf(node);
}

void* b2DynamicTree::GetProxy(int32 proxyId)
{
	if (proxyId < m_poolCount)
	{
		return m_pool[proxyId].userData;
	}
	else
	{
		return NULL;
	}
}

void b2DynamicTree::InsertLeaf(b2DynamicTreeNode* leaf)
{
	if (m_root == NULL)
	{
		m_root = leaf;
		m_root->parent = NULL;
		return;
	}

	// Find the best sibling for this node.
	b2Vec2 center = leaf->aabb.GetCenter();
	b2DynamicTreeNode* sibling = m_root;
	if (sibling->IsLeaf() == false)
	{
		do 
		{
			b2DynamicTreeNode* child1 = sibling->children[0];
			b2DynamicTreeNode* child2 = sibling->children[1];

			b2Vec2 delta1 = b2Abs(child1->aabb.GetCenter() - center);
			b2Vec2 delta2 = b2Abs(child1->aabb.GetCenter() - center);

			float32 norm1 = delta1.x + delta1.y;
			float32 norm2 = delta2.x + delta2.y;

			if (norm1 < norm2)
			{
				sibling = child1;
			}
			else
			{
				sibling = child2;
			}

		}
		while(sibling->IsLeaf() == false);
	}

	// Create a parent for the siblings.
	b2DynamicTreeNode* node1 = sibling->parent;
	b2DynamicTreeNode* node2 = AllocateNode();
	node2->parent = node1;
	node2->userData = NULL;
	node2->aabb.Combine(leaf->aabb, sibling->aabb);

	if (node1)
	{
		if (sibling->parent->children[0] == sibling)
		{
			node1->children[0] = node2;
		}
		else
		{
			node1->children[1] = node2;
		}

		node2->children[0] = sibling;
		node2->children[1] = leaf;
		sibling->parent = node2;
		leaf->parent = node2;

		do 
		{
			if (node1->aabb.Contains(node2->aabb))
			{
				break;
			}

			node1->aabb.Combine(node1->children[0]->aabb, node1->children[1]->aabb);
			node2 = node1;
			node1 = node1->parent;
		}
		while(node1 != NULL);
	}
	else
	{
		node2->children[0] = sibling;
		node2->children[1] = leaf;
		sibling->parent = node2;
		leaf->parent = node2;
		m_root = node2;
	}
}

void b2DynamicTree::RemoveLeaf(b2DynamicTreeNode* leaf)
{
	if (leaf == m_root)
	{
		m_root = NULL;
		return;
	}

	b2DynamicTreeNode* node2 = leaf->parent;
	b2DynamicTreeNode* node1 = node2->parent;
	b2DynamicTreeNode* sibling;
	if (node2->children[0] == leaf)
	{
		sibling = node2->children[1];
	}
	else
	{
		sibling = node2->children[0];
	}

	if (node1)
	{
		// Destroy node2 and connect node1 to sibling.
		if (node1->children[0] == node2)
		{
			node1->children[0] = sibling;
		}
		else
		{
			node1->children[1] = sibling;
		}
		sibling->parent = node1;
		FreeNode(node2);

		// Adjust ancestor bounds.
		while (node1)
		{
			b2AABB oldAABB = node1->aabb;
			node1->aabb.Combine(node1->children[0]->aabb, node1->children[1]->aabb);

			if (oldAABB.Contains(node1->aabb))
			{
				break;
			}

			node1 = node1->parent;
		}
	}
	else
	{
		m_root = sibling;
		sibling->parent = NULL;
		FreeNode(node2);
	}
}

void b2DynamicTree::Rebalance(int32 iterations)
{
	if (m_root == NULL)
	{
		return;
	}

	for (int32 i = 0; i < iterations; ++i)
	{
		b2DynamicTreeNode* node = m_root;

		uint32 bit = 0;
		while (node->IsLeaf() == false)
		{
			node = node->children[(m_path >> bit) & 1];
			bit = (bit + 1) & (8* sizeof(uint32) - 1);
		}
		++m_path;

		RemoveLeaf(node);
		InsertLeaf(node);
	}
}
