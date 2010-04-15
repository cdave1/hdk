/*
 *  hdTGALoader.h
 *  AnimationEngine
 *
 *  Created by david on 9/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 * Acknowledgement: 
 *
 * 	Most of this code derived from Michael Liebscher's port of Wolfenstein 3D, which
 * was in turn derived from GIMP. <johnnycanuck@users.sourceforge.net>
 */

#ifndef _HD_TGA_LOADER_H_
#define _HD_TGA_LOADER_H_

#include "hdCore/hdFileSystem.h"

//#include "hdGameEngine.h"


#ifdef __cplusplus
extern "C" {
#endif

extern void LoadTGA( const char *filename, unsigned char **pic, unsigned short *width, unsigned short *height, unsigned short *bytes );
	
#ifdef __cplusplus
}
#endif
	
	
#endif