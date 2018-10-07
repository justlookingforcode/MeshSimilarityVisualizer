/* Start Header ------------------------------------------------------
    Copyright (C) 2014 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents 
    without the prior written consent of DigiPen Institute of 
    Technology is prohibited.

    File Name:      AABB.cpp
    Purpose:        This is where the AABB class implementations.
    Language:       C++
    Platform:       Windows Visual Studio
    Author:         Tan Wen De Kevin
    Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/

#include "AABB.h"
#include <iostream>
#include <algorithm>
namespace Proto
{
    /*************************************************************************/
    /*!
    \fn AABB::AABB()

    \brief
        This is the default constructor of the Axis-Aligned Bounding Box (AABB) 
        class.
    */
    /*************************************************************************/
    AABB::AABB()
    {
//        m_BVType = TYPE_AABB;
    }

    /*************************************************************************/
    /*!
    \fn void AABB::ComputeCenterRadius(const vec3 & t_Min,
                                       const vec3 & t_Max)

    \brief
        This function computes the center and the radius of the AABB.
        The radius consists of 3 components (x, y, and z), and is the positive
        half vector.
    */
    /*************************************************************************/
    void AABB::ComputeCenterRadius(const vec3 & t_Min,
                                   const vec3 & t_Max)
    {
        vec3 t_Vector   = t_Max + t_Min;
        m_Center        = t_Vector * 0.5f;
        t_Vector        = m_Center - t_Min;
        SetRadius(t_Vector);
    }

    /*************************************************************************/
    /*!
    \fn const vec3 AABB::GetRadius() const

    \brief
        This function retrieves the AABB radius.

    \return
        This function returns the AABB radius.
    */
    /*************************************************************************/
    const vec3 AABB::GetRadius() const
    {
        return vec3(m_Radius[0], m_Radius[1], m_Radius[2]);
    }

    /*************************************************************************/
    /*!
    \fn void AABB::SetRadius(const vec3 & t_NewRadius)

    \brief
        This function sets the new radius to the AABB

    \param t_NewRadius
        This is the new radius.
    */
    /*************************************************************************/
    void AABB::SetRadius(const vec3 & t_NewRadius)
    {
        this->m_Radius[0] = t_NewRadius.x;
        this->m_Radius[1] = t_NewRadius.y;
        this->m_Radius[2] = t_NewRadius.z;
    }

    /*************************************************************************/
    /*!
    \fn const vec3 AABB::GetMaxVertex() const

    \brief
        This function retrieves the AABB's max vertex.

    \return
        This function returns the AABB's max vertex.
    */
    /*************************************************************************/
    const vec3 AABB::GetMaxVertex() const
    {
        return (this->m_Center + GetRadius());
    }

    /*************************************************************************/
    /*!
    \fn const vec3 AABB::GetMinVertex() const

    \brief
        This function retrieves the AABB's min vertex.

    \return
        This function returns the AABB's min vertex.
    */
    /*************************************************************************/
    const vec3 AABB::GetMinVertex() const
    {
        return (this->m_Center - GetRadius());
    }

	vec3& AABB::GetMinVertex(vec3& v) const
	{
		v = m_Center - GetRadius();
		return v;
	}
	vec3& AABB::GetMaxVertex(vec3& v) const
	{
		v = m_Center + GetRadius();
		return v;
	}

    /*************************************************************************/
    /*!
    \fn void AABB::UpdateAABB(mat4       & t_ScaleRotMat, 
                              vec3       & t_Pos,
                              const vec3 & t_RotVec,
                              const AABB & t_ModelAABB)

    \brief
        This function updates the AABB.

    \param t_ScaleRotMat
        This is the concated scale-rotation matrix.

    \param * t_Pos
        This is the position of the model.

    \param t_ModelAABB
        This is the initial/original model AABB.
    */
    /*************************************************************************/
    void AABB::UpdateAABB(const vec3 & t_ScaleVec, 
                          const vec3 & t_Pos,
                          const vec3 & t_RotVec, 
                          const AABB &  t_ModelAABB)
    {
        mat4 t_ScaleRotMat = RotationMatrix(t_RotVec.x, t_RotVec.y, t_RotVec.z) * ScaleMatrix(t_ScaleVec);

		this->m_Center = vec3(t_ScaleRotMat *vec4(t_Pos, 1));
        for(int i = 0; i < TOTAL_AXIS; ++i) 
        {
            //this->m_Center[i] = t_Pos[i];
            this->m_Radius[i] = 0.f;

            for(int j = 0; j < TOTAL_AXIS; ++j) 
            {
                //this->m_Center[i] += t_ScaleRotMat[i][j]        * t_ModelAABB.m_Center[j];
                this->m_Radius[i] += fabs(t_ScaleRotMat[j][i])  * t_ModelAABB.m_Radius[j];
            }
        }
    }

    void AABB::UpdateAABB(const mat4& transform,
        const AABB &  t_ModelAABB)
    {

        vec3 transformedCenter = vec3(transform * vec4(t_ModelAABB.m_Center,1));

        for (int i = 0; i < TOTAL_AXIS; ++i)
        {
            this->m_Center[i] = transformedCenter[i];
            this->m_Radius[i] = 0.f;

            for (int j = 0; j < TOTAL_AXIS; ++j)
            {
                //this->m_Center[i] += transform[i][j] * t_ModelAABB.m_Center[j];
                this->m_Radius[i] += fabs(transform[j][i])  * t_ModelAABB.m_Radius[j];
            }
        }
    }



    /*************************************************************************/
    /*!
    \fn void AABB::Create(VertexBufferType & t_ModelVertexList)

    \brief
        This function creates the AABB.

    \param t_ModelVertexList
        This is the object model vertex list.
    */
    /*************************************************************************/
    void AABB::Create(const VertexBufferType & t_ModelVertexList)
    {
        //vec3 g_Min = vec3( FLT_MAX ,  FLT_MAX , -FLT_MAX);
        vec3 g_Min = vec3( FLT_MAX ,  FLT_MAX , FLT_MAX);
        //vec3 g_Max = vec3(-FLT_MAX , -FLT_MAX ,  FLT_MAX);
        vec3 g_Max = vec3(-FLT_MAX , -FLT_MAX ,  -FLT_MAX);

        u32 t_Size = t_ModelVertexList.size();
        for(u32 i = 0; i < t_Size; ++i)
        {
            Vertex t_NewVertex  = t_ModelVertexList[i];
            f32 t_PosX          = t_NewVertex.pos.x;
            f32 t_PosY          = t_NewVertex.pos.y;
            f32 t_PosZ          = t_NewVertex.pos.z;

            // Update lower-left-front corner of BB
            g_Min.x = std::min(g_Min.x, t_PosX);
			g_Min.y = std::min(g_Min.y, t_PosY);
			//g_Min.z = std::max(g_Min.z, t_PosZ);
			g_Min.z = std::min(g_Min.z, t_PosZ);
			    
            // Update upper-right-back corner of BB
			g_Max.x = std::max(g_Max.x, t_PosX);
			g_Max.y = std::max(g_Max.y, t_PosY);
			//g_Max.z = std::min(g_Max.z, t_PosZ);
			g_Max.z = std::max(g_Max.z, t_PosZ);
        }

        // set the BB center point based on the min-max found
        ComputeCenterRadius(g_Min, g_Max);
    }

	void AABB::Create(const VertexBufferType & t_ModelVertexList, const std::vector<int>& indicies)
	{
		//vec3 g_Min = vec3( FLT_MAX ,  FLT_MAX , -FLT_MAX);
		vec3 g_Min = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
		//vec3 g_Max = vec3(-FLT_MAX , -FLT_MAX ,  FLT_MAX);
		vec3 g_Max = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		u32 t_Size = indicies.size();
		for (u32 i = 0; i < t_Size; ++i)
		{
			Vertex t_NewVertex = t_ModelVertexList[indicies[i]];
			f32 t_PosX = t_NewVertex.pos.x;
			f32 t_PosY = t_NewVertex.pos.y;
			f32 t_PosZ = t_NewVertex.pos.z;

			// Update lower-left-front corner of BB
			g_Min.x = std::min(g_Min.x, t_PosX);
			g_Min.y = std::min(g_Min.y, t_PosY);
			//g_Min.z = std::max(g_Min.z, t_PosZ);
			g_Min.z = std::min(g_Min.z, t_PosZ);

			// Update upper-right-back corner of BB
			g_Max.x = std::max(g_Max.x, t_PosX);
			g_Max.y = std::max(g_Max.y, t_PosY);
			//g_Max.z = std::min(g_Max.z, t_PosZ);
			g_Max.z = std::max(g_Max.z, t_PosZ);
		}

		// set the BB center point based on the min-max found
		ComputeCenterRadius(g_Min, g_Max);
	}

	void AABB::Create(const mat4&transform, const VertexBufferType & t_ModelVertexList)
	{
		//vec3 g_Min = vec3( FLT_MAX ,  FLT_MAX , -FLT_MAX);
		vec3 g_Min = vec3(FLT_MAX, FLT_MAX, FLT_MAX);
		//vec3 g_Max = vec3(-FLT_MAX , -FLT_MAX ,  FLT_MAX);
		vec3 g_Max = vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);

		u32 t_Size = t_ModelVertexList.size();
		for (u32 i = 0; i < t_Size; ++i)
		{
			Vertex t_NewVertex = t_ModelVertexList[i];
			vec3 pt = vec3(transform * vec4(t_NewVertex.pos,1));
			f32 t_PosX = pt.x;
			f32 t_PosY = pt.y;
			f32 t_PosZ = pt.z;

			// Update lower-left-front corner of BB
			g_Min.x = std::min(g_Min.x, t_PosX);
			g_Min.y = std::min(g_Min.y, t_PosY);
			//g_Min.z = std::max(g_Min.z, t_PosZ);
			g_Min.z = std::min(g_Min.z, t_PosZ);

			// Update upper-right-back corner of BB
			g_Max.x = std::max(g_Max.x, t_PosX);
			g_Max.y = std::max(g_Max.y, t_PosY);
			//g_Max.z = std::min(g_Max.z, t_PosZ);
			g_Max.z = std::max(g_Max.z, t_PosZ);
		}

		// set the BB center point based on the min-max found
		ComputeCenterRadius(g_Min, g_Max);
	}

    /*************************************************************************/
    /*!
    \fn void AABB::operator = (AABB * t_AABB)

    \brief
        This function copies the AABB that is passed in.

    \param t_AABB
        This is the AABB object on the RHS.
    */
    /*************************************************************************/
    void AABB::operator = (AABB * t_AABB)
    {
        //this->m_BVMesh = t_AABB->m_BVMesh;
        //this->m_BVType = t_AABB->m_BVType;
        this->m_Center = t_AABB->m_Center;

        for(int i = 0; i < 3; ++i)
            this->m_Radius[i] = t_AABB->m_Radius[i];
    }

	bool AABB::IsPlanar()
	{
		return m_Radius.x <= EPSILON || m_Radius.y <= EPSILON || m_Radius.z <= EPSILON;
	}




	void AABB::SetMin(const vec3&min)
	{
		vec3 max;
		GetMaxVertex(max);
		m_Radius = (max - min) * 0.5f;
		m_Center = (min + max) * 0.5f;
	}

	void AABB::SetMax(const vec3& max)
	{
		vec3 min;
		GetMinVertex(min);
		m_Radius = (max - min) * 0.5f;
		m_Center = (min + max) * 0.5f;

	}

	void AABB::SetMin(u32 axisID, f32 position)
	{
		vec3 min,max;
		GetMinVertex(min);
		GetMaxVertex(max);
		min[axisID] = position;
		m_Radius = (max - min) * 0.5f;
		m_Center = (min + max) * 0.5f;

	}

	void AABB::SetMax(u32 axisID, f32 position)
	{
		vec3 min,max;
		GetMinVertex(min);
		GetMaxVertex(max);
		max[axisID] = position;
		m_Radius = (max - min) * 0.5f;
		m_Center = (min + max) * 0.5f;
	}



}
