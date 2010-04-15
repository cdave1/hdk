/*
 *  hdGLESIncludes.h
 *  LevelEditor
 *
 *  Created by david on 17/03/09.
 *  Copyright 2009 n/a. All rights reserved.
 * 
 * Adapted from wolf3d's gles_glue.c under gpl licence.
 *
 *
 
 Copyright (C) 2009 Id Software, Inc.
 
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 
 *
 *
 *
 *
 */

/*
 *  gluLookAt.h
 *
 *  This is a modified version of the function of the same name from 
 *  the Mesa3D project ( http://mesa3d.org/ ), which is  licensed
 *  under the MIT license, which allows use, modification, and 
 *  redistribution
 *
 *  In order to work under OpenGL ES, all instances of GLdouble
 *  had to be changed to GLfloat, and all "d" function calls had
 *  to be changed to the "f" versions.
 *
 *  Original developer's comments have been left in place.
 *
 *  Out of respect for the original authors, this is licensed under
 *  the Mesa (MIT) license. Original license follows:
 *  
 *  -----------------------------------------------------------------------
 *
 *  Copyright (C) 1999-2007  Brian Paul   All Rights Reserved.
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a
 *  copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.
 
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 *  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 *  BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 *  AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 *  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *
 * David note: this function was changed to hdgluLookAt
 */

#ifndef _HD_OPEN_GL_ES_
#define _HD_OPEN_GL_ES_

#include <assert.h>


#if TARGET_GL_OPENGL == 1
	#ifdef __APPLE__
		#include <OpenGL/OpenGL.h>
		#include <OpenGL/gl.h>
	#endif

	#define hdglMaxTexSize 2048
#if TARGET_GL_OPENGLES == 1

	#include <OpenGLES/ES1/gl.h>
	#include <OpenGLES/ES1/glext.h>
	#define GL_QUADS 888
	#define hdglMaxTexSize 1024
#else
	#ifdef __APPLE__
		#include <OpenGL/OpenGL.h>
		#include <OpenGL/gl.h>
	#endif

	#define hdglMaxTexSize 2048
#endif



#include "hdSettings.h"



#ifdef __cplusplus
extern "C" {
#endif
	


extern void hdInitImmediateModeGL();
extern void hdResetArrayPointers();
extern void hdglBegin( GLenum prim );
extern void hdglVertex3f( float x, float y, float z );
extern void hdglVertex2f( float x, float y);
extern void hdglVertex2i( GLint x, GLint y );
extern void hdglColor4ub( GLubyte r, GLubyte g, GLubyte b, GLubyte a );
extern void hdglColor4f( GLfloat r, GLfloat g, GLfloat b, GLfloat a );
extern void hdglTexCoord2i( GLint s, GLint t );
extern void hdglTexCoord2f( GLfloat s, GLfloat t );
extern void hdglEnd();

extern void hdgluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
				   GLfloat centerx, GLfloat centery, GLfloat centerz,
				   GLfloat upx, GLfloat upy, GLfloat upz);
	
#ifdef DEBUG
#include "hdCommon.h"
#endif
extern void hdglError(const char* source);
	
#ifdef __cplusplus
}
#endif

#endif