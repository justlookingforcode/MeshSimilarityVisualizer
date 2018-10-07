#include "HierachicalBS.h"

HierachicalBS::HierachicalBS()
{
}



HierachicalBS::HierachicalBS(const HierachicalBS& r)
{

	this->nodes = r.nodes;
	this->lowestDepthStartingIndex = r.lowestDepthStartingIndex;
}



HierachicalBS::~HierachicalBS()
{
}






HierachicalBS& HierachicalBS::operator = (const HierachicalBS& r)
{

	this->nodes = r.nodes;
	this->lowestDepthStartingIndex = r.lowestDepthStartingIndex;
	return *this;
}


HierachicalBS& HierachicalBS::ApplyTransform(const vec3 &  t_translationVec, const vec3& t_ScaleVec, const vec3& t_RotVec, const HierachicalBS& t_ModelSpaceSource)
{
	u32 total = nodes.size();
	for (u32 i = 0; i != total; ++i)
	{
		Proto::BS& bs(nodes[i].m_BS);
		if (nodes[i].index != -1)
		{
			const Proto::BS& src(t_ModelSpaceSource.nodes[i].m_BS);
			
			bs.UpdateBS(t_ScaleVec, t_translationVec, t_RotVec, src);
		}
	}
	return *this;
}

