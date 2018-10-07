/******************************************************************************/
/*!
\file       CollisionPrimitives.h
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016

*/
/******************************************************************************/

#ifndef _COLLISION_PRIMITIVES_H_
#define _COLLISION_PRIMITIVES_H_

// ==========================
// includes
// ==========================

#include "Plane.h"
#include "BS.h"
#include "AABB.h"
//#include "OBB.h"

// ==========================
// includes
// ==========================

/*
* forward declaration for a 2d vector, has 3 components x,y,z
* wrap with a typedef so that you can more easily integrate this with other peoples libaries
*/
namespace Proto
{
	/**
	* Arbitary convex polygon
	* do this last, it might or might not be needed for your 2d game and is the most troublesome to implement
	* interface is probably wrong to be confirmed
	*/
	struct ColPolygon
	{
		vec3        centre;
		VEC3CONT    vertices;
	};

	/**
	* this struct wraps around any collision information that you might need to return and process
	* this can be useful for other scripts that handle collision response.
	*/
	struct CollisionData
	{
		vec3    point;
		vec3    q;      // intersection point
		f32     t;       // intesection values
	};

	// Check if ray intersects Sphere
	bool RaySphereIntersect(vec3    & p,            // ray calculated with the parametric eq: p + td
		vec3    & t_Ray,        // ray vector
		BS      & t_BS,
		f32     & t_IntTime,    // intersection time
		vec3    & t_IntPt);      // intersection pt

	// Check if ray intersects Plane
	bool RayPlaneIntersect(vec3  & t_Pt1,
		vec3  & t_Pt2,
		Plane & t_Plane,
		f32   & t_IntTime,   // intersection time
		vec3  & t_IntPt);    // intersection pt

	// shifted to Conversion.cpp
	vec3 GetMouseWorldPos(mat4 & t_ProjMatrix,
		mat4 & t_ViewMatrix,
		const vec2 & t_MousePos,
		mat4 & t_ScreenToNDC);

	vec3 GetMouseWorldRay(mat4 & t_ProjMatrix,
		mat4 & t_ViewMatrix,
		vec2 & t_MousePos,
		mat4 & t_ScreenToNDC,
		vec3 & t_NearPlanePosWorld);
}

#endif
