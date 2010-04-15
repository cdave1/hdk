/*
 *  hdCamera.h
 *  fpsTest
 *
 *  Created by david on 16/11/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_CAMERA_H
#define HD_CAMERA_H

#include "hdMath.h"

typedef struct hdCamera
{
	hdCamera() {}
	
	hdVec3 eye;
	
	hdVec3 center;
	
	hdVec3 up;
	
};



#endif