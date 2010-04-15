/*
 *  hdGL.h
 *  AnimationEngine
 *
 *  Created by david on 26/03/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 * General interface to different graphics platforms.
 *
 * Currently supporting openGL and openGLES
 *
 */

#ifndef HD_GRAPHICS_LIB
#define HD_GRAPHICS_LIB


//#ifdef __APPLE__
//#include <TargetConditionals.h>
//#endif

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#define TARGET_GL_OPENGLES 1
#endif

#if TARGET_GL_OPENGL == 1
	#ifdef __APPLE__
//#warning "Including open gl"
		#include <OpenGL/OpenGL.h>
		#include <OpenGL/gl.h>
	#endif

	#define hdglMaxTexSize 2048
#elif TARGET_GL_OPENGLES == 1
//#warning "Including open gl es"
	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
	#define GL_QUADS 888
	#define hdglMaxTexSize 1024

	typedef double GLdouble;
#else
	#warning "no gl directive found - including opengl by default" 
	#include <OpenGL/OpenGL.h>
	#include <OpenGL/gl.h>

	#define hdglMaxTexSize 2048
#endif

#include "hdCore/hdSystemSettings.h"
#include "hdGraphicsLibrary/hdTexture.h"




#ifdef __cplusplus
extern "C" {
#endif
	
	
	
	extern void hdInitImmediateModeGL();
	extern void hdResetArrayPointers();
	
	extern void hdglBegin( GLenum prim );
	
	extern void hdglOrtho (GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
	
	extern void hdglOrthof(float left, float right, float bottom, float top, float zNear, float zFar);
	
	extern void hdglClipPlane(GLenum plane, const GLdouble *equation);
	
	extern void hdglClipPlanef(GLenum plane, const float *equation);
	
	extern void hdglBindTexture(const hdTexture *texture);
	
	extern void hdglVertex3f( float x, float y, float z );
	extern void hdglVertex2f( float x, float y);
	extern void hdglVertex2i( GLint x, GLint y );
	extern void hdglColor4ub( GLubyte r, GLubyte g, GLubyte b, GLubyte a );
	extern void hdglColor4f( GLfloat r, GLfloat g, GLfloat b, GLfloat a );
	extern void hdglTexCoord2i( GLint s, GLint t );
	extern void hdglTexCoord2f( GLfloat s, GLfloat t );
	extern void hdglEnd();
	
	extern void hdglClearBuffers();
	
	extern void hdgluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
							GLfloat centerx, GLfloat centery, GLfloat centerz,
							GLfloat upx, GLfloat upy, GLfloat upz);
	

	extern void hdglError(const char* source);
	
	extern int hdglGenTextureFrameBuffer(const hdTexture *texture);
	
	extern void hdglBindTextureFrameBuffer(const GLuint fbo, 
										   float x, float y, 
										   float w, float h);
	
	extern void hdglDeleteTextureFrameBuffer(const GLuint fbo);
	
	
	
	extern GLenum WrapToGL( TWrapMode mode );
	
	extern GLenum MagFilterToGL( TMagFilter MagFilter );
	
	extern GLenum MinFilterToGL( bool MipMap, TMinFilter MinFilter );	
	
	
	
	

	
	
	
#ifdef __cplusplus
}
#endif

#endif