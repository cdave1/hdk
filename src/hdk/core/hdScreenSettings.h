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

#ifndef HD_SCREEN_SETTINGS_H
#define HD_SCREEN_SETTINGS_H

#include <hdk/hdMath.h>

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