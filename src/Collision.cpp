/* Start Header ------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.

File Name:      Collision.cpp
Purpose:        This is where the Collision and Collision Response
function implementations are.
Language:       C++
Platform:       Windows Visual Studio
Author:         Tan Wen De Kevin
Creation Date:  19/Oct/2016
- End Header -------------------------------------------------------*/

#include "HierachicalAABB.h"
#include "HierachicalBS.h"
#include "BS.h"
#include "AABB.h"
#include "Collision.h"
#include "math.hpp"
#include <iostream>
#include <stdio.h>
#include <sstream>

namespace Proto
{
	const f32 OFFSET = 0.0002f;

	/************************************************************************** ********************//**
	* \fn  bool IsCollided(BV & t_BV1, BV & t_BV2)
	*
	* \brief   Checks for collision between any two bounding volumes.
	*
	*
	* \param [in,out]  t_BV1   The 1st bounding volume object.
	* \param [in,out]  t_BV2   The 2nd bounding volume object.
	*
	* \return  true if collided, false if not.
	**************************************************************************************************/
	


	bool IsCollided(BS & t_BV1, BS & t_BV2)
	{
		return SphereSphereCollision(t_BV1, t_BV2);
	}

	/**********************************************************************************************//**
	* \fn  bool SphereSphereCollision(BS & t_BS1, BS & t_BS2)
	*
	* \brief   Checks for sphere-sphere collision.
	*
	*
	* \param [in,out]  t_BS1   The 1st bounding sphere object.
	* \param [in,out]  t_BS2   The 2nd bounding sphere object.
	*
	* \return  true if it succeeds, false if it fails.
	**************************************************************************************************/
	bool SphereSphereCollision(BS & t_BS1, BS & t_BS2)
	{
		// Calculate squared distance between centers
		vec3 t_D = t_BS1.m_Center - t_BS2.m_Center;
		f32 t_Dist2 = Dot(t_D, t_D); // glm::dot(t_D, t_D);

		// Spheres interact if squared distance is leess than squared sum of radii
		f32 t_RadiusSum = t_BS1.GetRadius() + t_BS2.GetRadius();
		return t_Dist2 <= t_RadiusSum * t_RadiusSum;
	}

	/**********************************************************************************************//**
	* \fn  bool AABBAABBCollision(AABB & t_AABB1, AABB & t_AABB2)
	*
	* \brief   Checks for AABB-AABB collision.
	*
	*
	* \param [in,out]  t_AABB1 The 1st AABB object.
	* \param [in,out]  t_AABB2 The 2nd AABB object.
	*
	* \return  true if it succeeds, false if it fails.
	**************************************************************************************************/
	bool AABBAABBCollision(const AABB & t_AABB1, const AABB & t_AABB2)
	{
		vec3 t_PosDiff = glm::abs(t_AABB1.m_Center - t_AABB2.m_Center);
		f32 t_PosDiffX = fabs(t_PosDiff.x);
		f32 t_PosDiffY = fabs(t_PosDiff.y);
		f32 t_PosDiffZ = fabs(t_PosDiff.z);

		vec3 t_TotalRadius = t_AABB1.GetRadius() + t_AABB2.GetRadius();
		f32 t_TotalRadiusX = fabs(t_TotalRadius.x);
		f32 t_TotalRadiusY = fabs(t_TotalRadius.y);
		f32 t_TotalRadiusZ = fabs(t_TotalRadius.z);

		if ((t_PosDiffX > t_TotalRadiusX) ||
			(t_PosDiffY > t_TotalRadiusY) ||
			(t_PosDiffZ > t_TotalRadiusZ))
		{
			return false;
		}

		//std::cout << "AABB-AABB Collision!\n";
		return true;
	}

	/**********************************************************************************************//**
	* \fn  f32 SqDistPointAABB(const vec3 & t_BSCenter, const AABB & t_AABB)
	*
	* \brief   Sq distance point a bb.
	*
	*
	* \param   t_Point The point.
	* \param   t_AABB  The AABB object.
	*
	* \return  Square distance of float type
	**************************************************************************************************/
	f32 SqDistPointAABB(const vec3 & t_BSCenter, const AABB & t_AABB)
	{
		f32 t_SqDist = 0.f;

		for (int i = 0; i < TOTAL_AXIS; ++i)
		{
			vec3 t_AABBMin = t_AABB.GetMinVertex();
			vec3 t_AABBMax = t_AABB.GetMaxVertex();

			// For each axis count any excess distance outside box extents
			f32  t_CurrPtVal = t_BSCenter[i];

			if (t_CurrPtVal < t_AABB.GetMinVertex()[i])
			{
				f32 t_Diff = t_AABBMin[i] - t_CurrPtVal;
				t_SqDist += (t_Diff * t_Diff);
			}

			if (t_CurrPtVal > t_AABB.GetMaxVertex()[i])
			{
				f32 t_Diff = t_CurrPtVal - t_AABBMax[i];
				t_SqDist += (t_Diff * t_Diff);
			}
		}

		return t_SqDist;
	}

