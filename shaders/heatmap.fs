/******************************************************************************/
/*!
\file   main.fs
\par    Purpose: Library for generating polygonal meshes.
\par    Language: GLSL
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\author Kevin Tan
\date   8/04/2016
*/
/******************************************************************************/

#version 330 core

uniform sampler2D colorTex;     /*  Base color texture */
uniform sampler2D normalTex;    /*  Normal texture for normal mapping */
uniform sampler2D bumpTex;      /*  Bump texture for bump mapping */

precision mediump float;
varying vec3 iNormal; 
varying vec2 iHeapMapUV; 

void main() {

    vec3 color;

    
    if(iHeapMapUV.x > 0.5)
    {
        float percent = (iHeapMapUV.x - 0.5) / 0.5;
        float red = 2.0 * min(percent, 0.5);
        float green = 2.0 * min(1.0 - percent, 0.5);
        float blue = 0.0;
        color = vec3(red,green,blue);
    }
    else
    {
        float percent = iHeapMapUV.x / 0.5;
        float red = 0.0;
        float green = 2.0 * min(percent, 0.5);
        float blue = 2.0 * min(1.0 - percent, 0.5);
        color = vec3(red,green,blue);
    }
    vec4 final = vec4(color *(1 - iHeapMapUV.y),1.0); 
    gl_FragColor = final;
}