/* Start Header ------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.

File Name:      Collision.h
Purpose:        This is where the Collision and Collision Response
function implementations are.
Language:       C++
Platform:       Windows Visual Studio
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/

#ifndef _COLLISION_H_
#define _COLLISION_H_

// ==========================
// includes
// ==========================

#include "BS.h"
#include "AABB.h"
#include "Plane.h"
#include "HierachicalAABB.h"
#include "HierachicalBS.h"
#include "SceneObject.h"
// ==========================
// class/ function prototypes
// ==========================

namespace Proto
{
	// ===============================================
	// COLLISION FUNCTIONS HERE
	// ===============================================

	f32 SqDistPointAABB(const vec3 & t_BSCenter, const AABB & t_AABB);
	bool SphereSphereCollision(BS & t_BS1, BS & t_BS2);
	bool AABBAABBCollision(const AABB & t_AABB1, const AABB & t_AABB2);
	bool SphereAABBCollision(const BS & t_BS1, const AABB & t_AABB2);



	void hAABBhAABBCollision(HierachicalAABB& t1, HierachicalAABB& t2);
	void hBShBSCollision(HierachicalBS& t1, HierachicalBS& t2);


	bool IntersectRaySphere(const vec3& origin, const vec3& ray, const BS& sphere, f32& t);



	bool IntersectRayAABB(const vec3& origin, const vec3& ray,const  AABB& aabb, f32& tMin, vec3&intersect);


	bool IsCollided(BS& t_BS1, BS& t_BS2,
		vec3& t_V1, vec3& t_V2,
		f32 t_DtTimeBound, f32& t_IntersectionTime);


	// Check if point is in rect
	bool IsPointInRect(const vec2& t_point
		, const vec2& t_rectCenter
		, const vec2& t_rectSize);
	bool IntersectSegmentPlane(const vec3&  a, const vec3& ray, const vec3&  d, const vec3&  e, const vec3&  f,
		float &t, vec3 &q);
	bool IntersectSegmentPlane(const vec3& a, const vec3& b, const Plane& p, float &t, vec3 &q);
	bool IntersectRayPlane(const vec3&  a, const vec3& ray, const vec3&  d, const vec3&  e, const vec3&  f,
		float &t, vec3 &q);
	bool IntersectRayPlane(const vec3& a, const vec3& ray, const Plane& p, float &t, vec3 &q);
	bool IntersectRayTriangle(
		const Vec3 &orig, const Vec3 &dir,
		const Vec3 &v0, const Vec3 &v1, const Vec3 &v2,
		f32 &t);
	
	// ===============================================
	// COLLISION RESPONSE FUNCTIONS HERE
	// ===============================================

	vec3 Lerp(const vec3 & t_Start, const vec3 & t_End, const f32 & t_Percent);
	vec3 Slerp(const vec3 & t_Start, const vec3 & t_End, const f32 & t_Percent);
	vec3 Nlerp(const vec3 & t_Start, const vec3 & t_End, const f32 & t_Percent);
	/*
	bool IsNotDegenerateVector(const vec3& v);

	void getStaticExtremaProjection(std::vector<vec3>& t_Points, const vec3& axis, f32& min, f32& max);

	*/
}

#endif
