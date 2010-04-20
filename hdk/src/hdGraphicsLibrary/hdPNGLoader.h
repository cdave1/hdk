/*
 *  hdPNGLoader.h
 *  TotemGame
 *
 *  Created by david on 28/09/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_PNG_LOADER_H
#define HD_PNG_LOADER_H

#ifdef __cplusplus
extern "C" {
#endif
	
	extern void LoadPNG( const char *filename, unsigned char **pic, unsigned short *width, unsigned short *height, unsigned short *bytes );
	
	
	
#ifdef __cplusplus
}
#endif

#endif