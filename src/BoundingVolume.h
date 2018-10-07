/* Start Header ------------------------------------------------------
    Copyright (C) 2014 DigiPen Institute of Technology.
    Reproduction or disclosure of this file or its contents 
    without the prior written consent of DigiPen Institute of 
    Technology is prohibited.

    File Name:      BoundingVolume.h
    Purpose:        This is where the BoundingVolume class 
                    implementations.
    Language:       C++
    Platform:       Windows Visual Studio
    Author:         Tan Wen De Kevin
    Creation Date:  24/Sep/2016
    - End Header -------------------------------------------------------*/

#ifndef _BOUNDINGVOLUME_H_
#define _BOUNDINGVOLUME_H_

// ==========================
// includes
// ==========================

#include "Mesh.hpp"
#include "math.h"
#include "Conversion.h"


// ==========================
// class/ function prototypes
// ==========================

namespace Proto
{
    #define TOTAL_AXIS 3

    enum BVTYPE
    {
        TYPE_NONE   = -1,
        TYPE_BS     = 0,
        TYPE_AABB,
        TYPE_OBB,
		
        TOTAL_TYPE
    };

  
}

#endif
