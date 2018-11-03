/******************************************************************************/
/*!
\file   graphics.hpp
\par    Purpose: Declaring the graphics-related variables & functions
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit

\date   31/3/2015
*/
/******************************************************************************/

#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP


#include "gl/gl3w.h"
#include "gl/freeglut.h"
#include "AntTweakBar.h"


#include "mesh.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "plane.h"


#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "AntTweakBar.lib")


/*  For displaying frame-per-second */
extern float fps;
extern GLint mainMVMatLoc, mainNMVMatLoc, mainProjMatLoc, objectColorLoc;  /*  used for main program */

extern size_t windowWidth, windowHeight;


extern bool drawWireFrame;

extern GLint textureLoc;
extern GLint textureOnLoc;
extern GLint fpsLoc;

/*  Pre-defined camera */
extern Camera mainCam;

struct ActiveTexID
{
	enum { COLOR = 0, NORMAL, NORMAL_2 };
};

struct ProgType
{
	enum { MAIN_PROG = 0, HEAT_MAP_PROG,  NUM_PROGTYPES };
};

/*  Public functions */
void SetUp();
void CleanUp();
void Resize(int w, int h);
void Render();
void ComputeObjMVMat(Mat4& MVMat, Mat4& NMVMat, const Mat4& viewMat, const Mat4& modelMat);
void RenderMeshObj(const Mesh &obj, bool wireframe = false);
void UpdateGPUMesh(const Mesh &obj);

void SendMVMat(const Mat4 &mvMat, const Mat4 &nmvMat, GLint mvMatLoc, GLint nmvMatLoc);
void SendObjectColor(const vec3& color, GLint objectColorLoc);
void ComputeObjMVMat(Mat4& MVMat, Mat4& NMVMat, const Mat4& viewMat, const Mat4& modelMat);
void LoadResources();
void SendObjTexID(GLuint texID, int activeTex, GLint texLoc);

//My helper functions
void LoadSceneObj(void* p_go, str sModelName, str SOinstID,
    vec3 pos, vec3 scale, vec3 rot, s32 imgID);


void TW_CALL ToggleDrawWireFrame(void *);
void TW_CALL ToggleRenderingMode(void *);

void TW_CALL SetControlledObjAsCamera(void *);
void TW_CALL SetControlledObjAsSceneObj0(void *);
void TW_CALL SetControlledObjAsSceneObj1(void *);

void TW_CALL SetControlledObjAsSceneObj2(void *);
void TW_CALL ToggleBoundingVolumeVisibility(void *);
void TW_CALL IncrementDepth(void *);
void TW_CALL DecrementDepth(void *);

extern u8 u8CurrentBSPDepth;
extern bool drawBoundingVolumes;
#endif