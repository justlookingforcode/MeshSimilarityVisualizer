/* Start Header ------------------------------------------------------
    Copyright (C) 2014 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents 
    without the prior written consent of DigiPen Institute of 
    Technology is prohibited.

    File Name:      AABB.h
    Purpose:        This is where the AABB class implementations.
    Language:       C++
    Platform:       Windows Visual Studio
    Author:         Tan Wen De Kevin
    Creation Date:  24/Sep/2016
    - End Header -------------------------------------------------------*/

#ifndef _AABB_H_
#define _AABB_H_

// ==========================
// includes
// ==========================

#include "BoundingVolume.h"

// ==========================
// class/ function prototypes
// ==========================

namespace Proto
{
    class AABB//: public BV
    {
        public:
            
            AABB();

            const vec3      GetRadius() const;
            void            SetRadius(const vec3 & t_NewRadius);
			bool			IsPlanar();
            void            operator = (AABB * t_AABB);

            const vec3      GetMaxVertex() const;
            const vec3      GetMinVertex() const;
			vec3&			GetMinVertex(vec3& v) const;
			vec3&			GetMaxVertex(vec3& v) const;

			void SetMax(const vec3&);
			void SetMin(const vec3&);

			void SetMax(u32 axisID, f32 position);
			void SetMin(u32 axisID, f32 position);

            void            Create(const VertexBufferType & t_ModelVertexList);

			void            Create(const VertexBufferType & t_ModelVertexList,const  std::vector<int>& indicies);
			void            Create(const mat4& transform, const VertexBufferType & t_ModelVertexList);
            void            ComputeCenterRadius(const vec3 & t_Min,
                                                const vec3 & t_Max);
            
            void            UpdateAABB(const vec3 & t_ScaleVec, 
                                       const vec3 & t_Pos,
                                       const vec3 & t_RotVec,
                                       const AABB & t_ModelAABB);
			

            void UpdateAABB(const mat4& transform, const AABB &  t_ModelAABB);
			vec3            m_Radius;
            vec3            m_Center;

        private:
        
            
    };
}

#endif
