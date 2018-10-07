/******************************************************************************/
/*!
\file   math.cpp
\par    Purpose: Implementations of math functions
\par    Language: C++
\par    Platform: Visual Studio 2013, Windows 7 64-bit
\date   28/10/2014
*/
/******************************************************************************/

#include "math.hpp"
#include "defines.h"

/******************************************************************************/
/*  Wrappers for GLM functions                                                */
/******************************************************************************/

/******************************************************************************/
/*!
\fn     Mat4 Scale(float sx, float sy, float sz)
\brief
        Compute scaling matrix
\param  sx
        Scale factor along x
\param  sy
        Scale factor along y
\param  sz
        Scale factor along z
\return
        The resulting scaling matrix
*/
/******************************************************************************/
Mat4 Scale(float sx, float sy, float sz)
{
    return glm::scale(Mat4(1.0f), Vec3(sx, sy, sz));
}


/******************************************************************************/
/*!
\fn     Mat4 Scale(const Vec3 &s)
\brief
        Compute scaling matrix
\param  s
        3-component vector indicating the scale factors along x, y, z
\return
        The resulting scaling matrix
*/
/******************************************************************************/
Mat4 Scale(const Vec3 &s)
{
    return glm::scale(Mat4(1.0f), s);
}


/******************************************************************************/
/*!
\fn     Mat4 Rotate(float angle, const Vec3 &axis)
\brief
        Compute rotation matrix
\param  angle
        Rotation angle
\param  axis
        Rotation axis
\return
        The resulting rotation matrix
*/
/******************************************************************************/
Mat4 Rotate(float angle, const Vec3 &axis)
{
    return glm::rotate(Mat4(1.0f), angle, axis);
}


/******************************************************************************/
/*!
\fn     Mat4 Rotate(float angle, float ax, float ay, float az)
\brief
        Compute rotation matrix
\param  angle
        Rotation angle
\param  ax
        x-component of rotation axis
\param  ay
        y-component of rotation axis
\param  az
        z-component of rotation axis
\return
        The resulting rotation matrix
*/
/******************************************************************************/
Mat4 Rotate(float angle, float ax, float ay, float az)
{
    return glm::rotate(Mat4(1.0f), angle, Vec3(ax, ay, az));
}


/******************************************************************************/
/*!
\fn     Mat4 Translate(float tx, float ty, float tz)
\brief
        Compute translation matrix
\param  tx
        Translation along x
\param  ty
        Translation along y
\param  tz
        Translation along z
\return
        The resulting translation matrix
*/
/******************************************************************************/
Mat4 Translate(float tx, float ty, float tz)
{
    return glm::translate(Mat4(1.0f), Vec3(tx, ty, tz));
}


/******************************************************************************/
/*!
\fn     Mat4 Translate(const Vec3 &t)
\brief
        Compute translation matrix
\param  s
        3-component vector indicating the translation amounts along x, y, z
\return
        The resulting translation matrix
*/
/******************************************************************************/
Mat4 Translate(const Vec3 &t)
{
    return glm::translate(Mat4(1.0f), t);
}


/******************************************************************************/
/*!
\fn     Mat4 Frustum(float leftPlane, float rightPlane, 
                            float bottomPlane, float topPlane, 
                            float nearPlane, float farPlane)
\brief
        Compute perspective projection matrix
\param  leftPlane
        x-coordinate of the intersection between left plane and near plane
\param  rightPlane
        x-coordinate of the intersection between right plane and near plane
\param  bottomPlane
        y-coordinate of the intersection between bottom plane and near plane
\param  topPlane
        y-coordinate of the intersection between top plane and near plane
\param  nearPlane
        z-coordinate of near plane
\param  farPlane
        z-coordinate of far plane
\return
        The resulting perspective projection matrix
*/
/******************************************************************************/
Mat4 Frustum(   float leftPlane, float rightPlane, 
                float bottomPlane, float topPlane, 
                float nearPlane, float farPlane)
{
    return glm::frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
}


