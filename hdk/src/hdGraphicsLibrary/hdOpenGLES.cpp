/*
 *  hdgl.cpp
 *  AnimationEngine
 *
 *  Created by david on 26/03/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdGraphicsInterface.h"

struct Vertex {
	float xyz[3];
	float st[2];
	GLubyte c[4];
};

#define MAX_VERTS 8192

typedef struct Vertex Vertex;
Vertex immediate[ MAX_VERTS ];
Vertex vab;
short quad_indexes[MAX_VERTS * 3 / 2 ];
int curr_vertex;
GLenum curr_prim;
bool initted = false;

void		hdInitImmediateModeGL() {

	

	glVertexPointer( 3, GL_FLOAT, sizeof( Vertex ), immediate[ 0 ].xyz );
	glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex ), immediate[ 0 ].st );
	glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( Vertex ), immediate[ 0 ].c );
	if (initted == false)
	{
		for ( int i = 0; i < MAX_VERTS * 3 / 2; i+=6 ) {
			int q = i / 6 * 4;
			quad_indexes[ i + 0 ] = q + 0;
			quad_indexes[ i + 1 ] = q + 1;
			quad_indexes[ i + 2 ] = q + 2;
			
			quad_indexes[ i + 3 ] = q + 0;
			quad_indexes[ i + 4 ] = q + 2;
			quad_indexes[ i + 5 ] = q + 3;
		}
		
		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );
		glEnableClientState( GL_COLOR_ARRAY );
		initted = true;
	}
}


void hdResetArrayPointers()
{
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glEnableClientState( GL_COLOR_ARRAY );
	glVertexPointer( 3, GL_FLOAT, sizeof( Vertex ), immediate[ 0 ].xyz );
	glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex ), immediate[ 0 ].st );
	glColorPointer( 4, GL_UNSIGNED_BYTE, sizeof( Vertex ), immediate[ 0 ].c );
}


void hdglOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar)
{
	glOrthof((float)left, (float)right, (float)bottom, (float)top, (float)zNear, (float)zFar);
}


void hdglOrthof(float left, float right, float bottom, float top, float zNear, float zFar)
{
	glOrthof(left, right, bottom, top, zNear, zFar);
}


void hdglClipPlane(GLenum plane, const double *equation)
{
	glClipPlanef(plane, (float *)equation);
}


void hdglClipPlanef(GLenum plane, const float *equation)
{
	glClipPlanef(plane, equation);
}


void hdglBindTexture(const hdTexture *texture)
{
	int currentTexNum;
	
	if (SystemSettings_OpenGLVersion() == 11)
	{
		if (texture == NULL)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			return;
		} 
		
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
	else
	{
		hdError(0xFF, "No matching open gl version. Texture was not bound.\n");
	}
}


void hdglClearBuffers()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void hdglBegin( GLenum prim ) {
	curr_vertex = 0;
	curr_prim = prim;
}


void hdglVertex3f( float x, float y, float z ) {
	assert( curr_vertex < MAX_VERTS );
	vab.xyz[ 0 ] = x;
	vab.xyz[ 1 ] = y;
	vab.xyz[ 2 ] = z;
	immediate[ curr_vertex ] = vab;
	curr_vertex++;
}


void hdglVertex2i( GLint x, GLint y ) {
	assert( curr_vertex < MAX_VERTS );
	vab.xyz[ 0 ] = (float)x;
	vab.xyz[ 1 ] = (float)y;
	vab.xyz[ 2 ] = 0.0f;
	immediate[ curr_vertex ] = vab;
	curr_vertex++;
}


void hdglVertex2f( float x, float y ) {
	assert( curr_vertex < MAX_VERTS );
	vab.xyz[ 0 ] = x;
	vab.xyz[ 1 ] = y;
	vab.xyz[ 2 ] = 0.0f;
	immediate[ curr_vertex ] = vab;
	curr_vertex++;
}


void hdglColor4ub( GLubyte r, GLubyte g, GLubyte b, GLubyte a ) {
	vab.c[ 0 ] = r;
	vab.c[ 1 ] = g;
	vab.c[ 2 ] = b;
	vab.c[ 3 ] = a;
}


void hdglColor4f( GLfloat r, GLfloat g, GLfloat b, GLfloat a ) {
	vab.c[ 0 ] = (GLubyte) ( r * 255 );
	vab.c[ 1 ] = (GLubyte) ( g * 255 );
	vab.c[ 2 ] = (GLubyte) ( b * 255 );
	vab.c[ 3 ] = (GLubyte) ( a * 255 );
}


void hdglTexCoord2i( GLint s, GLint t ) {
	vab.st[ 0 ] = (float)s;
	vab.st[ 1 ] = (float)t;
}


void hdglTexCoord2f( GLfloat s, GLfloat t ) {
	vab.st[ 0 ] = s;
	vab.st[ 1 ] = t;
}


void hdglEnd() {
	if (curr_vertex == 0) 
	{
		curr_prim = 0;
		return;
	}
	
	if ( curr_prim == GL_QUADS ) {
		glDrawElements( GL_TRIANGLES, curr_vertex / 4 * 6, GL_UNSIGNED_SHORT, quad_indexes );
	} else {
		glDrawArrays( curr_prim, 0, curr_vertex );
	}
	curr_vertex = 0;
	curr_prim = 0;
}


void hdgluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
			   GLfloat centerx, GLfloat centery, GLfloat centerz,
			   GLfloat upx, GLfloat upy, GLfloat upz)
{
    GLfloat m[16];
    GLfloat x[3], y[3], z[3];
    GLfloat mag;
    
    /* Make rotation matrix */
    
    /* Z vector */
    z[0] = eyex - centerx;
    z[1] = eyey - centery;
    z[2] = eyez - centerz;
    mag = sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);
    if (mag) {          /* mpichler, 19950515 */
        z[0] /= mag;
        z[1] /= mag;
        z[2] /= mag;
    }
    
    /* Y vector */
    y[0] = upx;
    y[1] = upy;
    y[2] = upz;
    
    /* X vector = Y cross Z */
    x[0] = y[1] * z[2] - y[2] * z[1];
    x[1] = -y[0] * z[2] + y[2] * z[0];
    x[2] = y[0] * z[1] - y[1] * z[0];
    
    /* Recompute Y = Z cross X */
    y[0] = z[1] * x[2] - z[2] * x[1];
    y[1] = -z[0] * x[2] + z[2] * x[0];
    y[2] = z[0] * x[1] - z[1] * x[0];
    
    /* mpichler, 19950515 */
    /* cross product gives area of parallelogram, which is < 1.0 for
     * non-perpendicular unit-length vectors; so normalize x, y here
     */
    
    mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);
    if (mag) {
        x[0] /= mag;
        x[1] /= mag;
        x[2] /= mag;
    }
    
    mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);
    if (mag) {
        y[0] /= mag;
        y[1] /= mag;
        y[2] /= mag;
    }
    
