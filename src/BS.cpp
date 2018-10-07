/* Start Header ------------------------------------------------------
    Copyright (C) 2014 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents 
    without the prior written consent of DigiPen Institute of 
    Technology is prohibited.

    File Name:      BS.cpp
    Purpose:        This is where the Bounding Sphere class *
                    implementations.
    Language:       C++
    Platform:       Windows Visual Studio
    Author:         Tan Wen De Kevin
    Creation Date:  24/Sep/2016
    - End Header -------------------------------------------------------*/

#include "BS.h"
#include "Math.h"
#include "Conversion.h"

namespace Proto
{
    /*************************************************************************/
    /*!
    \fn BS::BS()

    \brief
        This is the default constructor of the Bounding Sphere (BS) class.
    */
    /*************************************************************************/
    BS::BS()
    {
        //m_BVType = TYPE_BS;
    }

    /*************************************************************************/
    /*!
    \fn BS::~BS()

    \brief
        This is the destructor of the Bounding Sphere (BS) class.
    */
    /*************************************************************************/
    BS::~BS()
    {
        //m_BVType        = TYPE_NONE;
        this->m_Radius  = 0.f;
    }

    /*************************************************************************/
    /*!
    \fn void BS::RitterSphere(BS &                  t_BS, 
                              VertexBufferType &    t_VertCont, 
                              int                   t_NoOfPoints)

    \brief
        This function builds/constructs a bounding sphere via the Ritter's method.

    \param t_BS
        This is the object's bounding sphere.

    \param t_VertCont
        This is the vertex buffer of the object model.

    \param t_NoOfPoints
        This is the size of the vertex buffer of the object model.
    */
    /*************************************************************************/
    void BS::RitterSphere(BS &                  t_BS, 
                          VertexBufferType &    t_VertCont, 
                          int                   t_NoOfPoints)
    {
        // Get sphere encompassing two approximately most distant points
        SphereFromDistantPoints(t_BS, t_VertCont, t_NoOfPoints);

        // Grow sphere to include all points
        for (s32 i = 0; i < t_NoOfPoints; ++i)
            SphereOfSphereAndPt(t_BS, t_VertCont[i]);
    }




	void  BS::RitterSphere(
		const VertexBufferType & t_VertCont,
		const std::vector<int>&   indices)
	{
		// Get sphere encompassing two approximately most distant points
		SphereFromDistantPoints(*this, t_VertCont, indices);

		// Grow sphere to include all points
		u32 total(indices.size());
		for (u32 i = 0; i < total; ++i)
			SphereOfSphereAndPt(*this, t_VertCont[indices[i]]);
	}

    /*************************************************************************/
    /*!
    \fn void BS::SphereFromDistantPoints(BS &               t_BS, 
                                         VertexBufferType & t_VertCont, 
                                         int                t_NoOfPoints)

    \brief
        This function get sphere encompassing two approximately most 
        distant points.

    \param t_BS
        This is the object's bounding sphere.

    \param t_VertCont
        This is the vertex buffer of the object model.

    \param t_NoOfPoints
        This is the size of the vertex buffer of the object model.
    */
    /*************************************************************************/
    void BS::SphereFromDistantPoints(BS &       t_BS, 
                                     const VertexBufferType & t_VertCont, 
                                     int        t_NoOfPoints)
    {
        // Find the most separated point pair defining the encompassing AABB
        int min, max;
		if (t_NoOfPoints == 0) return;
        MostSeparatedPointsOnAABB(min, max, t_VertCont, t_NoOfPoints);

        // Set up sphere to just encompass these two points
        Vertex t_MinPt   = t_VertCont[min];
        Vertex t_MaxPt   = t_VertCont[max];

        t_BS.m_Center = (t_MinPt.pos + t_MaxPt.pos) * 0.5f;
        vec3 t_RadVec = t_MaxPt.pos - t_BS.m_Center;
        t_BS.m_Radius   = glm::sqrt(glm::dot(t_RadVec, t_RadVec));
    }

	void BS::SphereFromDistantPoints(BS &       t_BS,
		const VertexBufferType & t_VertCont,
		const std::vector<int>&indicies)
	{
		// Find the most separated point pair defining the encompassing AABB
		int min, max;
		if (indicies.size() == 0) return;
		MostSeparatedPointsOnAABB(min, max, t_VertCont, indicies);

		// Set up sphere to just encompass these two points
		Vertex t_MinPt = t_VertCont[min];
		Vertex t_MaxPt = t_VertCont[max];

		t_BS.m_Center = (t_MinPt.pos + t_MaxPt.pos) * 0.5f;
		vec3 t_RadVec = t_MaxPt.pos - t_BS.m_Center;
		t_BS.m_Radius = glm::sqrt(glm::dot(t_RadVec, t_RadVec));
	}

