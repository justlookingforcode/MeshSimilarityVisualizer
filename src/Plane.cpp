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

#include "Plane.h"
#include <cfloat>

namespace Proto
{
	Plane::Plane() : a(0.f), b(0.f), c(0.f), d(0.f)
	{
	}

	Plane::Plane(const vec3 & t_PtA,
		const vec3 & t_PtB,
		const vec3 & t_PtC)
	{
		ComputePlane(t_PtA, t_PtB, t_PtC);
	}

	Plane & Plane::operator = (const Plane & p)
	{
		a = p.a;
		b = p.b;
		c = p.c;
		d = p.d;

		return *this;
	}

	Plane & Plane::operator=(const glm::vec4 & p)
	{
		a = p.x;
		b = p.y;
		c = p.z;
		d = p.w;

		return *this;
	}

	void Plane::ComputePlane(const glm::vec3 & t_PtA,
		const glm::vec3 & t_PtB,
		const glm::vec3 & t_PtC)
	{
		glm::vec3 t_VecV = t_PtB - t_PtA;               // unit vector v
		glm::vec3 t_VecU = t_PtC - t_PtA;               // unit vector u
		glm::vec3 t_VecW = glm::normalize(glm::cross(t_VecV, t_VecU));  // unit vector w = v X u

		this->a = t_VecW.x;
		this->b = t_VecW.y;
		this->c = t_VecW.z;
		this->d = glm::dot(t_VecW, t_PtA);
	}

	int Plane::Intersect2Planes(Plane & t_Plane1,
		Plane & t_Plane2,
		Line  & t_Line)
	{
		// extract the normal of the 2 planes
		vec3 t_N1(t_Plane1.a, t_Plane1.b, t_Plane1.c);
		vec3 t_N2(t_Plane2.a, t_Plane2.b, t_Plane2.c);

		// compute direction of intersection line
		t_Line.m_Vector = glm::cross(t_N1, t_N2);

		// If d is (near) zero, the planes are parallel (and separated)
		// or coincident, so they’re not considered intersecting
		f32 t_Denom = glm::dot(t_Line.m_Vector, t_Line.m_Vector);

		if (t_Denom < FLT_EPSILON)
			return 0;

		// Compute point on intersection line
		glm::vec3 t_TmpVec = t_Plane1.d * t_N2 - t_Plane2.d * t_N1;
		t_Line.m_Point = glm::cross(t_TmpVec, t_Line.m_Vector) / t_Denom;

		return 1;
	}

	int Plane::Intersect3Planes(Plane       & t_Plane1,
		Plane       & t_Plane2,
		Plane       & t_Plane3,
		glm::vec3   & t_Point)
	{
		// extract the normal of the 3 planes
		vec3 t_N1(t_Plane1.a, t_Plane1.b, t_Plane1.c);
		vec3 t_N2(t_Plane2.a, t_Plane2.b, t_Plane2.c);
		vec3 t_N3(t_Plane3.a, t_Plane3.b, t_Plane3.c);

		vec3 t_VecU = glm::cross(t_N2, t_N3);
		f32 t_Denom = glm::dot(t_N1, t_VecU);

		if (fabs(t_Denom) < FLT_EPSILON)
			return 0; // Planes do not intersect in a point

		vec3 t_TmpVec = t_Plane3.d * t_N2 - t_Plane2.d * t_N3;
		vec3 t_VecW = glm::cross(t_N1, t_TmpVec);
		t_Point = (t_Plane1.d * t_VecU + t_VecW) / t_Denom;
		return 1;
	}
}