/******************************************************************************/
/*!
\fn     Mat4 Perspective(float fovy, float aspect, float near, float far)
\brief
        Compute perspective projection matrix
\param  fovy
        Field-of-view y full angle (not half angle)
\param  aspect
        Width-to-height aspect ratio
\param  near
        Positive distance from camera to near plane
\param  far
        Positive distance from camera to far plane
\return
        The resulting perspective projection matrix
*/
/******************************************************************************/
Mat4 Perspective(float fovy, float aspect, float near, float far)
{
    return glm::perspective(fovy, aspect, near, far);
}


/******************************************************************************/
/*!
\fn     Mat4 LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up)
\brief
        Compute view transformation matrix
\param  eye
        Camera position
\param  center
        Look-at position
\param  up
        Up vector
\return
        The resulting view transformation matrix
*/
/******************************************************************************/
Mat4 LookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up)
{
    return glm::lookAt(eye, center, up);
}




/******************************************************************************/
/*!
\fn     Vec3 Cross(const Vec3 &v1, const Vec3 &v2)
\brief
        Cross product of two 3-component vectors
\param  v1
        The 1st vector
\param  v2
        The 2nd vector
\return
        The resulting cross product
*/
/******************************************************************************/
Vec3 Cross(const Vec3 &v1, const Vec3 &v2)
{
    return glm::cross(v1, v2);
}


/******************************************************************************/
/*!
\fn     float Dot(const Vec3 &v1, const Vec3 &v2)
\brief
        Dot product of two 3-component vectors
\param  v1
        The 1st vector
\param  v2
        The 2nd vector
\return
        The resulting dot product
*/
/******************************************************************************/
float Dot(const Vec3 &v1, const Vec3 &v2)
{
    return glm::dot(v1, v2);
}


/******************************************************************************/
/*!
\fn     float Distance(const Vec3 &v1, const Vec3 &v2)
\brief
        Distance between two 3D points.
\param  v1
        Position vector of the 1st point
\param  v2
        Position vector of the 2nd point
\return
        The resulting distance
*/
/******************************************************************************/
float Distance(const Vec3 &v1, const Vec3 &v2)
{
    return glm::distance(v1, v2);
}


/******************************************************************************/
/*!
\fn     Mat4 Inverse(const Mat4 &m)
\brief
        Find inverse of a matrix
\param  m
        The input matrix
\return
        The resulting inverse matrix
*/
/******************************************************************************/
Mat4 Inverse(const Mat4 &m)
{
    return glm::inverse(m);
}


/******************************************************************************/
/*!
\fn     Mat4 Transpose(const Mat4 &m)
\brief
        Find transpose of a matrix
\param  m
        The input matrix
\return
        The resulting transpose matrix
*/
/******************************************************************************/
Mat4 Transpose(const Mat4 &m)
{
    return glm::transpose(m);
}


/******************************************************************************/
/*!
\fn     void PrintMat(const Mat4 &m)
\brief
        Print out a matrix so that translation appears in the last column
\param  m
        The input matrix
*/
/******************************************************************************/
void PrintMat(const Mat4 &m)
{
    std::cout << std::fixed;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
            std::cout << std::setw(8) << std::setprecision(2) << m[j][i] << " ";
        std::cout << "\n";
    }
    std::cout << "\n";
}


/******************************************************************************/
/*!
\fn     void PrintVec(const Vec3 &v)
\brief
        Print out a 3-component vector
\param  m
        The input vector
*/
/******************************************************************************/
void PrintVec(const Vec3 &v)
{
    std::cout << std::fixed;
    for (int i = 0; i < 3; ++i)
        std::cout << std::setw(8) << std::setprecision(2) << v[i] << " ";
    std::cout << "\n";
}


/******************************************************************************/
/*!
\fn     void PrintVec(const Vec4 &v)
\brief
        Print out a 4-component vector
\param  m
        The input vector
*/
/******************************************************************************/
void PrintVec(const Vec4 &v)
{
    std::cout << std::fixed;
    for (int i = 0; i < 4; ++i)
        std::cout << std::setw(8) << std::setprecision(2) << v[i] << " ";
    std::cout << "\n";
}


/******************************************************************************/
/*!
\fn     float RoundDecimal(float input)
\brief
        Round a value to 4-decimal places
\param  input
        The given value
\return
        The rounded value
*/
/******************************************************************************/
float RoundDecimal(float input)
{
    return std::floor(input * 10000.0f + 0.5f) / 10000.0f;
}