    /*************************************************************************/
    /*!
    \fn void BS::SphereOfSphereAndPt(BS & t_BS, Vertex & t_Vertex)

    \brief
        This function grows sphere to include all points

    \param t_BS
        This is the object's bounding sphere.

    \param t_Vertex
        This is the vertex to be included into the sphere.
    */
    /*************************************************************************/
    void BS::SphereOfSphereAndPt(BS & t_BS, const Vertex & t_Vertex)
    {
        // Compute squared distance between point and sphere center
        vec3 d      = t_Vertex.pos - t_BS.m_Center;
        f32 dist2   = glm::dot(d, d);

        // Only update s if point p is outside it
        f32 t_RadiusSq = t_BS.m_Radius * t_BS.m_Radius;
        
        if(dist2 > t_RadiusSq)
        {
            f32 dist      = glm::sqrt(dist2);
            f32 newRadius = (t_BS.m_Radius + dist) * 0.5f;
            f32 k         = (newRadius - t_BS.m_Radius) / dist;
            t_BS.m_Radius   = newRadius;
            t_BS.m_Center   += d * k;
        }
    }

    /**********************************************************************************************//**
     * \fn  void BS::MostSeparatedPointsOnAABB(int & t_Min, 
     * 		                                   int & t_Max,
     *                                         VertexBufferType & t_VertCont, 
     *                                         int t_NoOfPoints)
     *
     * \brief   Most separated point pair defining the encompassing AABB.
     *
     * \date    18/9/2014
     *
     * \param [in,out]  t_Min       The minimum point.
     * \param [in,out]  t_Max       The maximum point.
     * \param [in,out]  t_VertCont  The vertex buffer container.
     * \param   t_NoOfPoints        The size of the vertex buffer of the object model.
     **************************************************************************************************/
    void BS::MostSeparatedPointsOnAABB(int &                t_Min, 
                                       int &                t_Max, 
                                       const VertexBufferType &   t_VertCont, 
                                       int                  t_NoOfPoints)
    {
        // First find most extreme points along principal axes
        int     minx = 0, maxx = 0, 
                miny = 0, maxy = 0, 
                minz = 0, maxz = 0;
		if (t_NoOfPoints == 0) return;
        for(int i = 1; i < t_NoOfPoints; ++i) 
        {
            vec3 t_CurrPt = t_VertCont[i].pos;

            if (t_CurrPt.x < t_VertCont[minx].pos.x)
                minx = i;

            if (t_CurrPt.x > t_VertCont[maxx].pos.x)
                maxx = i;

            if (t_CurrPt.y < t_VertCont[miny].pos.y)
                miny = i;

            if (t_CurrPt.y > t_VertCont[maxy].pos.y)
                maxy = i;

            if (t_CurrPt.z < t_VertCont[minz].pos.z)
                minz = i;

            if (t_CurrPt.z > t_VertCont[maxz].pos.z)
                maxz = i;
        }

        // Compute the squared distances for the three pairs of points
        vec3 t_DiffX = t_VertCont[maxx].pos - t_VertCont[minx].pos;
        vec3 t_DiffY = t_VertCont[maxy].pos - t_VertCont[miny].pos;
        vec3 t_DiffZ = t_VertCont[maxz].pos - t_VertCont[minz].pos;

        f32 dist2x = glm::dot(t_DiffX, t_DiffX);
        f32 dist2y = glm::dot(t_DiffY, t_DiffY);
        f32 dist2z = glm::dot(t_DiffZ, t_DiffZ);

        // Pick the pair (min,max) of points most distant
        t_Min = minx;
        t_Max = maxx;
        
        if(dist2y > dist2x && dist2y > dist2z) 
        {
            t_Max = maxy;
            t_Min = miny;
        }

        if(dist2z > dist2x && dist2z > dist2y) 
        {
            t_Max = maxz;
            t_Min = minz;
        }
    }


