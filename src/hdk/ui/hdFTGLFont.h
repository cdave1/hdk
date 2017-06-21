/*
 *  hdFontPolygon.h
 *  TotemGame
 *
 *  Created by david on 7/05/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

/*
 *
 * Font class that uses the FTGL engine for rendering
 *
 * Not suitable for iPhone or anything in a low memory 
 * environment.
 *
 */

#ifndef _HDK_FONT_QUAD_
#define _HDK_FONT_QUAD_

//#include "ft2build.h"
#include "ftgles.h"

#ifdef LEVEL_EDITOR
#include <hdk/OpenGL.h"
#else
#include <hdk/OpenGLES.h"
#endif

#include <hdk/Common.h"
#include <hdk/physicsEngine.h"
#include "constants.h"
#include <hdk/Polygon.h"
#include <hdk/TextureManager.h"

#include <hdk/DrawingUtilities.h"

#define kMaxFontTextSize 512
#define kMaxFontPathSize 512

class hdFTGLFont : public hdDrawable, public hdPolygon
{
public:
	hdFTGLFont() {} 
	
	hdFTGLFont(const char* fontFileName, 
				  const int fontsize,
				  hdGameWorld* gameWorld);
	
	hdFTGLFont(const char* fontFileName, 
				  const int fontsize,
				  const char* text,
				  hdGameWorld* gameWorld, 
				  const hdAABB& aabb);
	
	hdFTGLFont(const char* fontFileName,
				  const int fontsize,
				  const char* text,
				  hdGameWorld* gameWorld, 
				  const float x, const float y, 
				  const float w, const float h);
	
	~hdFTGLFont();
	
	bool MouseDown(float x, float y);
	
	bool MouseOver(float x, float y);
	
	bool MouseUp(float x, float y);
	
	void SetRenderToTexture(const bool renderToTexture);
	
	const char* GetText() const;
	
	void SetText(const char* text);
	
	void Draw() const;
	
	void SetTint(float r, float g, float b, float a);
	
	void AlignCenter();
	
	void AlignRight();
	
	void AlignLeft();
	
	
private:
	void PrivateInit(const char* fontFileName,
					 const int fontsize,
					 const char* text);
	
	void DrawFont();
	
	void SetupFontTexture(bool reset);
	
	FTSimpleLayout simpleLayout;
	
	int renderMode;
	
	hdTexture* m_fontTexture;
	
	FTFont* m_font;
	
	int m_fontsize;
	
	char m_text[kMaxFontTextSize];
	
	bool m_renderToTexture;
	
	float m_colorTint[4];
	
};





inline const char* hdFTGLFont::GetText() const
{
	return m_text;
}


inline void hdFTGLFont::SetTint(float r, float g, float b, float a)
{
	m_colorTint[0] = r;
	m_colorTint[1] = g;
	m_colorTint[2] = b;
	m_colorTint[3] = a;
}


#endif