/******************************************************************************/
/*!
\fn     Vec3 RoundDecimal(const Vec3 &input)
\brief
        Round the components of a vector3 to 4-decimal places
\param  input
        The given vector
\return
        The vector with rounded values
*/
/******************************************************************************/
Vec3 RoundDecimal(const Vec3 &input)
{
    return Vec3(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]));
}


/******************************************************************************/
/*!
\fn     Vec4 RoundDecimal(const Vec4 &input)
\brief
        Round the components of a vector4 to 4-decimal places
\param  input
        The given vector
\return
        The vector with rounded values
*/
/******************************************************************************/
Vec4 RoundDecimal(const Vec4 &input)
{
    return Vec4(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]), RoundDecimal(input[3]));
}


/******************************************************************************/
/*!
\fn     bool DegenerateTri(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2)
\brief
        Check if a triangle is degenerate, that is if the length between any
        two vertices is close to 0.
\param  v0
        The 1st vertex
\param  v1
        The 2nd vertex
\param  v2
        The 3rd vertex
\return
        Whether the triangle is degenerate
*/
/******************************************************************************/
bool DegenerateTri(const Vec3 &v0, const Vec3 &v1, const Vec3 &v2)
{
    return (Distance(v0, v1) < EPSILON ||
            Distance(v1, v2) < EPSILON ||
            Distance(v2, v0) < EPSILON);
}


/*************************************************************************/
/*!
\fn mat4 TranslationMatrix(const f32 & t_TranslateX,
const f32 & t_TranslateY,
const f32 & t_TranslateZ)

\brief
This is the function builds the translation matrix using 3 float values.

\param t_TranslateX
This is the x-component of the translation vector.

\param t_TranslateY
This is the y-component of the translation vector.

\param t_TranslateZ
This is the z-component of the translation vector.

\return
This function a translation matrix as result.
*/
/*************************************************************************/
mat4 TranslationMatrix(const f32 & t_TranslateX,
    const f32 & t_TranslateY,
    const f32 & t_TranslateZ)
{
    return glm::translate(IDENT_MAT, vec3(t_TranslateX, t_TranslateY, t_TranslateZ));
}

/*************************************************************************/
/*!
\fn mat4 TranslationMatrix(const vec3 & t_TranslationVec)

\brief
This is the function builds the translation matrix using the
translation vector.

\param t_TranslationVec
This is the translation vector.

\return
This function a translation matrix as result.
*/
/*************************************************************************/
mat4 TranslationMatrix(const vec3 & t_TranslationVec)
{
    return glm::translate(IDENT_MAT, t_TranslationVec);
}

/*************************************************************************/
/*!
\fn mat4 ScaleMatrix(const f32 & t_ScaleX,
const f32 & t_ScaleY,
const f32 & t_ScaleZ)

\brief
This is the function builds the scale matrix using 3 float values.

\param t_ScaleX
This is the x-component of the scale vector.

\param t_ScaleY
This is the y-component of the scale vector.

\param t_ScaleZ
This is the z-component of the scale vector.

\return
This function a scale matrix as result.
*/
/*************************************************************************/
mat4 ScaleMatrix(const f32 & t_ScaleX,
    const f32 & t_ScaleY,
    const f32 & t_ScaleZ)
{
    return glm::scale(IDENT_MAT, vec3(t_ScaleX, t_ScaleY, t_ScaleZ));
}

/*************************************************************************/
/*!
\fn mat4 ScaleMatrix(const vec3 & t_ScaleVec)

\brief
This is the function builds the scale matrix using the scale vector.

\param t_ScaleVec
This is the scale vector.

\return
This function a scale matrix as result.
*/
/*************************************************************************/
mat4 ScaleMatrix(const vec3 & t_ScaleVec)
{
    return glm::scale(IDENT_MAT, t_ScaleVec);
}

