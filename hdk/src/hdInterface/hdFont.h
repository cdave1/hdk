/*
 *  hdFluidFont.h
 *  hdGameEngine
 *
 *  Created by david on 14/05/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_FONT_H_
#define _HD_FONT_H_

#include "hdMath.h"
#include "hdGraphicsLibrary.h"

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
	
//	~hdFont();
	
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