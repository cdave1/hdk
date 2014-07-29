/*
 * Copyright (c) 2014 Hackdirt Ltd.
 * Author: David Petrie (david@davidpetrie.com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software. Permission is granted to anyone to use this software for
 * any purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 * that you wrote the original software. If you use this software in a product, an
 * acknowledgment in the product documentation would be appreciated but is not
 * required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#include "hdScreenSettings.h"

typedef struct screenRect
{
	float width;
	float height;
} hdScreenRect;

typedef enum orientation
{
	hdScreenOrientationLandscape = 0,
	hdScreenOrientationPortrait = 1,
} hdScreenOrientation_t;

typedef struct screenSettings
{
	hdScreenOrientation_t orientation;
	float aspectRatio;
	float pixelsPerGameUnit;
	float scaleFactor;
	hdScreenRect screenRect;
	hdScreenRect defaultCameraRect;
	hdScreenRect minCameraRect;
	hdScreenRect maxCameraRect;
	
	float lowerZoomRatioX;
	float lowerZoomRatioY;
	float upperZoomRatioX;
	float upperZoomRatioY;
} hdScreenSettings_t;


hdScreenSettings_t screenSettings;

void ScreenSettings_SetLandscape()
{
	screenSettings.orientation = hdScreenOrientationLandscape;
}


void ScreenSettings_SetPortrait()
{
	screenSettings.orientation = hdScreenOrientationPortrait;
}


void ScreenSettings_SetScaleFactor(float scaleFactor)
{
	screenSettings.scaleFactor = hdMax(1.0f, scaleFactor);
}


void ScreenSettings_SetScreenRect(float aspectRatio, float pixelWidth, float pixelHeight)
{
	screenSettings.aspectRatio = aspectRatio;
	screenSettings.screenRect.width = pixelWidth;
	screenSettings.screenRect.height = pixelHeight;
}


void ScreenSettings_SetDefaultCameraRect(float width, float height)
{
	screenSettings.defaultCameraRect.width = width;
	screenSettings.defaultCameraRect.height = height;
}


void ScreenSettings_SetMinCameraRect(float width, float height)
{
	screenSettings.minCameraRect.width = width;
	screenSettings.minCameraRect.height = height;
}


void ScreenSettings_SetMaxCameraRect(float width, float height)
{
	screenSettings.maxCameraRect.width = width;
	screenSettings.maxCameraRect.height = height;
}


extern void ScreenSettings_SetPixelsPerGameUnit(float val)
{
	screenSettings.pixelsPerGameUnit = val;
}


void ScreenSettings_GetAspectRatio(float *outRatio)
{
	if (screenSettings.orientation == hdScreenOrientationLandscape)
		*outRatio = screenSettings.aspectRatio;
	else
		*outRatio = 1.0f / screenSettings.aspectRatio;
}


void ScreenSettings_GetScreenRect(float *outWidth, float *outHeight)
{
	if (screenSettings.orientation == hdScreenOrientationLandscape)
	{
		*outWidth = screenSettings.screenRect.width;
		*outHeight = screenSettings.screenRect.height;
	}
	else
	{
		*outWidth = screenSettings.screenRect.height;
		*outHeight = screenSettings.screenRect.width;
	}
}


void ScreenSettings_GetScaleFactor(float *outScaleFactor)
{
	*outScaleFactor = hdClamp(screenSettings.scaleFactor, 1.0f, 10.0f);
}


void ScreenSettings_GetDefaultCameraRect(float *outWidth, float *outHeight)
{
	if (screenSettings.orientation == hdScreenOrientationLandscape)
	{
		*outWidth = screenSettings.defaultCameraRect.width;
		*outHeight = screenSettings.defaultCameraRect.height;
	}
	else
	{
		*outWidth = screenSettings.defaultCameraRect.height;
		*outHeight = screenSettings.defaultCameraRect.width;
	}
}


void ScreenSettings_GetMinCameraRect(float *outWidth, float *outHeight)
{
	if (screenSettings.orientation == hdScreenOrientationLandscape)
	{
		*outWidth = screenSettings.minCameraRect.width;
		*outHeight = screenSettings.minCameraRect.height;
	}
	else
	{
		*outWidth = screenSettings.minCameraRect.height;
		*outHeight = screenSettings.minCameraRect.width;
	}
}


void ScreenSettings_GetMaxCameraRect(float *outWidth, float *outHeight)
{
	if (screenSettings.orientation == hdScreenOrientationLandscape)
	{
		*outWidth = screenSettings.maxCameraRect.width;
		*outHeight = screenSettings.maxCameraRect.height;
	}
	else
	{
		*outWidth = screenSettings.maxCameraRect.height;
		*outHeight = screenSettings.maxCameraRect.width;
	}
}


void ScreenSettings_GetPixelsPerGameUnit(float *val)
{
	*val = screenSettings.pixelsPerGameUnit;
}										


void ScreenSettings_GetZoomRatio(float *lowerX, float *lowerY, float *upperX, float *upperY)
{
	*lowerX = 1.0f;
	*lowerY = 1.0f * screenSettings.aspectRatio;
	*upperX = 2.0f;
	*upperY = 2.0f * screenSettings.aspectRatio;
}
