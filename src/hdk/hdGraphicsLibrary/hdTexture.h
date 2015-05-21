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

#ifndef _HD_TEXTURE_H_
#define _HD_TEXTURE_H_

#define kMaxTextures        1024
#define kMaxTexturePathSize 256

typedef enum
{
    TT_Sprite,
    TT_Wall,
    TT_16Wall,
    TT_FastWall,
    TT_Pic,
    TT_Font,
    TT_Background,
    TT_16Background,
    TT_16Pic,
    TT_ScaleableFont,
    TextureTypeCount
} texturetype_t;


typedef enum
{
    Repeat = 0,
    Clamp,
    WrapModeCount	// Number of Wrap modes
} TWrapMode;


typedef enum
{
    Auto = 0,
    RGBA8888,
    RGBA4444,
    RGBA5551,
    RGB888,
    RGB565,
    DXTC1,
    DXTC1Alpha,
    DXTC3,
    DXTC5,
    Luminance,
    Alpha,
    AlphaLuminance,
    DsDt,

    UpImageFormatCount	// Number of Upload formats
} TTexFormat;


typedef enum
{
    Nearest = 0,
    Linear,
    MagFilterCount	// Number of Magnification filters
} TMagFilter;


typedef enum
{
    NearestMipMapOff = 0,
    NearestMipMapNearest,
    NearestMipMapLinear,
    LinearMipMapOff,
    LinearMipMapNearest,
    LinearMipMapLinear,

    MinFilterCount	// Number of Min filters

} TMinFilter;


class hdTexture
{
public:
    hdTexture() :
    MipMap(false),
    isTextureCube(false),
    isPremultipliedAlpha(false),
    UploadFormat(Auto),
    WrapS(Repeat),
    WrapT(Repeat),
    WrapR(Repeat),
    MinFilter(NearestMipMapOff),
    MagFilter(Nearest),
    registration_sequence(0),
    width(0),
    height(0),
    upload_width(0),
    upload_height(0),
    texnum(0),
    bytes(0),
    bpp(32),
    type(TT_Sprite)
    {}

    bool		MipMap;
    bool		isTextureCube;
    bool		isPremultipliedAlpha;
    TTexFormat		UploadFormat;
    TWrapMode		WrapS;
    TWrapMode		WrapT;
    TWrapMode		WrapR;
    TMinFilter		MinFilter;
    TMagFilter		MagFilter;
    unsigned int		registration_sequence;		// 0 = free
    unsigned short width, height;
    unsigned short upload_width, upload_height;
    unsigned int texnum;
    unsigned short bytes;
    unsigned short bpp;
    texturetype_t type;
    char	name[ kMaxTexturePathSize ];			// game path, including extension
};


#endif