	/**********************************************************************************************//**
	* \fn  bool SphereAABBCollision(BS & t_BS, AABB & t_AABB)
	*
	* \brief   Checks for BS-AABB collision.
	*
	*
	* \param [in,out]  t_BS    The bounding sphere object.
	* \param [in,out]  t_AABB  The AABB object.
	*
	* \return  true if it succeeds, false if it fails.
	**************************************************************************************************/
	bool SphereAABBCollision(const BS & t_BS, const AABB & t_AABB)
	{
		f32 t_SqDist = SqDistPointAABB(t_BS.m_Center, t_AABB);
		f32 t_BSRadius = t_BS.GetRadius();
		f32 t_BSRadiusSq = t_BSRadius * t_BSRadius;

		// intersects if the dist. from center is > sphere radius
		return (t_SqDist <= t_BSRadiusSq + OFFSET);
	}



	/**********************************************************************************************//**
	* \fn  vec3 Lerp(const vec3 & t_Start, const vec3 & t_End, const f32 & t_Percent)
	*
	* \brief   Lerps.
	*
	*
	* \param   t_Start     The start.
	* \param   t_End       The end.
	* \param   t_Percent   The percent.
	*
	* \return  .
	**************************************************************************************************/
	vec3 Lerp(const vec3 & t_Start, const vec3 & t_End, const f32 & t_Percent)
	{
		return (t_Start + t_Percent * (t_End - t_Start));
	}

	/**********************************************************************************************//**
	* \fn  vec3 Slerp(const vec3 & t_Start, const vec3 & t_End, const f32 & t_Percent)
	*
	* \brief   Slerps.
	*
	*
	* \param   t_Start     The start.
	* \param   t_End       The end.
	* \param   t_Percent   The percent.
	*
	* \return  .
	**************************************************************************************************/
	vec3 Slerp(const vec3 & t_Start, const vec3 & t_End, const f32 & t_Percent)
	{
		f32 t_DotProd = glm::dot(t_Start, t_End);
		glm::clamp(t_DotProd, -1.0f, 1.0f);

		f32 t_Theta = glm::acos((t_DotProd)*t_Percent);
		vec3 t_RelativeVec = t_End - t_Start * t_DotProd;
		Normalise(t_RelativeVec);//glm::normalize(t_RelativeVec);

		return ((t_Start * glm::cos(t_Theta)) + (t_RelativeVec * glm::sin(t_Theta)));
	}

	/**********************************************************************************************//**
	* \fn  vec3 Nlerp(const vec3 & t_Start, const vec3 & t_End, const f32 & t_Percent)
	*
	* \brief   Nlerps.
	*
	*
	* \param   t_Start     The start.
	* \param   t_End       The end.
	* \param   t_Percent   The percent.
	*
	* \return  .
	**************************************************************************************************/
	vec3 Nlerp(const vec3 & t_Start, const vec3 & t_End, const f32 & t_Percent)
	{
		return Normalise(Lerp(t_Start, t_End, t_Percent));//glm::normalize(Lerp(t_Start, t_End, t_Percent));
	}

	/**********************************************************************************************//**
		* \fn   bool SphereOBBCollision(BS & t_BS, OBB & t_OBB)
		*
		* \brief   sphere obb collison
		*
		*
		* \param   bounding sphere
		* \param   aabb
		*
		* \return  .
		**************************************************************************************************/




	/*************************************************************************/
	/*!
	\fn f32 ScalarCrossProduct2(vec3& v0, vec3& v1, vec3& v2)

	\brief
	This absoluted scalar cross product needd  in dynamic OBB vs OBB test
	*/
	/*************************************************************************/
	f32 ScalarCrossProduct2(vec3& v0, vec3& v1, vec3& v2)
	{
		vec3 cp(Cross(v1, v2));
		return fabs(Dot(v0, cp));
	}


	bool IntersectRaySphere(const vec3& origin, const vec3& ray, const BS& sphere, f32&t)
	{

		vec3 m = origin - sphere.m_Center;
		f32 b = Dot(m, ray); // detect if ray is pointing away from the sphere
		f32 c = Dot(m, m) - (sphere.m_Radius * sphere.m_Radius); //detect if object is in the sphere, negative is in sphere
		if (c > 0.f && b> 0.f) return false; 
		f32 discr = b*b - c;
		if (discr < 0.f) return false;

		t = -b -sqrt(discr);
		t = (t < 0.f) ? 0.f : t;
		return true;

	}


