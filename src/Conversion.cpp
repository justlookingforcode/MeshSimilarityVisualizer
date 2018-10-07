/* Start Header ------------------------------------------------------
Copyright (C) 2014 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.

File Name:      Conversion.cpp
Purpose:        This is the Conversion function implementations.
Language:       C++
Platform:       Windows Visual Studio
Author:         Tan Wen De Kevin
Creation Date:  24/Sep/2016
- End Header -------------------------------------------------------*/
#include "Conversion.h"
#include <sstream>

namespace Proto
{
	/*************************************************************************/
	/*!
	\fn void ConvertStringToBool(bool & t_Result, str & t_Str)

	\brief
	This function converts string to boolean.

	\param t_Result
	This is the result of the conversion.

	\param t_Str
	This is the specified string to be converted.
	*/
	/*************************************************************************/
	void ConvertStringToBool(bool & t_Result, str & t_Str)
	{
		std::istringstream(t_Str) >> std::boolalpha >> t_Result;
	}

	/*************************************************************************/
	/*!
	\fn void ConvertStringToInteger(int & t_Result, str & t_Str)

	\brief
	This function converts string to integer.

	\param t_Result
	This is the result of the conversion.

	\param t_Str
	This is the specified string to be converted.
	*/
	/*************************************************************************/
	void ConvertStringToInteger(int & t_Result, str & t_Str)
	{
		t_Result = std::stoi(t_Str);
	}

	/*************************************************************************/
	/*!
	\fn void ConvertStringToFloat(f32 & t_Result, str & t_Str)

	\brief
	This function converts string to float.

	\param t_Result
	This is the result of the conversion.

	\param t_Str
	This is the specified string to be converted.
	*/
	/*************************************************************************/
	void ConvertStringToFloat(f32 & t_Result, str & t_Str)
	{
		t_Result = std::stof(t_Str);
	}

	/*************************************************************************/
	/*!
	\fn void ConvertBoolToString(str & t_Result, bool & t_BoolValue)

	\brief
	This is the function converts from bool type to string.

	\param t_Result
	This is the result of the conversion.

	\param t_BoolValue
	This is the bool value to be converted.
	*/
	/*************************************************************************/
	void ConvertBoolToString(str & t_Result, bool & t_BoolValue)
	{
		t_Result = (t_BoolValue) ? "true" : "false";
	}

	/*************************************************************************/
	/*!
	\fn void ConvertIntegerToString(str & t_Result, int & t_IntValue)

	\brief
	This is the function converts from integer type to string.

	\param t_Result
	This is the result of the conversion.

	\param t_IntValue
	This is the integer value to be converted.
	*/
	/*************************************************************************/
	void ConvertIntegerToString(str & t_Result, int & t_IntValue)
	{
		std::stringstream t_String;
		t_String << t_IntValue;
		t_Result = t_String.str();
	}

	void ConvertFloatToString(str & t_Result, f32 & t_IntValue)
	{
		std::stringstream t_String;
		t_String << t_IntValue;
		t_Result = t_String.str();
	}

	/*************************************************************************/
	/*!
	\fn void ConvertVec4ToVec3(vec3 & t_Result, vec4 & t_Vec4)

	\brief
	This is the function converts from vec4 type to vec3.

	\param t_Result
	This is the result of the conversion.

	\param t_Vec4
	This is the vector to be converted.
	*/
	/*************************************************************************/
	vec3 ConvertVec4ToVec3(vec4 & t_Vec4)
	{
		return vec3(t_Vec4.x, t_Vec4.y, t_Vec4.z);
	}

	/*************************************************************************/
	/*!
	\fn void ConvertVec3ToVec4(vec4 & t_Result, vec3 & t_Vec3)

	\brief
	This is the function converts from vec3 type to vec4.

	\param t_Result
	This is the result of the conversion.

	\param t_Vec3
	This is the vector to be converted.
	*/
	/*************************************************************************/
	vec4 ConvertVec3ToVec4(vec3 & t_Vec3)
	{
		return vec4(t_Vec3.x, t_Vec3.y, t_Vec3.z, 1.f);
	}

	/*************************************************************************/
	/*!
	\fn str ConvertUpperToLowerCase(const str & t_String)

	\brief
	This is the function converts from upper case to lower case.

	\param t_String
	This is the string value to be converted.
	*/
	/*************************************************************************/
	str ConvertUpperToLowerCase(const str & t_String)
	{
		int i = 0;
		str t_ResultStr;

		while (t_String[i])
		{
			char t_Charc = tolower(t_String[i]);
			t_ResultStr += t_Charc;
			++i;
		}

		return t_ResultStr;
	}

	/*************************************************************************/
	/*!
	\fn str ConvertLowerToUpperCase(const str & t_String)

	\brief
	This is the function converts from lower case to upper case.

	\param t_String
	This is the string value to be converted.
	*/
	/*************************************************************************/
	str ConvertLowerToUpperCase(const str & t_String)
	{
		int i = 0;
		str t_ResultStr;

		while (t_String[i])
		{
			char t_Charc = toupper(t_String[i]);
			t_ResultStr += t_Charc;
			++i;
		}

		return t_ResultStr;
	}
}
