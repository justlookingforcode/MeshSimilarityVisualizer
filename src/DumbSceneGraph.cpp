/*  Start Header -------------------------------------------------------
File Name:      DumbSceneGraph.cpp
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/

#include "DumbSceneGraph.h"
#include "SceneObject.h"
#include "Collision.h"
DumbSceneGraph::DumbSceneGraph()
{
}



DumbSceneGraph::~DumbSceneGraph()
{
}



SceneObject* DumbSceneGraph::GetClosestObject(const vec3& origin, const vec3& ray, f32& timeOfIntersection)
{
	u32 total = surfaces.size();
	SceneObject *o(nullptr);

	f32 smallestT = FLT_MAX, ti = FLT_MAX;
	for (u32 i = 0; i < total; ++i)
	{
		/*
		bool collided = false;
		SceneObject *to = surfaces[i];

		collided = to->IntersectionTest(origin, ray, ti);

		if (o == to) continue;
		else if (collided && ti < smallestT)
		{
			timeOfIntersection = smallestT = ti;
			o = to;
		}
		*/
	}
	
	return o;
}




SceneObject* DumbSceneGraph::GetClosestOccluder(const vec3& origin, const vec3& ray, const f32 lightDistance, f32& timeOfIntersection)
{
	u32 total = surfaces.size();
	SceneObject *o(nullptr);

	f32 smallestT = FLT_MAX, ti = FLT_MAX;
	for (u32 i = 0; i < total; ++i)
	{
		/*
		bool collided = false;
		SceneObject *to = surfaces[i];

		collided = to->IntersectionTest(origin, ray, ti);

		if (o == to) continue;
		else if (collided && ti < smallestT && ti < lightDistance)
		{
			timeOfIntersection = smallestT = ti;
			o = to;
			return o;
		}
		*/
	}
	return o;
}


void DumbSceneGraph::InsertObject(SceneObject* object)
{
	surfaces.push_back(object);
}

