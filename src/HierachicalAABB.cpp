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

void HierachicalAABB::GetHalfLengthAndSort(
	std::array<int, 3>& idxs,
	std::array<float, 3>& lens,
	const vec3& radius)
{
	//get the largest half extends (scale from center)
	for (int i = 0; i < 3; ++i)
	{
		float halfLen = std::abs(radius[i] * 0.5f);
		if (halfLen >= lens[0])
		{
			ShiftBackAndAssign<float, 3>(lens, 0, halfLen);
			ShiftBackAndAssign<int, 3>(idxs, 0, i);
		}
		else if (halfLen >= lens[1])
		{
			ShiftBackAndAssign<float, 3>(lens, 1, halfLen);
			ShiftBackAndAssign<int, 3>(idxs, 1, i);
		}
		else if (halfLen >= lens[2])
		{
			lens[2] = halfLen;
			idxs[2] = i;
		}
	}
}

void HierachicalAABB::SubDivideModelTriangles(
	const Proto::AABB& parentAABB
	, const VertexBufferType &pnts		//vertex buffer
	, const std::vector<int> &indicies	//index buffer
	, std::vector<int>&leftIndices
	, std::vector<int>&rightIndices)
{
	//TODO : IMPLEMENT YOUR OWN CUSTOM TRIANGLE/VERTEX SPLITTING ALGORITHM

	std::array<int, 3> idxs{ 0,0,0 };
	std::array<float, 3> lens{ 0, 0, 0 };
	std::array<vec3, 3> axis{ vec3{1,0,0}, vec3{0,1,0}, vec3{0,0,1} };

	GetHalfLengthAndSort(idxs, lens, parentAABB.m_Radius);

	unsigned axisCount = 0;
	std::vector<int> left, right;
	float oneThird = 1 / 3.0f;
	while (axisCount < 3 && (left.empty() || right.empty()))
	{
		left.clear();
		right.clear();
		const vec3 curAxis = axis[idxs[axisCount]];
		for (int i = 0; i < indicies.size(); i+=3)
		{
			vec3 barycenter = (pnts[indicies[i]].pos + pnts[indicies[i+1]].pos + pnts[indicies[i+2]].pos) *oneThird;
			float dot = Dot(curAxis, barycenter - parentAABB.m_Center);

			if (dot > 0.f)
			{
				right.push_back(indicies[i]);
				right.push_back(indicies[i+1]);
				right.push_back(indicies[i+2]);
			}
			else
			{
				left.push_back(indicies[i]);
				left.push_back(indicies[i+1]);
				left.push_back(indicies[i+2]);
			}
		}
		++axisCount;
	}

	std::swap(leftIndices, left);
	std::swap(rightIndices, right);
	return;
}


void HierachicalAABB::ConstructSubTree(const VertexBufferType &pnts
	, const std::vector<int> &indicies
	, HierachicalAABBNode* node
	, const u32 parentIndex
	, const u32 iterationCount)
{
	//have atleast one node in the tree
	if (iterationCount <1) 
		return;

	//create a node
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
	SubDivideModelTriangles(node->m_AABB, pnts, indicies,lIndices, rIndicies);	
	
	
	return;
	
}



