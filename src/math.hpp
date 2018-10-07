/******************************************************************************/
/*!
\file   math.hpp
\par    Purpose: Placeholder for math constants/functions
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\date   28/10/2014
*/
/******************************************************************************/

#ifndef MATH_HPP
#define MATH_HPP

#include <iostream>
#include <iomanip>
#include <cmath>

#define GLM_FORCE_RADIANS
#include "defines.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/norm.hpp"

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;


/*  Useful constants */



const int X = 0;
const int Y = 1;
const int Z = 2;

const Vec3 BASIS[3]{    Vec3(1.0f, 0.0f, 0.0f),
                        Vec3(0.0f, 1.0f, 0.0f),
                        Vec3(0.0f, 0.0f, 1.0f)
                   };


/******************************************************************************/
/*  Utility functions                                                         */
/******************************************************************************/
float RoundDecimal(float input);
Vec3 RoundDecimal(const Vec3 &input);
Vec4 RoundDecimal(const Vec4 &input);
bool DegenerateTri(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2);


/******************************************************************************/
/*  Wrappers for GLM functions                                                */
/******************************************************************************/

Mat4 Scale(float sx, float sy, float sz);
Mat4 Scale(const Vec3 &s);

Mat4 Rotate(float angle, const Vec3 &axis);
Mat4 Rotate(float angle, float ax, float ay, float az);

Mat4 Translate(float tx, float ty, float tz);
Mat4 Translate(const Vec3 &t);

Mat4 Frustum(float leftPlane, float rightPlane, 
             float bottomPlane, float topPlane, 
             float nearPlane, float farPlane);
Mat4 Perspective(float fovy, float aspect, float near, float far);


Mat4 LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up);

Mat4 Inverse(const Mat4 &m);
Mat4 Transpose(const Mat4 &m);


Vec3 Cross(const Vec3 &v1, const Vec3 &v2);
float Dot(const Vec3 &v1, const Vec3 &v2);
float Distance(const Vec3 &v1, const Vec3 &v2);
template <typename T>
inline T Normalise(T t_Vec)
{
    return glm::normalize(t_Vec);
}

inline vec3 SNormalise(const vec3& t_Vec)
{
	return (t_Vec.x == 0.f && t_Vec.y == 0.f &&t_Vec.z == 0.f)? vec3(0,0,0) : glm::normalize(t_Vec);
}
template <typename T>
inline T Normalize(T t_Vec)
{
    return glm::normalize(t_Vec);
}
template <typename T>
float *ValuePtr(T value) {  return glm::value_ptr(value);   }

void PrintMat(const Mat4 &m);   /*  for debugging matrices */
void PrintVec(const Vec3 &v);   /*  for debugging vectors */
void PrintVec(const Vec4 &v);   /*  for debugging vectors */


// ==============================================
// MATRIX OPERATIONS
// ==============================================

mat4 TranslationMatrix(const f32 & t_TranslateX,
    const f32 & t_TranslateY,
    const f32 & t_TranslateZ);

mat4 TranslationMatrix(const vec3 & t_TranslationVec);

mat4 ScaleMatrix(const f32 & t_ScaleX,
    const f32 & t_ScaleY,
    const f32 & t_ScaleZ);

mat4 ScaleMatrix(const vec3 & t_ScaleVec);

mat4 RotationMatrix(const f32 & t_RotX,
    const f32 & t_RotY,
    const f32 & t_RotZ);

mat4 RotationMatrix(mat4  & t_Matrix,
    f32 & t_RotAngle,
    vec3  & t_Axis);

mat4 TransformationMatrix(const mat4 & t_LHSMat,
    const mat4 & t_RHSMat);

mat4 TransformationMatrix(const mat4 & t_TransMat,
    const mat4 & t_RotMat,
    const mat4 & t_ScaleMat);

mat4 ComputeViewMatrix(vec3 t_EyePos,
    vec3 t_Target,
    vec3 t_UpVec);

void modified_gram_schmidt(Mat3 &out, const Mat3 &in);


template<typename T>
bool SameSign(T t1, T t2)
{
	return  (t1*t2 <= 0);
}

#endif