	void BS::MostSeparatedPointsOnAABB(int &                t_Min,
		int &                t_Max,
		const VertexBufferType &   t_VertCont,
		const std::vector<int>&indicies)
	{
		// First find most extreme points along principal axes
		int minx = indicies[0], maxx = indicies[0],
			miny = indicies[0], maxy = indicies[0],
			minz = indicies[0], maxz = indicies[0];
		s32 t_NoOfPoints = indicies.size();
		if (indicies.size() == 0) return;
		for (int i = 1; i < t_NoOfPoints; ++i)
		{
			vec3 t_CurrPt = t_VertCont[indicies[i]].pos;

			if (t_CurrPt.x < t_VertCont[minx].pos.x)
				minx = indicies[i];

			if (t_CurrPt.x > t_VertCont[maxx].pos.x)
				maxx = indicies[i];

			if (t_CurrPt.y < t_VertCont[miny].pos.y)
				miny = indicies[i];

			if (t_CurrPt.y > t_VertCont[maxy].pos.y)
				maxy = indicies[i];

			if (t_CurrPt.z < t_VertCont[minz].pos.z)
				minz = indicies[i];

			if (t_CurrPt.z > t_VertCont[maxz].pos.z)
				maxz = indicies[i];
		}

		// Compute the squared distances for the three pairs of points
		vec3 t_DiffX = t_VertCont[maxx].pos - t_VertCont[minx].pos;
		vec3 t_DiffY = t_VertCont[maxy].pos - t_VertCont[miny].pos;
		vec3 t_DiffZ = t_VertCont[maxz].pos - t_VertCont[minz].pos;

		f32 dist2x = glm::dot(t_DiffX, t_DiffX);
		f32 dist2y = glm::dot(t_DiffY, t_DiffY);
		f32 dist2z = glm::dot(t_DiffZ, t_DiffZ);

		// Pick the pair (min,max) of points most distant
		t_Min = minx;
		t_Max = maxx;

		if (dist2y > dist2x && dist2y > dist2z)
		{
			t_Max = maxy;
			t_Min = miny;
		}

		if (dist2z > dist2x && dist2z > dist2y)
		{
			t_Max = maxz;
			t_Min = minz;
		}
	}



    void BS::operator = (BS * t_BS)
    {
        //this->m_BVMesh = t_BS->m_BVMesh;
        //this->m_BVType = t_BS->m_BVType;
        this->m_Center = t_BS->m_Center;
        this->m_Radius = t_BS->m_Radius;
    }

    /**********************************************************************************************//**
     * \fn  const f32 & BS::GetRadius() const
     *
     * \brief   Gets the sphere radius.
     *

     * \date    18/9/2014
     *
     * \return  The sphere radius.
     **************************************************************************************************/
    const f32 & BS::GetRadius() const
    {
        return this->m_Radius;
    }

    /**********************************************************************************************//**
     * \fn  void BS::SetRadius(const f32 & t_NewRadius)
     *
     * \brief   Sets the radius.
     *

     * \date    18/9/2014
     *
     * \param   t_NewRadius The new radius.
     **************************************************************************************************/
    void BS::SetRadius(const f32 & t_NewRadius)
    {
        this->m_Radius = t_NewRadius;
    }

    /*************************************************************************/
    /*!
    \fn void BS::UpdateBS(vec3 &        t_ScaleVec, 
                          vec3 &        t_TransVec,
                          vec3 &        t_RotVec,
                          const BS &    t_InBS)

    \brief
        This function updates the sphere.

    \param t_ScaleVec
        This is the scale vector.

    \param t_TransVec
        This is the translation vector.

    \param t_RotVec
        This is the rotation vector.

    \param t_InBS
        This is the initial/original model bounding sphere.
    */
    /*************************************************************************/
    void BS::UpdateBS(const vec3 &  t_ScaleVec, 
                      const vec3 &  t_Pos,
                      const vec3 &  t_RotVec,
                      const BS &    t_InBS)
    {
        // build transformation matrix
        mat4 t_ScaleMat         = ScaleMatrix(t_ScaleVec);
        mat4 t_RotMat           = RotationMatrix(t_RotVec.x, t_RotVec.y, t_RotVec.z); 
        mat4 t_MatResult        = TransformationMatrix(t_RotMat, t_ScaleMat);

        vec3 t_BSModelCenter    = t_InBS.m_Center;
        vec4 t_NewCenter        = t_MatResult * ConvertVec3ToVec4(t_BSModelCenter);
        

        // check for the largest scale to be used as the radius
        f32 t_LargeScale        = (t_ScaleVec.x > t_ScaleVec.y) ? t_ScaleVec.x : t_ScaleVec.y;
        t_LargeScale            = (t_LargeScale > t_ScaleVec.z) ? t_LargeScale : t_ScaleVec.z;
        this->m_Radius          = t_InBS.m_Radius * t_LargeScale;
		this->m_Center = ConvertVec4ToVec3(t_NewCenter) + t_Pos;
    }
}
