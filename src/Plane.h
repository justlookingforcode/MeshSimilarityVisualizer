/* Start Header ------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.

File Name:      Plane.h
Purpose:        This is where the * Plane class implementations.
Language:       C++
Platform:       Windows Visual Studio
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/

#ifndef PLANE_H_
#define PLANE_H_

// ==========================
// includes
// ==========================

#include "defines.h"

// ==========================
// class/ function prototypes
// ==========================

namespace Proto
{
#define SMALL_NUM       0.00000001  // anything that avoids division overflow
#define NO_OF_PLANES    6           // no. of planes

	// Line class
	struct Line
	{
		vec3    m_Point;
		vec3    m_Vector;

		Line()
		{
			m_Point.x = 0;
			m_Point.y = 0;
			m_Point.z = 0;

			m_Vector.x = 0;
			m_Vector.y = 0;
			m_Vector.z = 0;
		}
	};

	// Plane class
	class Plane
	{
	public:

		f32       a; // a = n.x
		f32       b; // b = n.y
		f32       c; // c = n.z
		f32       d; // d = -(dot(n,p0))

		Plane();
		Plane(const vec3 & t_PtA,
			const vec3 & t_PtB,
			const vec3 & t_PtC);

		Plane &     operator = (const Plane & p);
		Plane &     operator = (const vec4 & p);

		int         Intersect2Planes(Plane & t_Plane1,
			Plane & t_Plane2,
			Line  & t_Line);

		int         Intersect3Planes(Plane & t_Plane1,
			Plane & t_Plane2,
			Plane & t_Plane3,
			vec3  & t_Point);
		void ComputePlane(const vec3 & t_PtA,
			const vec3 & t_PtB,
			const vec3 & t_PtC);
	private:


	};

}

#endif
