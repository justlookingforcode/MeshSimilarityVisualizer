/* Start Header ------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.

File Name:      OBB.h
Purpose:        OBB class , and associated functions
Language:       C++
Platform:       Windows Visual Studio
Author:         Kevin Tan
Kevin Tan
Creation Date:  10/10/2014
- End Header -------------------------------------------------------*/
#ifndef OBB_H_
#define OBB_H_

#include <array>
#include "Defines.h"
#include "BoundingVolume.h"
/*
* This code heavily references http://www.gameenginegems.net/geg2.php
*///Dito14 algorithm

	class OBB 
	{
	public:

		OBB();
		~OBB();

		/* void Create(VertexBufferType & t_ModelVertexList);*/
		//<! hack to get around wierd ass bug
		void Create(VertexBufferType & t_ModelVertexList, vec3&center);
		f32 SqDistPointOBB(vec3 t_Point);
		void ClosestPtPointOBB(vec3 t_Point, vec3 & t_PtOnOBB);


		void UpdateOBB(const mat4& transform);
		mat4& GetBoxTransform(mat4& output);


		void BuildFromModel(const VertexBufferType &pnts, const std::vector<int> &indicies);


		vec3 m_center;
		vec3 m_radius;
		mat3 m_localAxes;
		



	private:
		void CovarianceMatrix(mat3 &C, const VertexBufferType &pnts, const std::vector<int> &indicies);
	
	};

#endif