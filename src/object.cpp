/******************************************************************************/
/*!
\file   object.cpp
\par    Purpose: Definitions for scene objects
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\date   27/3/2015
*/
/******************************************************************************/

#include "object.hpp"

/*  Pre-defined meshes */
Mesh mesh[1] =
{

};


/*  Pre-defined objects */
std::vector<Object> obj;

/*  Mirror and sphere positions, which are used in graphics.cpp for rendering scene from these objects */
Vec3 mirrorTranslate;
Vec3 spherePos;


/*  Light pos are defined in world frame, but we need to compute their pos in view frame for
    lighting. In this frame, the vertex positions are not too large, hence the computation
    is normally more accurate.
*/
Vec3 lightPosWF[NUM_LIGHTS], lightPosVF[NUM_LIGHTS];


/*  Setting up the light position */
void SetUpLight(float height);


/******************************************************************************/
/*!
\fn     void SetUpScene()
\brief
        Set up all objects in the scene.
*/
/******************************************************************************/
void SetUpScene()
{
	Vec3 baseSize = Vec3(7.0f, 0.5f, 7.0f);


    SetUpLight(baseSize.x*50.f);
}


/******************************************************************************/
/*!
\fn     void SetUpLight(float height)
\brief
        Set up the light at (0, height, 0).
*/
/******************************************************************************/
void SetUpLight(float height)
{
    lightPosWF[0] = Vec3(0, height, 0);
	lightPosWF[1] = Vec3(height, height, 0);
	lightPosWF[2] = Vec3(-height, height, 0);
	lightPosWF[3] = Vec3(0, height, -height);
	lightPosWF[4] = Vec3(0, height, height);
}