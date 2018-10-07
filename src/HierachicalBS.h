#ifndef HIERACHICAL_BS_H_
#define HIERACHICAL_BS_H_
#include <vector>
#include "BS.h"
#include "math.hpp"
#include "Defines.h"
#include "BoundingVolume.h"
#include "Mesh.hpp"
struct HierachicalBSNode
{
	HierachicalBSNode()
		: index(-1)
		, m_Parent(-1)
		, m_LeftChild(-1)
		, m_RightChild(-1)
		, depth(-1)
		, collided(false)
	{}

	bool collided;
	u16 depth;
	s32 index;
	s32 m_Parent;
	s32 m_LeftChild;
	s32 m_RightChild;
	Proto::BS m_BS;
};

class HierachicalBS
{
public:
	HierachicalBS();
	HierachicalBS(const HierachicalBS&);
	~HierachicalBS();
	void BuildFromModel(const VertexBufferType &pnts, const std::vector<int> &indicies, const u32 maxDepth = 7);
	HierachicalBS& operator = (const HierachicalBS&);
	HierachicalBS& ApplyTransform(const vec3 &  t_translationVec, const vec3& t_ScaleVec, const vec3& t_RotVec, const HierachicalBS& t_ModelSpaceSource);

	std::vector<HierachicalBSNode> nodes;
	u32 lowestDepthStartingIndex;
private:



};
#endif