#define M(row,col)  m[col*4+row]
    M(0, 0) = x[0];
    M(0, 1) = x[1];
    M(0, 2) = x[2];
    M(0, 3) = 0.0;
    M(1, 0) = y[0];
    M(1, 1) = y[1];
    M(1, 2) = y[2];
    M(1, 3) = 0.0;
    M(2, 0) = z[0];
    M(2, 1) = z[1];
    M(2, 2) = z[2];
    M(2, 3) = 0.0;
    M(3, 0) = 0.0;
    M(3, 1) = 0.0;
    M(3, 2) = 0.0;
    M(3, 3) = 1.0;
#undef M
    glMultMatrixf(m);
    
    /* Translate Eye to Origin */
    glTranslatef(-eyex, -eyey, -eyez);
    
}





extern int hdglGenTextureFrameBuffer(const hdTexture *texture)
{
	GLuint fbo, status;
	
	glGenFramebuffersOES(1, &fbo);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, fbo);
	glFramebufferTexture2DOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_TEXTURE_2D, texture->texnum, 0);
	
	status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
	if (status != GL_FRAMEBUFFER_COMPLETE_OES)
	{
		hdPrintf("Could not create frame buffer.");
		return -1;
	}
	else
	{
		return fbo;
	}
}


extern void hdglBindTextureFrameBuffer(const GLuint fbo, 
									   float x, float y, 
									   float w, float h)
{
#ifndef LEVEL_EDITOR
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, fbo);
	glViewport(0, 0, w, h);
#endif
}


extern void hdglDeleteTextureFrameBuffer(const GLuint fbo)
{
#ifndef LEVEL_EDITOR
	GLuint status;
	
	glDeleteFramebuffersOES(1, &fbo);
	
	status = glCheckFramebufferStatusOES(GL_FRAMEBUFFER_OES);
#endif
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
#ifdef DEBUG
	GLenum error = glGetError();
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
#endif
}
