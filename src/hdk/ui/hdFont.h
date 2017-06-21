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

#ifndef _HD_FONT_H_
#define _HD_FONT_H_

#include <hdk/math.h>
#include <hdk/graphics.h>

#define kMaxFontTextSize 512
#define kFluidFontFileCharCount 128

#define FIRST_PRINTABLE_ASCII_INDEX 32
#define LAST_PRINTABLE_ASCII_INDEX 126

typedef enum
{
    e_fontAlignmentLeft = 0,
    e_fontAlignmentRight,
    e_fontAlignmentCenter,
} e_fontAlignment;

/*
 4 bytes	byteWidth    -- Width of the font (in bytes)
 4 bytes	byteHeight   -- Height of the font (in bytes)
 4 bytes	xOffset      -- X character placement offset (in pixels)
 4 bytes	yOffset      -- Y character placement offset (in pixels)
 4 bytes	screenWidth  -- Width of the font (in screen coordinates)
 4 bytes	screenHeight -- Height of the font (in screen coordinates)
 ?? bytes	fontData     -- Array of byteWidth by byteHeight pixels
 */
typedef struct _FluidFontCharHeader
{
    int byteWidth;
    int byteHeight;
    signed int xOffset;
    signed int yOffset;
    int screenWidth;
    int screenHeight;
    int pixelArea;


    hdVec3 texCoords[4];
    int charId;
    unsigned char* buffer;

} FluidFontCharHeader;

class hdFont
{
public:
    ~hdFont() {}

    const hdTexture* GetTexture();

private:
    friend class hdFontManager;
    friend class hdFontPolygon;

    hdFont();

    FluidFontCharHeader fileChars[kFluidFontFileCharCount];

    hdTexture* m_texture;
    
    char	name[ kMaxTexturePathSize ];
    
    int m_lineHeight;
};


inline const hdTexture* hdFont::GetTexture()
{
    return m_texture;
}

#endif
