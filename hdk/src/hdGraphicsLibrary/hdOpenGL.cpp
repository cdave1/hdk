/*
 *  hdGL.cpp
 *  AnimationEngine
 *
 *  Created by david on 26/03/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdGraphicsInterface.h"

void		hdInitImmediateModeGL() {}

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
//	glTexCoord2i(s, t);
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


inline GLenum MagFilterToGL( TMagFilter MagFilter )
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
	if( MipMap )
	{
		switch( MinFilter )
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

void hdglError(const char* source)
{
	GLenum error = glGetError();
//#warning "hdglError - not printing to stdout!!!"
	switch (error) {
		case GL_NO_ERROR:
			break;
		case GL_INVALID_ENUM:
			hdError(error, "GL Error: GL_INVALID_ENUM. %s\n\n", source);
			break;
		case GL_INVALID_VALUE:
			//hdPrintf("%s\n", source);
			hdError(error, "GL Error: GL_INVALID_VALUE. %s\n\n", source);
			break;
		case GL_INVALID_OPERATION:
			hdError(error, "GL Error: GL_INVALID_OPERATION. %s\n\n", source);
			break;
		case GL_STACK_OVERFLOW:
			hdError(error, "GL Error: GL_STACK_OVERFLOW. %s\n\n", source);
			break;
		case GL_STACK_UNDERFLOW:
			hdError(error, "GL Error: GL_STACK_UNDERFLOW. %s\n\n", source);
			break;
		case GL_OUT_OF_MEMORY:
			hdError(error, "GL Error: GL_OUT_OF_MEMORY. %s\n\n", source);
			break;
		default:
			break;
	}
}