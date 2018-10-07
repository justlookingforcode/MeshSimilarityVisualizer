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

in vec2 uvCoord;

uniform bool lightOn;
uniform int numLights;
uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec4 specular;
uniform int specularPower;
uniform vec3 uOutColor;
/*  These could be in view space or tangent space */
in vec3 lightDir[10];
in vec3 viewDir;
in vec3 normal;

uniform bool normalMappingOn;   /*  whether normal mapping is on */
uniform bool parallaxMappingOn; /*  whether parallax mapping is on */
uniform bool textureOn;

out vec4 outColor;


void main(void)
{




    //@TODO
    /*  If normal mapping is off, then use the interpolated normal from vertex shader.
        Otherwise, sample the normal from normal map.

        If parallax mapping is off, then uvCoord should be used for all the texture sampling.
        Otherwise, we should use the adjusted uvCoord.
        Adjusted uvCoord is set to be uvCoord + h * viewDir.xy,
        where h is height from bump map * 0.15 - 0.005.

        For lighting, Blinn-Phong model should be used.
    */
    /*  ... */
    vec3 vd = normalize(viewDir);
    vec3 norm;
    vec4 intensity;


    if (textureOn && !parallaxMappingOn)
    {
        outColor = vec4(texture(colorTex, uvCoord).rgb, 1);
    }

    if (textureOn && !normalMappingOn)
    {
        norm = normalize(normal);
    }
    else if (textureOn && !parallaxMappingOn)
    {
        norm = normalize(texture(normalTex, uvCoord).xyz * 2 - 1);
    }
    else if (textureOn && parallaxMappingOn)
    {
        float h = texture(bumpTex, uvCoord).r * 0.15 - 0.005;
        vec2 uvCoord2 = uvCoord + h * vd.xy;
        outColor = vec4(texture(colorTex, uvCoord2).rgb, 1);
        norm = normalize(texture(normalTex, uvCoord2).xyz * 2 - 1);
    }
    if (textureOn)
    {
        intensity = ambient;
        for (int i = 0; i < numLights; ++i)
        {
            vec3 lightVec = normalize(lightDir[i]);
            vec3 halfWayVec = normalize(vd + lightVec);
            intensity += diffuse * max(dot(norm, lightVec), 0) + specular * pow(max(dot(norm, halfWayVec), 0), specularPower);
        }
        outColor = outColor * intensity;
    }
    else
    {
        outColor = vec4(uOutColor,1); /*  Use texture for color */
    }
    //fragColor = vec4(norm, 1);
}