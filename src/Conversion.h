/* Start Header ------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.

File Name:      Conversion.h
Purpose:        This is the Conversion function implementation.
Language:       C++
Platform:       Windows Visual Studio
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/

#ifndef _CONVERSION_H_
#define _CONVERSION_H_

// ==========================
// includes
// ==========================

#include "Defines.h"

// ==========================
// class/ function prototypes
// ==========================

namespace Proto
{
	// string to something-else conversion
	void ConvertStringToBool(bool & t_Result, str & t_Str);
	void ConvertStringToInteger(int & t_Result, str & t_Str);
	void ConvertStringToFloat(f32 & t_Result, str & t_Str);

	void ConvertBoolToString(str & t_Result, bool & t_BoolValue);
	void ConvertIntegerToString(str & t_Result, int & t_IntValue);
	void ConvertFloatToString(str & t_Result, f32 & t_IntValue);

	// vector types conversion
	vec3 ConvertVec4ToVec3(vec4 & t_Vec4);
	vec4 ConvertVec3ToVec4(vec3 & t_Vec3);

	str ConvertUpperToLowerCase(const str & t_String);
	str ConvertLowerToUpperCase(const str & t_String);
};

#endif