	int CheckSign(const vec3& p0, const vec3& p1, const vec3& p2, const vec3& p3)
	{
		vec3 v10 = p1 - p0;
		vec3 v20 = p2 - p0;
		vec3 v30 = p3 - p0;
		float result = Dot(v10, Cross(v20,v30));
		if (result > 0)
		{
			return 1;
		}
		else if (result <= 0.1f && result >= -0.1f)
		{
			return 0;
		}
		else
			return -1;

	}



	bool IntersectRayAABB(const vec3& origin, const vec3& ray, const AABB& aabb, f32& tMin, vec3& intersect)
	{
		tMin = 0;
		f32 tMax= FLT_MAX;


		// For all three slabs
		for (int i = 0; i < 3; i++) {
			f32 e= aabb.m_Radius[i];
			f32 max = aabb.m_Center[i] + e;
			f32 min = aabb.m_Center[i] - e;
			if (fabs(ray[i]) < EPSILON) {
				// Ray is parallel to slab. No hit if origin not within slab

				if (origin[i] < min || origin[i] > max) return false;
			}
			else {
				// Compute intersection t value of ray with near and far plane of slab
				f32 ood = 1.0f / ray[i];
				f32 t1 = (min - origin[i]) * ood;
				f32 t2 = (max - origin[i]) * ood;
				// Make t1 be intersection with near plane, t2 with far plane
				if (t1 > t2) std::swap(t1, t2);
				// Compute the intersection of slab intersection intervals
				if (t1 > tMin) tMin = t1;
				if (t2 > tMax) tMax = t2;
				// Exit with no collision as soon as slab intersection becomes empty
				if (tMin > tMax) return false;
			}
		}
		// Ray intersects all 3 slabs. Return point (q) and intersection t value (tmin)
		intersect = origin + ray * tMin;
		return true;
	}








	/*************************************************************************/
	/*!
	\fn void getStaticExtremaProjection(std::vector<vec3>& t_Points, const vec3& axis, f32& min, f32& max)

	\brief
	project a static  obb onto an axis
	*/
	/*************************************************************************/
	void getStaticExtremaProjection(std::vector<vec3>& t_Points, const vec3& axis, f32& min, f32& max)
	{
		min = Dot(t_Points[0], axis);
		max = min;
		u32 total = t_Points.size();

		for (u32 i = 1; i < total; ++i)
		{
			f32 p = Dot(t_Points[i], axis);
			max = (std::max)(p, max);
			min = (std::min)(p, min);
		}
	}

	/*************************************************************************/
	/*!
	\fn void getDynamicExtremaProjection(std::vector<vec3>& t_Points, const vec3& vector, const vec3& axis, f32& min, f32& max)

	\brief
	project a dynamic  obb onto an axis
	*/
	/*************************************************************************/
	void getDynamicExtremaProjection(std::vector<vec3>& t_Points, const vec3& vector, const vec3& axis, f32& min, f32& max)
	{
		min = (std::numeric_limits<f32>::max)();
		max = -(std::numeric_limits<f32>::max)();
		u32 total = t_Points.size();

		for (u32 i = 0; i < total; ++i)
		{
			f32 p = Dot(t_Points[i], axis);
			max = (std::max)(p, max);
			min = (std::min)(p, min);

			p = Dot(t_Points[i] + vector, axis);
			max = (std::max)(p, max);
			min = (std::min)(p, min);
		}
	}









	/*************************************************************************/
	/*!
	\fn     bool IsCollided(BS& t_BS1, BS& t_BS2,
	vec3& t_V1, vec3& t_V2,
	f32 t_DtTimeBound, f32& t_IntersectionTime)

	\brief
	dynamic Sphere vs Sphere collsion test
	*/
	/*************************************************************************/
	bool IsCollided(BS& t_BS1, BS& t_BS2,
		vec3& t_V1, vec3& t_V2,
		f32 t_DtTimeBound, f32& t_IntersectionTime)
	{
		vec3 t_RelativeVelocity(t_V2 - t_V1);

		//<! treat BS 1 as a relative center, find vector between Sphere centres
		vec3 t_RelativePositionVector(t_BS1.m_Center - t_BS2.m_Center);
		BS t_RelativeSphere;
		t_RelativeSphere.m_Center = vec3(0, 0, 0);
		t_RelativeSphere.m_Radius = t_BS1.m_Radius + t_BS2.m_Radius;


		f32 t_direction = (Dot(-t_RelativePositionVector, t_RelativeVelocity));
		vec3 t_x;

		f32 t_lineSegmentSquareLength(Dot(t_RelativePositionVector, t_RelativePositionVector));
		if (t_direction <= 0)
		{
			t_IntersectionTime = 0.f; return true;
		}
		else if (t_direction >= t_lineSegmentSquareLength)
		{
			t_IntersectionTime = t_DtTimeBound;
			return true;
		}
		else
		{
			t_IntersectionTime = t_direction / t_lineSegmentSquareLength;
			return true;
		}

		return false;

	}


