#ifndef HIERACHICAL_AABB_H_
#define HIERACHICAL_AABB_H_
#include <vector>
#include <array>
#include "AABB.h"
#include "math.hpp"
#include "Defines.h"
#include "BoundingVolume.h"
#include "Mesh.hpp"


struct HierachicalAABBNode
{
	HierachicalAABBNode()
		: index(-1)
		, m_Parent(-1)
		, m_LeftChild(-1)
		, m_RightChild(-1)
		, depth(-1)
		, collided(false)
	{}


	HierachicalAABBNode(const HierachicalAABBNode& r)
		: index(r.index)
		, m_Parent(r.m_Parent)
		, m_LeftChild(r.m_LeftChild)
		, m_RightChild(r.m_RightChild)
		, depth(r.depth)
		, collided(r.collided)
		, m_AABB(r.m_AABB)
		, indices(r.indices)
		, triangleIndices(r.triangleIndices)
	{}

	bool collided;
	u16 depth;
	s32 index;
	s32 m_Parent;
	s32 m_LeftChild;
	s32 m_RightChild;
	Proto::AABB m_AABB;
	std::vector<int> indices;
	std::vector<std::array<int,3>> triangleIndices;
};
typedef void(*VisitorFunc)(const HierachicalAABBNode& node);
typedef bool(*TraversalCheckFunc)(const HierachicalAABBNode& node);

class HierachicalAABB
{
public:
	HierachicalAABB();
	HierachicalAABB(const HierachicalAABB&);
	~HierachicalAABB();
	void BuildFromModel(const VertexBufferType &pnts, const std::vector<int> &indicies, const u32 maxDepth = 1);
	HierachicalAABB& operator = (const HierachicalAABB&);


	template< typename T1, typename T2>
	void VisitNodes(T1& v, T2& c)
	{
		if (!nodes.empty())
			CheckSubTree(nodes[0], v, c);
	}




	std::vector<HierachicalAABBNode> nodes;
	u32 lowestDepthStartingIndex;


	u32 getMaxDepth();

private:
	u32 maxDepth;
	void SubDivideModelTriangles(const Proto::AABB& parentAABB, const VertexBufferType &pnts, const std::vector<int> &indicies, std::vector<int>&leftIndices, std::vector<int>&rightIndices);
	void ConstructSubTree(const VertexBufferType &pnts, const std::vector<int> &indicies, HierachicalAABBNode*, const u32 parentIndex, const u32 iterationCount);

	template< typename T1, typename T2>
	void CheckSubTree(const HierachicalAABBNode& node, T1& visitor, T2& IsInPartition)
	{


		if (IsInPartition(node))//check for node intersection
		{
			if (node.m_LeftChild == -1 || node.m_RightChild == -1)
				visitor(node);// visit node contents
			if (node.m_LeftChild != -1)//traverse left
			{
				CheckSubTree(nodes[node.m_LeftChild], visitor, IsInPartition);
			}
			if (node.m_RightChild != -1)//traverse right
			{
				CheckSubTree(nodes[node.m_RightChild], visitor, IsInPartition);
			}
		}

	}



};

#endif