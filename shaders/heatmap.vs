/******************************************************************************/
/*!
\file   main.vs
\par    Purpose: Library for generating polygonal meshes.
\par    Language: GLSL
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Kevin Tan
\date   8/04/2016
*/
/******************************************************************************/
#version 330 core

/*  These vertex attributes are in model space */
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vTan;
layout (location = 3) in vec3 vBitan;
layout (location = 4) in vec2 vUV;
layout (location = 5) in vec2 vHeapMapUV;
uniform mat4 mvMat;     /*  model-view matrix for positions */
uniform mat4 nmvMat;    /*  model-view matrix for normals */
uniform mat4 projMat;   /*  projection matrix */

varying vec3 iNormal; 
varying vec2 iHeapMapUV; 
void main() {
    gl_Position = projMat * mvMat* vec4(vPosition,1.0); 
    iNormal=vec3(mvMat *vec4(vNormal,0)); 
    iNormal = normalize(iNormal);
    iHeapMapUV=vHeapMapUV;
}