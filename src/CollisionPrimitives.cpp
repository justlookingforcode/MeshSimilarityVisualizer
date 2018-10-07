/******************************************************************************/
/*!
\file       CollisionPrimitives.cpp
\author     Tan Wen De Kevin

\date       September 06, 2014
October 09, 2014

*/
/******************************************************************************/

#include "CollisionPrimitives.h"
#include "Conversion.h"

namespace Proto
{
	/******************************************************************************/
	/*!
	\brief
	Test if ray and sphere intersects, if yes, returns the time of intersection t
	and intersection point q
	\param p
	ray is calculated with the parametric eqn p + td
	\param d
	the vector of the ray
	\param s
	the sphere that we are checking intersection against
	\param t
	the intersecting value
	\param q
	the point of intersection
	*/
	/******************************************************************************/
	bool RaySphereIntersect(vec3    & p,            // ray calculated with the parametric eq: p + td
		vec3    & t_Ray,        // ray vector
		BS      & t_BS,
		f32     & t_IntTime,    // intersection time
		vec3    & t_IntPt)      // intersection pt
	{
		vec3 m = p - t_BS.m_Center;
		f32 t_BSRadius = t_BS.GetRadius();

		f32 b = Dot(m, t_Ray);
		f32 c = Dot(m, m) - (t_BSRadius * t_BSRadius);

		// ray's origin outside sphere,  (c > 0)
		// ray pointing away from sphere (b > 0)
		if (c > 0.f && b > 0.f)
			return false;

		vec3 e = Normalise(t_Ray);

		f32 f = Dot(m, e);
		f32 discriminant = f * f - c;

		// if discriminant is negative means the ray miss the sphere
		if (discriminant < 0.f)
			return false;

		// Ray intersects sphere, smallest time of intersection is calculated
		t_IntTime = -b - sqrtf(discriminant);

		// negative t value means ray started inside sphere, clamp it to zero
		if (t_IntTime < 0.f)
			t_IntTime = 0.f;

		// update point of intersection of ray with sphere
		t_IntPt = p + t_IntTime * t_Ray;

		return true;
	}
   

}
