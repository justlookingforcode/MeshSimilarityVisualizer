#include "hierachicalAABB.h"
#include <array>
HierachicalAABB::HierachicalAABB()
{
}



HierachicalAABB::HierachicalAABB(const HierachicalAABB&r)
	:nodes(r.nodes)
	, maxDepth(r.maxDepth)
	, lowestDepthStartingIndex(r.lowestDepthStartingIndex)
{
}



HierachicalAABB::~HierachicalAABB()
{
}



void HierachicalAABB::BuildFromModel(const VertexBufferType &pnts, const std::vector<int> &indicies, const u32 maxDepth)
{
	this->maxDepth = maxDepth;
	//TODO: IMPLEMENT YOUR OWN MODEL PARSING ENTRY POINT
	this->nodes.resize(1);
	this->nodes[0].m_LeftChild = this->nodes[0].m_RightChild = -1;
	ConstructSubTree(pnts, indicies, &this->nodes[0], 0, 1);
}



HierachicalAABB& HierachicalAABB::operator = (const HierachicalAABB& r)
{
	this->maxDepth = r.maxDepth;
	this->nodes = r.nodes;
	this->lowestDepthStartingIndex = r.lowestDepthStartingIndex;

	return *this;
}



u32 HierachicalAABB::getMaxDepth()
{
	return maxDepth;
}

void HierachicalAABB::SubDivideModelTriangles(
	const Proto::AABB& parentAABB
	, const VertexBufferType &pnts
	, const std::vector<int> &indicies
	, std::vector<int>&leftIndices
	, std::vector<int>&rightIndices)
{
	//TODO : IMPLEMENT YOUR OWN CUSTOM TRIANGLE/VERTEX SPLITTING ALGORITHM
	return;
}


void HierachicalAABB::ConstructSubTree(const VertexBufferType &pnts
	, const std::vector<int> &indicies
	, HierachicalAABBNode* node
	, const u32 parentIndex
	, const u32 iterationCount)
{
	if (iterationCount <1) return;

	Proto::AABB aabb;
	aabb.Create(pnts, indicies);
	node->m_AABB = aabb;
	std::vector<int> lIndices, rIndicies;
	node->index = parentIndex;
	node->depth = maxDepth - iterationCount;
	//leaf node
	if (iterationCount <= 1)
	{
		node->indices = indicies;
		u32 total = node->indices.size();
		node->triangleIndices.resize(total / 3);
		for (u32 i = 0; i < total; i += 3)
		{
			u32 i0, i1, i2;
			i0 = node->indices[i];
			i1 = node->indices[i + 1];
			i2 = node->indices[i + 2];
			node->triangleIndices[i / 3][0] = i0;
			node->triangleIndices[i / 3][1] = i1;
			node->triangleIndices[i / 3][2] = i2;
		}

		return;
	}
	//TODO : IMPLEMENT YOUR OWN CUSTOM TREE CONSTRUCTION ALGORITHM
		
	
	
	return;
	
}



