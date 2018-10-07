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
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 nrm;
layout (location = 2) in vec3 tan;
layout (location = 3) in vec3 bitan;
layout (location = 4) in vec2 uv;

uniform mat4 mvMat;     /*  model-view matrix for positions */
uniform mat4 nmvMat;    /*  model-view matrix for normals */
uniform mat4 projMat;   /*  projection matrix */

uniform bool lightOn;           /*  whether lighting should be applied */
uniform int  numLights;
uniform vec3 lightPosVF[10];    /*  light pos already in view frame */

uniform bool normalMappingOn;   /*  whether normal mapping should be applied */
uniform bool textureOn;

out vec2 uvCoord;

/*  Output vectors:
    - If normalMapping is on then these vectors are in tangent space.
    - Otherwise they are in view space
*/
out vec3 lightDir[10];
out vec3 viewDir;
out vec3 normal;


void main(void) 
{
    vec4 posVF = mvMat * vec4(pos, 1.0);


    /*  For object texturing */
    uvCoord = uv;



    
        //@TODO
        /*  Compute view direction, light direction and normal in view space */
        /*  ... */
		/*view direction*/
		viewDir = -posVF.xyz; 
		/*normal*/
		normal = normalize(mat3(nmvMat) * nrm);  
		/*lights*/       
        for (int i = 0; i < numLights; ++i)
          lightDir[i] = normalize(lightPosVF[i] - posVF.xyz); 

        if (normalMappingOn)    
        {
            //@TODO
            /*  If normal mapping is used, bring view dir and light dir to tangent space. 
                To do this, the TBN matrix needs to be used.
                Normal doesn't need to be transformed since we'll obtain it directly
                from the normal texture in the fragment shader.
            */
            /*  ... */
			vec3 tangentVF		= vec3(normalize(mat3(mvMat) * tan));
            vec3 bitangentVF	= vec3(normalize(mat3(mvMat) * bitan));
            mat3 transpose		= transpose(mat3(tangentVF, bitangentVF, normal));
            viewDir = transpose * (-posVF.xyz);               
            for (int i = 0; i < numLights; ++i)
              lightDir[i] = transpose * (lightDir[i]);   
        }
    
	gl_Position = projMat * posVF;
}