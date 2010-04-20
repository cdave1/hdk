/*
 *  hdFTGLFont.cpp
 *  TotemGame
 *
 *  Created by david on 7/05/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdFTGLFont.h"


hdFTGLFont::hdFTGLFont(const char* fontFileName, 
							 const int fontsize,
							hdGameWorld* gameWorld) : hdPolygon(gameWorld)
{
	this->PrivateInit(fontFileName, fontsize, "");
	m_renderToTexture = false;
}


hdFTGLFont::hdFTGLFont(const char* fontFileName, 
							 const int fontsize,
			  const char* text,
			  hdGameWorld* gameWorld, 
			  const hdAABB& aabb) : hdPolygon(gameWorld)
{
	this->PrivateInit(fontFileName, fontsize, text);
	this->SetAs2DBox(aabb.lower, aabb.upper);
	this->ResetAABB();
}


hdFTGLFont::hdFTGLFont(const char* fontFileName,
							  const int fontsize,
							  const char* text,
							  hdGameWorld* gameWorld, 
							  const float x, const float y, 
							  const float w, const float h) : hdPolygon(gameWorld)
{
	this->PrivateInit(fontFileName, fontsize, text);
	this->SetAs2DBox(hdVec3(x,y,0.0), hdVec3(x+w,y+h,0.0));
	this->ResetAABB();
}


hdFTGLFont::~hdFTGLFont()
{
	delete m_fontTexture;
	delete m_font;
}


void hdFTGLFont::PrivateInit(const char* fontFileName,
								const int fontsize,
								const char* text)
{
	if (strlen(fontFileName) == 0) 
	{
		hdError(-1, "Font: zero-length filename, name: %s. Aborting...", fontFileName);
		assert(false);
	}
	
	m_renderToTexture = true;
	renderMode = FTGL::RENDER_FRONT;// | FTGL::RENDER_BACK;
	m_fontsize = fontsize;
	m_fontTexture = NULL;
	
	m_colorTint[0] = 1.0f;
	m_colorTint[1] = 1.0f;
	m_colorTint[2] = 1.0f;
	m_colorTint[3] = 1.0f;
	
	char fp[kMaxFontPathSize];
	snprintf( fp, kMaxFontPathSize, "%s/%s", hdGamedir(), fontFileName );
	
	m_font = new FTTextureFont(fp);
	//m_font = new FTBufferFont(fp);
	if(m_font->Error())
    {
        hdPrintf("Could not load font: %s\n", *fp);
		return;
    }
	m_font->FaceSize(m_fontsize);
    m_font->CharMap(ft_encoding_latin_1);
	
	if (strlen(text) > 0)
	{
		snprintf(m_text, kMaxFontTextSize, text);
	}

	simpleLayout.SetLineLength(320.0f);
	simpleLayout.SetLineSpacing(0.75f);
    simpleLayout.SetFont(m_font);
	simpleLayout.SetAlignment(FTGL::ALIGN_LEFT);
	
	this->SetupFontTexture(true);
}


void hdFTGLFont::SetRenderToTexture(const bool renderToTexture)
{
	m_renderToTexture = renderToTexture;
	if (m_renderToTexture)
	{
		simpleLayout.SetLineLength(320.0f);
	} 
	else
	{
		simpleLayout.SetLineLength(m_aabb.upper.x - m_aabb.lower.x);
	}
}


void hdFTGLFont::SetText(const char* text)
{
	snprintf(m_text, kMaxFontTextSize, text);
	
	if (m_renderToTexture)
	{
		this->SetupFontTexture(true);
	}
}


void hdFTGLFont::AlignCenter()
{
	simpleLayout.SetAlignment(FTGL::ALIGN_CENTER);
}


void hdFTGLFont::AlignRight()
{
	simpleLayout.SetAlignment(FTGL::ALIGN_RIGHT);
}


void hdFTGLFont::AlignLeft()
{
	simpleLayout.SetAlignment(FTGL::ALIGN_LEFT);
}


void hdFTGLFont::SetupFontTexture(bool reset)
{
	
	if (strlen(m_text) == 0) return;
	
	if (!reset) 
	{
		if (m_fontTexture != NULL) return;	
	}
	
	GLuint tempRenderBuffer;
	GLuint tempFrameBuffer;
	GLuint					oldRenderbuffer;
	GLuint					oldFramebuffer;

	glGetIntegerv(GL_RENDERBUFFER_BINDING_OES, (GLint *) &oldRenderbuffer);
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, (GLint *) &oldFramebuffer);
	
	glGenRenderbuffersOES(1, &tempRenderBuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, tempRenderBuffer);
	
	glGenFramebuffersOES(1, &tempFrameBuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, tempFrameBuffer);
	glRenderbufferStorageOES(GL_RENDERBUFFER_OES, GL_RGBA8_OES, 320, 480);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, tempRenderBuffer);
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrthof(0.0f, 
			 320.0f, 
			 0.0f, 
			 480.0f, 
			 100.0f, 
			 -100.0f);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(0.0f, 480.0f - m_fontsize, 0.0f);
	hdglColor4f(m_colorTint[0], m_colorTint[1], m_colorTint[2], m_colorTint[3]);
	simpleLayout.Render(m_text, -1, FTPoint(), renderMode);
	glPopMatrix();
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLubyte* pixels = (GLubyte *)malloc(sizeof(GLubyte) * 4 * 512 * 512);
	glReadPixels(0, 0, 512, 512, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	
	hdglError("After FTGL font render");
	
	m_fontTexture = hdTextureManager::Instance()->LoadTexture("**m_font**", pixels, 512.0f, 512.0f, TT_Sprite, 4);
	free(pixels);
	
	glDeleteRenderbuffersOES(1, &tempRenderBuffer);	
	glDeleteFramebuffersOES(1, &tempFrameBuffer);
	
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, oldFramebuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, oldRenderbuffer);
}


void hdFTGLFont::DrawFont()
{
	
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrthof(0.0f, 
			 320.0f, 
			 0.0f, 
			 480.0f, 
			 100.0f, 
			 -100.0f);
	
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glTranslatef(m_aabb.lower.x, m_aabb.lower.y, 0.0f);
	glPushMatrix();
	glEnable(GL_BLEND);
	glTranslatef(m_aabb.lower.x, m_aabb.lower.y, 0.0f);
	hdglColor4f(m_colorTint[0], m_colorTint[1], m_colorTint[2], hdClamp(m_alpha, 0.0f, m_colorTint[3]));
	simpleLayout.Render(m_text, -1, FTPoint(), renderMode);
	glDisable(GL_BLEND);
	glPopMatrix();
	
	glDisable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}


void hdFTGLFont::Draw() const
{
	
	if (this->IsHidden()) return;
	
	if (m_renderToTexture)
	{
		if (m_fontTexture == NULL) return;	
		//DrawVertices(((hdPolygon*)this)->GetVertices(), ((hdPolygon*)this)->GetTextureCoordinates(), 4, m_aabb, m_fontTexture);
		
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glEnable(GL_BLEND);
		
		hdglBegin(GL_QUADS);
		hdglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, m_fontTexture->texnum);
		float h = m_aabb.upper.y - m_aabb.lower.y;
		
		hdglTexCoord2f(0.0f, (480.0f - h)/512.0f);
		hdglVertex2f(m_aabb.lower.x, m_aabb.lower.y);
		
		hdglTexCoord2f(0.0f, 480.0f/512.0f);
		hdglVertex2f(m_aabb.lower.x, m_aabb.upper.y);
		
		hdglTexCoord2f(320.0f/512.0f, 480.0f/512.0f);
		hdglVertex2f(m_aabb.upper.x, m_aabb.upper.y);
		
		hdglTexCoord2f(320.0f/512.0f, (480.0f - h)/512.0f);
		hdglVertex2f(m_aabb.upper.x, m_aabb.lower.y);
		hdglEnd();
		glDisable(GL_BLEND);
		glPopMatrix();
		
		glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
	} 
	else
	{
		((hdFTGLFont *)this)->DrawFont();
	}
}