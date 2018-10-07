/*  Start Header -------------------------------------------------------
File Name:      ISceneGraph.h
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/
#ifndef I_SCENE_GRAPH
#define	I_SCENE_GRAPH
#include "math.hpp"
class SceneObject;

class ISceneGraph
{
public:
	//ISceneGraph(){};
	virtual ~ISceneGraph(){};
	virtual SceneObject* GetClosestObject(const vec3& origin, const vec3& ray, float& timeOfIntersection) = 0;
	virtual SceneObject* GetClosestOccluder(const vec3& origin, const vec3& ray, const f32 lightDistance, float& timeOfIntersection) = 0;
	virtual void InsertObject(SceneObject *pObject) = 0;
};
#endif