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

#include "hdGraphicsInterface.h"

void hdInitImmediateModeGL() {}


void hdResetArrayPointers() {}


void hdglClearBuffers()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void hdglBegin( GLenum prim ) {
	glBegin(prim);
}


void hdglOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	glOrtho(left, right, bottom, top, zNear, zFar);
}


void hdglOrthof(float left, float right, float bottom, float top, float zNear, float zFar)
{
	glOrtho((GLdouble)left, (GLdouble)right, (GLdouble)bottom, (GLdouble)top, (GLdouble)zNear, (GLdouble)zFar);
}


void hdglClipPlane(GLenum plane, const GLdouble *equation)
{
	glClipPlane(plane, equation);
}


void hdglClipPlanef(GLenum plane, const float *equation)
{
	GLdouble eqn[] = {equation[0], equation[1], equation[2], equation[3] };
	
	glClipPlane(plane, eqn);
}


void hdglBindTexture(const hdTexture *texture)
{
	int currentTexNum;
	
	if (texture == NULL)
		glBindTexture(GL_TEXTURE_2D, 0);
	else
	{
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexNum);
		
		if (texture->texnum != currentTexNum)
		{
			glBindTexture(GL_TEXTURE_2D, texture->texnum);
			
			hdglError("glBindTexture"); 
			
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapToGL( texture->WrapS ) );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapToGL( texture->WrapT ) );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilterToGL( texture->MipMap, texture->MinFilter ) );
			glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilterToGL( texture->MagFilter ) );		
			
			hdglError("glTexParameteri");
			
			if ( texture->type == TT_Wall || texture->type == TT_16Wall ) {
				glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4.0f );
			} 
			else
			{
				//glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 1.0f );
			}
			
			hdglError("glTexParameterf");
		}
	}
}


void hdglVertex3f( float x, float y, float z ) {
	glVertex3f(x, y, z);
}


void hdglVertex2i( GLint x, GLint y ) {
	glVertex2i(x, y);
}


void hdglVertex2f( float x, float y ) {
	glVertex2f(x, y);
}


void hdglColor4ub( GLubyte r, GLubyte g, GLubyte b, GLubyte a ) {
	glColor4ub(r, g, b, a);
}


void hdglColor4f( GLfloat r, GLfloat g, GLfloat b, GLfloat a ) {
	glColor4f(r, g, b, a);
}


void hdglTexCoord2i( GLint s, GLint t ) {
	glTexCoord2i(s, t);
}


void hdglTexCoord2f( GLfloat s, GLfloat t ) {
	glTexCoord2f(s, t);
}


void hdglEnd() {
	glEnd();
}



inline GLenum WrapToGL( TWrapMode mode )
{
	if( mode == Repeat )
	{
		return GL_REPEAT;
	}
	else
	{
		return GL_CLAMP_TO_EDGE;
	}
}


inline GLenum MagFilterToGL(TMagFilter MagFilter)
{	
	switch( MagFilter )
	{
		case Nearest: 
			return GL_NEAREST;
			
		case Linear: 
			return GL_LINEAR;
			
		default: 
			break;
	}
	
	return GL_LINEAR;
}


inline GLenum MinFilterToGL( bool MipMap, TMinFilter MinFilter )
{
	if (MipMap)
	{
		switch (MinFilter)
		{
			case NearestMipMapOff: 
				return GL_NEAREST;
				
			case NearestMipMapNearest: 
				return GL_NEAREST_MIPMAP_NEAREST;
				
			case NearestMipMapLinear: 
				return GL_NEAREST_MIPMAP_LINEAR;
				
			case LinearMipMapOff: 
				return GL_LINEAR;
				
			case LinearMipMapNearest: 
				return GL_LINEAR_MIPMAP_NEAREST;
				
			case LinearMipMapLinear: 
				return GL_LINEAR_MIPMAP_LINEAR;
				
			default: 
				break;
		}
	}
	else
	{
		switch( MinFilter )
		{
			case NearestMipMapOff:
			case NearestMipMapNearest:
			case NearestMipMapLinear:
				return GL_NEAREST;
				
			case LinearMipMapOff:
			case LinearMipMapNearest:
			case LinearMipMapLinear:
				return GL_LINEAR;
				
			default: 
				break;
		}
	}
	
	return GL_LINEAR;
}


bool hdglErrorCode(const char *userInfo)
{
	GLenum error = glGetError();
	bool ret = (error != GL_NO_ERROR);

	switch (error) 
	{
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			hdError(error, "GL Error: GL_INVALID_ENUM. %s\n\n", userInfo);
			break;
		case GL_INVALID_VALUE:
			hdError(error, "GL Error: GL_INVALID_VALUE. %s\n\n", userInfo);
			break;
		case GL_INVALID_OPERATION:
			hdError(error, "GL Error: GL_INVALID_OPERATION. %s\n\n", userInfo);
			break;
		case GL_STACK_OVERFLOW:
			hdError(error, "GL Error: GL_STACK_OVERFLOW. %s\n\n", userInfo);
			break;
		case GL_STACK_UNDERFLOW:
			hdError(error, "GL Error: GL_STACK_UNDERFLOW. %s\n\n", userInfo);
			break;
		case GL_OUT_OF_MEMORY:
			hdError(error, "GL Error: GL_OUT_OF_MEMORY. %s\n\n", userInfo);
			break;
		default:
			hdError(error, "GL Error: UNHANDLED ERRORCODE. %s\n\n", userInfo);
			break;
			
	}
	return ret;
}
