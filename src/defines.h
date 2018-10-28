/*  Start Header -------------------------------------------------------
File Name:      Defines.h
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/

#ifndef _DEFINES_H_
#define _DEFINES_H_
#define CLOTH_SIM 1
// ==========================
// includes
// ==========================

#include <string>
#include <vector>
#include "glm/gtc/type_ptr.hpp"

#define UNUSED(x) __pragma(warning(suppress:4100)) x
#define BUFFER_OFFSET(x) ((char *)NULL + x)
// ==========================
// defnes
// ==========================

typedef char                s8;
typedef short               s16;
typedef int                 s32;
typedef long long           s64;

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef float               f32;
typedef double              f64;

typedef unsigned long       ulong;
typedef std::string         str;

typedef glm::vec2           vec2;
typedef glm::vec3			vec3;
typedef glm::vec4           vec4;

typedef glm::mat3           mat3;
typedef glm::mat4           mat4;

typedef std::vector<str>    STRCONT;
typedef std::vector<vec2>   VEC2CONT;
typedef std::vector<vec3>   VEC3CONT;
typedef std::vector<vec4>   VEC4CONT;



#define     M_E             2.71828182845904523536
#define     M_LOG2E         1.44269504088896340736
#define     M_LOG10E        0.434294481903251827651
#define     M_LN2           0.693147180559945309417
#define     M_LN10          2.30258509299404568402
#define     M_1_PI          0.318309886183790671538
#define     M_2_PI          0.636619772367581343076
#define     M_2_SQRTPI      1.12837916709551257390
#define     M_SQRT2         1.41421356237309504880
#define     M_SQRT1_2       0.707106781186547524401
#define		ConvertToDegrees(X)	static_cast<float>(X * (180 / PI))
#define		ConvertToRadians(X) static_cast<float>(X * (PI / 180))

#define     GL_MINORVERS    3
#define     GL_MAJORVERS    3

const vec3                  X_AXIS(1.f, 0.f, 0.f);
const vec3                  Y_AXIS(0.f, 1.f, 0.f);
const vec3                  Z_AXIS(0.f, 0.f, 1.f);
const mat4                  IDENT_MAT(1.f);

const int                   xIndex = 0;
const int                   yIndex = 1;
const f32                   PI = 3.14159265359f;
const f32                   PI_HALF = PI / 2;
const f32                   PI_QUARTER = PI / 4;
const f32                   PI_THREE_HALF = PI * 3 / 2;
const f32                   PI_TWICE = PI * 2;
const float  TWO_PI = 2.0f * PI;
const float  HALF_PI = 0.5f * PI;
const float  QUARTER_PI = 0.25f * PI;
const float  EIGHTH_PI = 0.125f * PI;
const float  FIFTHTEENTH_PI = 0.0667f * PI;
const float  SIXTEENTH_PI = 0.0625f * PI;

const float  DEG_TO_RAD = PI / 180.0f;
const float  RAD_TO_DEG = 180.0f / PI;

const float  EPSILON = 0.00001f;

#define TEST_0 0
#define TEST_1 1
#define TEST_2 2
#define TEST_NUM TEST_2
#endif
