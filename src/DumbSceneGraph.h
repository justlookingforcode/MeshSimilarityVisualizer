/*  Start Header -------------------------------------------------------
File Name:      DumbSceneGraph.h
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/
#ifndef DUMB_SCENE_GRAPH_H
#define DUMB_SCENE_GRAPH_H
#include "ISceneGraph.h"



class DumbSceneGraph : public ISceneGraph
{
public:
	DumbSceneGraph();
	virtual ~DumbSceneGraph();
	virtual SceneObject* GetClosestObject(const vec3& origin, const vec3& ray, f32& timeOfIntersection);
	virtual SceneObject* GetClosestOccluder(const vec3& origin, const vec3& ray, const f32 lightDistance, f32& timeOfIntersection);
	virtual void InsertObject(SceneObject* object);

private:
	std::vector<SceneObject*> surfaces;
};
#endif