/* Start Header ------------------------------------------------------
    Copyright (C) 2014 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents 
    without the prior written consent of DigiPen Institute of 
    Technology is prohibited.

    File Name:      BS.h
    Purpose:        This is where the Bounding Sphere class 
                    implementations.
    Language:       C++
    Platform:       Windows Visual Studio
    Author:         Tan Wen De Kevin
    Creation Date:  24/Sep/2016
    - End Header -------------------------------------------------------*/

#ifndef _BOUNDINGSPHERE_H_
#define _BOUNDINGSPHERE_H_

// ==========================
// includes
// ==========================

#include "BoundingVolume.h"

// ==========================
// class/ function prototypes
// ==========================

namespace Proto
{
    class BS//: public BV
    {
        public:
            
            BS();
            ~BS();

            const f32 &     GetRadius() const;
            void            SetRadius(const f32 & t_NewRadius);

            void operator = (BS * t_BS);

            void            UpdateBS(const vec3 & t_ScaleVec, 
                                     const vec3 & t_Pos,
                                     const vec3 & t_RotVec, 
                                     const BS   & t_InBS);
            
            void            RitterSphere(BS &               t_BS, 
                                         VertexBufferType & t_VertCont, 
                                         int                t_NoOfPoints);

			void            RitterSphere(
										const VertexBufferType & t_VertCont,
										const std::vector<int>&   indices);
            
        //private:

            f32             m_Radius;
            vec3            m_Center;
            void            SphereFromDistantPoints(BS &                t_BS, 
                                                    const VertexBufferType &  t_VertCont, 
                                                    int                 t_NoOfPoints);
			void            SphereFromDistantPoints(BS &                t_BS,
				const VertexBufferType &  t_VertCont,
				const std::vector<int>&   indices);


            void            SphereOfSphereAndPt(BS      & t_BS, 
                                                const Vertex  & t_Vertex);

            void            MostSeparatedPointsOnAABB(int &                 min, 
                                                      int &                 max, 
                                                      const VertexBufferType &    t_VertCont, 
                                                      int                   t_NoOfPoints);

			void            MostSeparatedPointsOnAABB(int &                 min,
				int &                 max,
				const VertexBufferType &    t_VertCont,
				const std::vector<int>&   indices);
    };
}

#endif