	/*************************************************************************/
	/*!
	\fn     bool IntersectSegmentPlane(const vec3& a, const vec3& ray, const Plane& p, float &t, vec3 &q)
	\brief
	line segment vs plane test
	*/
	/*************************************************************************/
	bool IntersectSegmentPlane(const vec3& a, const vec3& ray, const Plane& p, float &t, vec3 &q)
	{
		// Compute the t value for the directed line ab intersecting the plane
		vec3 ab = ray;
		vec3 b = a + ray;
		f32 i_t;
		vec3 n(p.a, p.b, p.c);
		f32 d = Dot(n, ab);
		f32 num = p.d - Dot(n, a);

		i_t = (num) / d;
		// If t in [0..1] compute and return intersection point
		if (i_t == i_t && i_t >= 0.0f && i_t <= 1.0f) {
			t = i_t;
			q = a + t * ab;
			return true;
		}
		// Else no intersection
		return false;
	}


	bool IntersectSegmentPlane(const vec3&  a, const vec3& ray, const vec3&  d, const vec3&  e, vec3&  f,
		float &t, vec3 &q)
	{
		Plane p;
		vec3 v = Cross(e - d, f - d);
		p.a = v.x;
		p.b = v.y;
		p.c = v.z;
		p.d = Dot(d, v);
		return IntersectSegmentPlane(a, ray, p, t, q);
	}
	/*************************************************************************/
	/*!
	\fn     bool IntersectSegmentPlane(const vec3& a, const vec3& ray, const Plane& p, float &t, vec3 &q)
	\brief
	line vs plane test
	*/
	/*************************************************************************/
	bool IntersectRayPlane(const vec3& a, const vec3& ray, const Plane& p, float &t, vec3 &q)
	{
		// Compute the t value for the directed line ab intersecting the plane
		vec3 b = a + ray;
		vec3 ab = ray;
		f32 i_t;
		vec3 n(p.a, p.b, p.c);
		f32 d = Dot(n, ray); // closest distance to triangle based on normal
		f32 num = p.d - Dot(n, a); // which side of the triangle is the origin of the ray
#if 0
		//<! return if ray is parallel to the plane, if a is within the plane  return true, else return false
		if (d == 0.0f && num == 0.f){ t = 0; q = a; return true; }
		else if (d == 0.0f) return false;
#endif
		i_t = (num) / d;
		// If t > 0 compute and return intersection point
		if (i_t == i_t && i_t >= 0.0f) {
			t = i_t;
			q = a + t * ray;
			return true;
		}
		// Else no intersection
		return false;
	}



	bool IntersectRayPlane(const vec3&  a, const vec3& ray, const vec3&  d, const vec3&  e, const vec3&  f,
		float &t, vec3 &q)
	{
		Plane p;
		vec3 v = Cross(e - d, f - d);
		p.a = v.x;
		p.b = v.y;
		p.c = v.z;
		p.d = Dot(v, d);
		return IntersectRayPlane(a, ray, p, t, q);
	}
	bool IsPointInRect(const vec2& t_point
		, const vec2& t_rectCenter
		, const vec2& t_rectSize)
	{
		//Check if point is inside rect
		if (t_point.x >= t_rectCenter.x - t_rectSize.x / 2.0f
			&& t_point.x <= t_rectCenter.x + t_rectSize.x / 2.0f
			&& t_point.y >= t_rectCenter.y - t_rectSize.y / 2.0f
			&& t_point.y <= t_rectCenter.y + t_rectSize.y / 2.0f
			)
		{
			//Return true if point is inside rect
			return true;
		}

		//else return false
		return false;
	}