/*************************************************************************/
/*!
\fn mat4 RotationMatrix(const f32 & t_RotX,
const f32 & t_RotY,
const f32 & t_RotZ)

\brief
This is the function builds the rotation matrix using 3 float values.

\param t_RotX
This is the x-component of the rotation vector.

\param t_RotY
This is the y-component of the rotation vector.

\param t_RotZ
This is the z-component of the rotation vector.

\return
This function a rotation matrix as result.
*/
/*************************************************************************/
mat4 RotationMatrix(const f32 & t_RotX,
    const f32 & t_RotY,
    const f32 & t_RotZ)
{
    mat4 t_Rot1 = glm::rotate(IDENT_MAT, t_RotX, vec3(1.0f, 0.f, 0.f));
    mat4 t_Rot2 = glm::rotate(t_Rot1, t_RotY, vec3(0.0f, 1.f, 0.f));
    return glm::rotate(t_Rot2, t_RotZ, vec3(0.f, 0.f, 1.f));
}

/*************************************************************************/
/*!
\fn mat4 RotationMatrix(mat4  & t_Matrix,
f32 & t_RotAngle,
vec3  & t_Axis)

\brief
This is the function builds the rotation matrix using the
rotation angle, and the axis vector to be rotated about.

\param t_Matrix
This is the matrix to be multiplied to.

\param t_RotAngle
This is the rotation angle.

\param t_Axis
This is the axis vector to be rotated about.

\return
This function a rotation matrix as result.
*/
/*************************************************************************/
mat4 RotationMatrix(mat4  & t_Matrix,
    f32 & t_RotAngle,
    vec3  & t_Axis)
{
    return glm::rotate(t_Matrix, t_RotAngle, t_Axis);
}

/*************************************************************************/
/*!
\fn mat4 TransformationMatrix(const mat4 & t_TransMat,
const mat4 & t_RotMat,
const mat4 & t_ScaleMat)

\brief
This is the function builds the transformation matrix based on
the translation, rotation, and scaling matrices provided.

\param t_TransMat
This is the translation matrix.

\param t_RotMat
This is the rotation matrix.

\param t_ScaleMat
This is the scaling matrix.

\return
This function a transformation matrix as result.
*/
/*************************************************************************/
mat4 TransformationMatrix(const mat4 & t_TransMat,
    const mat4 & t_RotMat,
    const mat4 & t_ScaleMat)
{
    // M = T * R * S
    return t_TransMat * t_RotMat * t_ScaleMat;
}

/*************************************************************************/
/*!
\fn mat4 TransformationMatrix(const mat4 & t_LHSMat,
const mat4 & t_RHSMat)

\brief
This is the function builds the transformation matrix based on
any of the two (translation, rotation, or scaling) matrices provided.

e.g.
M = T * S
M = T * R
M = R * S

\param t_LHSMat
This is the matrix on the left.

\param t_RHSMat
This is the matrix on the right.

\return
This function a transformation matrix as result.
*/
/*************************************************************************/
mat4 TransformationMatrix(const mat4 & t_LHSMat,
    const mat4 & t_RHSMat)
{
    return t_LHSMat * t_RHSMat;
}

/*************************************************************************/
/*!
\fn mat4 ComputeViewMatrix(vec3 & t_EyePos,
vec3 & t_Target,
vec3 & t_UpVec)

\brief
This is the function builds the view matrix based on
the camera's eye, target, and up vector provided.

\param t_EyePos
This is the camera's eye.

\param t_Target
This is the camera's target.

\param t_UpVec
This is the camera's up vector.

\return
This function a view matrix as result.
*/
/*************************************************************************/
mat4 ComputeViewMatrix(vec3 t_EyePos,
    vec3 t_Target,
    vec3 t_UpVec)
{
    // eye      = camera pos
    // center   = target
    return glm::lookAt(t_EyePos, t_Target, t_UpVec);
}

void modified_gram_schmidt(Mat3 &out, const Mat3 &in)
{
	//normalised column 1 is used


	out[0] = Normalise(in[0]);
	
	
	//subtract projection that contains column 1
	out[1] = Normalise(in[1] - Dot(in[1], out[0])*out[0]);
	//subtract projection from column 1
	out[2] = in[2] - Dot(in[2], out[0])*out[0];
	//subtract projection from column 2
	out[2] -= Dot(out[2], out[1])*out[1];
	out[2] = Normalise(out[2]);

}