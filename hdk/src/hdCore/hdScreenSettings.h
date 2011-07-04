/*
 *  hdScreenSettings.h
 *  hdk
 *
 *  Created by David Petrie on 10/04/10.
 *  Copyright 2010 Hackdirt Ltd. All rights reserved.
 *
 */

#ifndef HD_SCREEN_SETTINGS_H
#define HD_SCREEN_SETTINGS_H

#include "hdMath.h"

#define HD_ASPECT_RATIO_3_2 1.5f
#define HD_ASPECT_RATIO_16_9 (16.0f/9.0f)
#define HD_ASPECT_RATIO_4_3 (4.0f/3.0f)


#ifdef __cplusplus
extern "C" {
#endif
	extern void ScreenSettings_SetLandscape();
	
	extern void ScreenSettings_SetPortrait();
	
	extern void ScreenSettings_SetScaleFactor(float scaleFactor);
	
	extern void ScreenSettings_SetScreenRect(float aspectRatio, float pixelWidth, float pixelHeight);
	
	extern void ScreenSettings_SetDefaultCameraRect(float width, float height);
	
	extern void ScreenSettings_SetMinCameraRect(float width, float height);
	
	extern void ScreenSettings_SetMaxCameraRect(float width, float height);
	
	extern void ScreenSettings_SetPixelsPerGameUnit(float val);
	
	extern void ScreenSettings_SetZoomRatio(float lowerX, float lowerY, float upperX, float upperY);
	
	
	extern void ScreenSettings_GetAspectRatio(float *outRatio);
	
	extern void ScreenSettings_GetScreenRect(float *outWidth, float *outHeight);
	
	extern void ScreenSettings_GetScaleFactor(float *outScaleFactor);
	
	extern void ScreenSettings_GetDefaultCameraRect(float *outWidth, float *outHeight);

	extern void ScreenSettings_GetMinCameraRect(float *outWidth, float *outHeight);
	
	extern void ScreenSettings_GetMaxCameraRect(float *outWidth, float *outHeight);
	
	extern void ScreenSettings_GetPixelsPerGameUnit(float *val);
	
	extern void ScreenSettings_GetZoomRatio(float *lowerX, float *lowerY, float *upperX, float *upperY);
	
#ifdef __cplusplus
}
#endif

#endif