	bool IntersectRayTriangle(
		const Vec3 &orig, const Vec3 &dir,
		const Vec3 &v0, const Vec3 &v1, const Vec3 &v2,
		f32 &t)
	{
		// compute plane's normal
		Vec3 v0v1 = v1 - v0;
		Vec3 v0v2 = v2 - v0;
		// no need to normalize
		Vec3 N = Cross(v0v1, v0v2); // N 
		N = Normalise(N);


		// check if ray and plane are parallel ?
		float NdotRayDirection = Dot(N,dir);
		if (fabs(NdotRayDirection) < EPSILON) // almost 0 , ray is othogonal to the notmal vector
			return false; // they are parallel so they don't intersect ! 

		
		// compute d parameter using equation 2
		f32 d = -Dot(N,v0);

		// compute time to intersect infinite plane equatin
		t = -(Dot(N,orig) + d) / NdotRayDirection;
		// check if the triangle is in behind the ray
		if (t < 0) 
			return false; // the triangle is behind 

		// compute the intersection point using equation 1
		Vec3 P = orig + t * dir;

		Vec3 R = P - v0;
		// edge 1
		Vec3 Q1 = v1 - v0;
		// edge 2
		Vec3 Q2 = v2 - v0;

		f32 d0 = Dot(R, Q1);
		f32 d1 = Dot(R, Q2);
		f32 q1q2 = Dot(Q1, Q2);
		f32 q1Sq = Dot(Q1, Q1);
		f32 q2Sq = Dot(Q2, Q2);

		glm::mat2 m;
		m[0][0] = q1Sq;
		m[0][1] = q1q2;
		m[1][0] = q1q2;
		m[1][1] = q2Sq;
		m = glm::inverse(m);
		vec2 v;
		v.x = d0;
		v.y = d1;

		v = m * v;
		if (v.x < 0 || v.y < 0 || v.x + v.y > 1.f) 
			return false;
		return true; // this ray hits the triangle 
	}



	void MarkAABBAsCollided(HierachicalAABB& t1, HierachicalAABB& t2, u32 i1, u32 i2)
	{
		if (i1 == -1 || i2 == -1) return;
		HierachicalAABBNode& n1(t1.nodes[i1]);
		HierachicalAABBNode& n2(t2.nodes[i2]);

		if (AABBAABBCollision(n1.m_AABB, n2.m_AABB))
		{
			n1.collided = true;
			n2.collided = true;
			MarkAABBAsCollided(t1, t2, n1.m_LeftChild, n2.m_LeftChild);
			MarkAABBAsCollided(t1, t2, n1.m_LeftChild, n2.m_RightChild);
			MarkAABBAsCollided(t1, t2, n1.m_RightChild, n2.m_LeftChild);
			MarkAABBAsCollided(t1, t2, n1.m_RightChild, n2.m_RightChild);
		}

	}


	void hAABBhAABBCollision(HierachicalAABB& t1, HierachicalAABB& t2)
	{
		std::for_each(t1.nodes.begin(), t1.nodes.end(), [](HierachicalAABBNode& n){n.collided = false; });
		std::for_each(t2.nodes.begin(), t2.nodes.end(), [](HierachicalAABBNode& n){n.collided = false; });

		u32 total1(t1.nodes.size()), total2(t2.nodes.size());
		u32 startIndex1(0);
		u32 startIndex2(0);
		MarkAABBAsCollided(t1, t2, startIndex1, startIndex2);


	}

	void MarkBSAsCollided(HierachicalBS& t1, HierachicalBS& t2, u32 i1, u32 i2)
	{

		if (i1 == -1 || i2 == -1) return;
		HierachicalBSNode& n1(t1.nodes[i1]);
		HierachicalBSNode& n2(t2.nodes[i2]);

		if (SphereSphereCollision(n1.m_BS, n2.m_BS))
		{
			n1.collided = true;
			n2.collided = true;
			MarkBSAsCollided(t1, t2, n1.m_LeftChild, n2.m_LeftChild);
			MarkBSAsCollided(t1, t2, n1.m_LeftChild, n2.m_RightChild);
			MarkBSAsCollided(t1, t2, n1.m_RightChild, n2.m_LeftChild);
			MarkBSAsCollided(t1, t2, n1.m_RightChild, n2.m_RightChild);
		}

	}


	void hBShBSCollision(HierachicalBS& t1, HierachicalBS& t2)
	{
		std::for_each(t1.nodes.begin(), t1.nodes.end(), [](HierachicalBSNode& n){n.collided = false; });
		std::for_each(t2.nodes.begin(), t2.nodes.end(), [](HierachicalBSNode& n){n.collided = false; });
		u32 total1(t1.nodes.size()), total2(t2.nodes.size());
		u32 startIndex1(0);
		u32 startIndex2(0);
		MarkBSAsCollided(t1, t2, startIndex1, startIndex2);

	}




}
