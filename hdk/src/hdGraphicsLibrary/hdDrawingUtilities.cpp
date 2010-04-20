/*
 *  hdDrawingUtilities.cpp
 *  hdGameEngine
 *
 *  Created by david on 16/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdDrawingUtilities.h"


void SetupPointSprites()
{
#if TARGET_GL_OPENGL == 1
	GLfloat sizes[2];
	glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, sizes);
	//float quadratic[] =  { 1.0f, 0.0f, 0.01f };
	//glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION, quadratic );
	glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE, 32.0f );
	glPointParameterf( GL_POINT_SIZE_MIN, sizes[0] );
	glPointParameterf( GL_POINT_SIZE_MAX, sizes[1] );
#elif TARGET_GL_OPENGLES == 1
	GLfloat sizes[2];
	glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, sizes);
	//float quadratic[] =  { 1.0f, 0.0f, 0.01f };
	//glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, quadratic );
	//glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE, 32.0f );
	glPointParameterf( GL_POINT_SIZE_MIN, sizes[0] );
	glPointParameterf( GL_POINT_SIZE_MAX, sizes[1] );
	glTexEnvi(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_TRUE);
#else
	GLfloat sizes[2];
	glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, sizes);
	//float quadratic[] =  { 1.0f, 0.0f, 0.01f };
	//glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION, quadratic );
	glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE, 32.0f );
	glPointParameterf( GL_POINT_SIZE_MIN, sizes[0] );
	glPointParameterf( GL_POINT_SIZE_MAX, sizes[1] );
#endif
}


void EnablePointSprite()
{
#if TARGET_GL_OPENGL == 1
	glEnable(GL_POINT_SPRITE_ARB);
	//GLfloat sizes[2];
	//glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, sizes);
	//float quadratic[] =  { 1.0f, 0.0f, 0.01f };
	//glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION, quadratic );
	//glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE, 32.0f );
	//glPointParameterf( GL_POINT_SIZE_MIN, sizes[0] );
	//glPointParameterf( GL_POINT_SIZE_MAX, sizes[1] );
	glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);
#elif TARGET_GL_OPENGLES == 1
	glEnable(GL_POINT_SPRITE_OES);
	//GLfloat sizes[2];
	//glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, sizes);
	//float quadratic[] =  { 1.0f, 0.0f, 0.01f };
	//glPointParameterfv( GL_POINT_DISTANCE_ATTENUATION, quadratic );
	//glPointParameterf( GL_POINT_FADE_THRESHOLD_SIZE, 32.0f );
	//glPointParameterf( GL_POINT_SIZE_MIN, sizes[0] );
	//glPointParameterf( GL_POINT_SIZE_MAX, sizes[1] );
	//glTexEnvi(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_TRUE);
#else
	glEnable(GL_POINT_SPRITE_ARB);
#endif
}

void DisablePointSprite()
{
#if TARGET_GL_OPENGL == 1
	glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_FALSE);
	glDisable(GL_POINT_SPRITE_ARB);
#elif TARGET_GL_OPENGLES == 1
	//glTexEnvi(GL_POINT_SPRITE_OES, GL_COORD_REPLACE_OES, GL_FALSE);
	glDisable(GL_POINT_SPRITE_OES);
#else
	glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_FALSE);
	glDisable(GL_POINT_SPRITE_ARB);	
#endif
}


void DrawPolygon(hdPolygon* polygon, hdTexture* texture)
{
	if (polygon->IsHidden()) return;
	DrawVertices(polygon->GetVertices(), polygon->GetTextureCoordinates(), polygon->GetVertexCount(), polygon->GetAABB(), texture);
}


void DrawPolygonWithTint(hdPolygon* polygon, hdTexture* texture, const float* colorTint, const float alpha)
{
	if (polygon->IsHidden()) return;
	DrawVerticesWithTint(polygon->GetVertices(), polygon->GetTextureCoordinates(), polygon->GetVertexCount(), polygon->GetAABB(), texture, colorTint, alpha);
}


void DrawExtrudedPolygonWithTint(hdPolygon* polygon, hdTexture* texture, const float* colorTint, const float alpha, const float depth, const hdVec3& screenCenter)
{
	if (polygon->IsHidden()) return;
	DrawVerticesWithTint(polygon->GetVertices(), polygon->GetTextureCoordinates(), polygon->GetVertexCount(), polygon->GetAABB(), texture, colorTint, alpha);
	hdglBegin(GL_QUADS);
	DrawExtrusion(polygon->GetVertices(), polygon->GetTextureCoordinates(), 
				  polygon->GetVertexCount(), polygon->GetAABB(), texture, colorTint, alpha, depth, false, false, false, screenCenter);
	hdglEnd();
}



void DrawVertices(hdVec3* vertices, hdVec2* textureCoordinates, const int vertexCount, const hdAABB& aabb, hdTexture* texture)
{
	float tint[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	DrawVerticesWithTint(vertices, textureCoordinates, vertexCount, aabb, texture, tint, 1.0f);
}






#define FLAT_FRONT_VERTEX_SHADE 0.85f
#define FLAT_BACK_VERTEX_SHADE 0.55f

void DrawExtrusion(hdVec3* vertices, hdVec2* textureCoordinates, 
				   const int vertexCount, const hdAABB& aabb, 
				   hdTexture* texture, const float* colorTint, 
				   const float alpha, const float depth, 
				   bool lighting, bool reflected, 
				   bool doDrawing,
				   const hdVec3& screenCenter)
{
	if (vertexCount == 0) return;
	

	
	if (lighting && fabs(depth) > 0.05f)
	{
		if (doDrawing)
		{
			StartTextureEffect(HDGL_EFFECT_DOUBLE_INTENSITY);
			
			hdglBegin(GL_QUADS);
		}
		
		DrawExtrusionLighting(vertices, textureCoordinates, 
							  vertexCount, aabb, 
							  texture, colorTint, 
							  alpha, depth, reflected, screenCenter);
		
		if (doDrawing)
		{
			
			
			hdglEnd();
			
			StopTextureEffect(HDGL_EFFECT_DOUBLE_INTENSITY);
		}
	}
	else
	{
		if (doDrawing)
		{
			hdglBegin(GL_QUADS);
		}
		
		DrawExtrusionFlatQuad(vertices, textureCoordinates, 
							  vertexCount, aabb, 
							  texture, colorTint, 
							  alpha, depth, reflected, screenCenter);
		
		if (doDrawing)
		{
			hdglEnd();
		}
	}
	

}



int GenerateExtrusionTextureVertices(hdVec2* outTexCoords, 
									 hdVec3* vertices, 
									 hdVec2* textureCoordinates, 
									 const int vertexCount, const hdAABB& aabb, 
									 hdTexture* texture, const float depth,
									 const float textureRepeat)
{
	int i1, i2, stride;
	float texCoord1, texCoord2;
	float perim, multiplier;
	hdVec3 diff;
	
	if (vertexCount > 4)
	{
		diff = aabb.upper - aabb.lower;
		perim = 0.33f * ((diff.x) + (diff.y));
		multiplier = fabs(perim/(depth*2));
	}
	else
	{
		multiplier = vertexCount * textureRepeat;
	}
	
	for (int i = 0; i < vertexCount; ++i)
	{
		i1 = i;
		i2 = ((i+1) >= vertexCount) ? (i+1)-vertexCount : (i+1);
		stride = i * 4;
		
		if (textureCoordinates[i1].y == textureCoordinates[i2].y)
		{
			outTexCoords[0 + stride].Set(textureCoordinates[i1].x, 0.0f);
			outTexCoords[1 + stride].Set(textureCoordinates[i1].x, 1.0f);
			outTexCoords[2 + stride].Set(textureCoordinates[i2].x, 1.0f);
			outTexCoords[3 + stride].Set(textureCoordinates[i2].x, 0.0f);
		}
		else if (textureCoordinates[i1].x == textureCoordinates[i2].x)
		{
			outTexCoords[0 + stride].Set(0.0f, textureCoordinates[i1].y);
			outTexCoords[1 + stride].Set(1.0f, textureCoordinates[i1].y);
			outTexCoords[2 + stride].Set(1.0f, textureCoordinates[i2].y);
			outTexCoords[3 + stride].Set(0.0f, textureCoordinates[i2].y);
		}
		else 
		{
			texCoord1 = multiplier * (float)i1/(float)vertexCount;
			texCoord2 = multiplier * (float)(i1+1)/(float)vertexCount;
			
			outTexCoords[0 + stride].Set(0.0f, texCoord1);
			outTexCoords[1 + stride].Set(1.0f, texCoord1);
			outTexCoords[2 + stride].Set(1.0f, texCoord2);
			outTexCoords[3 + stride].Set(0.0f, texCoord2);
		}
	}
	return stride;
}




void DrawExtrusionFlatQuad(hdVec3* vertices, hdVec2* textureCoordinates, 
						   const int vertexCount, const hdAABB& aabb, 
						   hdTexture* texture, const float* colorTint, 
						   const float alpha, const float depth, bool reflected,
						   const hdVec3& screenCenter)
{
	short i1, i2, stride;
	//int cross;
	hdVec3 normal;
	hdColor4 frontColor, backColor;
	
	frontColor.Setf(colorTint[0] * FLAT_FRONT_VERTEX_SHADE, 
					  colorTint[1] * FLAT_FRONT_VERTEX_SHADE,
					  colorTint[2] * FLAT_FRONT_VERTEX_SHADE, 
					  hdClamp(alpha, 0.0f, colorTint[3]));
	
	backColor.Setf(colorTint[0] * FLAT_BACK_VERTEX_SHADE, 
				   colorTint[1] * FLAT_BACK_VERTEX_SHADE, 
				   colorTint[2] * FLAT_BACK_VERTEX_SHADE, 
				   hdClamp(alpha, 0.0f, colorTint[3]));
	
	for (int i = 0; i < vertexCount; ++i)
	{
		i1 = i;
		i2 = ((i+1) >= vertexCount) ? (i+1)-vertexCount : (i+1);
		stride = i * 4;
		
		normal = vertices[i2] - vertices[i1];
		//cross = hdCrossProductSign(screenCenter, vertices[i1], (vertices[i1]-normal));
		
		//if ((cross <= 0) || (reflected && cross > 0))
		{
			
						
			if (texture != NULL) 
			{
				hdglTexCoord2f(textureCoordinates[0 + stride].x, 
							   textureCoordinates[0 + stride].y);
			}
			
			hdglColor4ub(frontColor.r, frontColor.g, frontColor.b, frontColor.a);
			hdglVertex3f(vertices[i1].x, vertices[i1].y, vertices[i1].z - depth);
			
			if (texture != NULL) 
			{
				hdglTexCoord2f(textureCoordinates[1 + stride].x, 
							   textureCoordinates[1 + stride].y);
			}
			
			hdglColor4ub(backColor.r, backColor.g, backColor.b, backColor.a);
			hdglVertex3f(vertices[i1].x, vertices[i1].y, vertices[i1].z + depth);
			
			if (texture != NULL) 
			{
				hdglTexCoord2f(textureCoordinates[2 + stride].x, 
							   textureCoordinates[2 + stride].y);
			}
			
			hdglColor4ub(backColor.r, backColor.g, backColor.b, backColor.a);
			hdglVertex3f(vertices[i2].x, vertices[i2].y, vertices[i2].z + depth);
			
			if (texture != NULL) 
			{
				hdglTexCoord2f(textureCoordinates[3 + stride].x, 
							   textureCoordinates[3 + stride].y);
			}
			
			hdglColor4ub(frontColor.r, frontColor.g, frontColor.b, frontColor.a);
			hdglVertex3f(vertices[i2].x, vertices[i2].y, vertices[i2].z - depth);
			
			
		}
	}
}


static bool hasGeneratedLookup = false;
#define DOT_LOOKUP_SIZE 1024
#define DOT_LOOKUP_BITMASK 1023
static uint8 colorDotLookup[DOT_LOOKUP_SIZE];
/*
 * Below code - waste of time; too slow.
 */
inline uint8 hdColorDot(float color, float normal)
{
	unsigned i;
	float lookupColor, lookupNormal, lookupStep;
	
	if (!hasGeneratedLookup)
	{
		lookupStep = 1.0f / (float)DOT_LOOKUP_SIZE;
		memset(colorDotLookup, 0, sizeof(colorDotLookup));
		
		lookupColor = lookupNormal = 0.0f;
		
		for (i = 0; i < DOT_LOOKUP_SIZE; ++i)
		{
			colorDotLookup[i] = (uint8)((int)(255 * lookupColor)) & DOT_LOOKUP_BITMASK;
			lookupColor += lookupStep;
		}
		hasGeneratedLookup = true;
	}
	
	return colorDotLookup[((uint32)(color * normal * DOT_LOOKUP_SIZE)) & DOT_LOOKUP_BITMASK];
}

static GLint envMode, combineRGB, src0RGB, src1RGB;
void StartTextureEffect(uint8 effect)
{
	glGetTexEnviv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, &envMode);
	glGetTexEnviv(GL_TEXTURE_ENV, GL_COMBINE_RGB, &combineRGB);
	glGetTexEnviv(GL_TEXTURE_ENV, GL_SRC0_RGB, &src0RGB);
	glGetTexEnviv(GL_TEXTURE_ENV, GL_SRC1_RGB, &src1RGB);
	
	if (effect == HDGL_EFFECT_DOUBLE_INTENSITY)
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
		glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 2.0f);
		glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
		glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
	}
}


void StopTextureEffect(uint8 effect)
{
	if (effect == HDGL_EFFECT_DOUBLE_INTENSITY)
	{
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, envMode);
		glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, combineRGB);
		glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, src0RGB);
		glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, src1RGB);
		glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE, 1.0f);
	}
}


#define FRONT_VERTEX_SHADE 0.85f
#define BACK_VERTEX_SHADE 0.55f
#define SHADING_MODIFIER 0.5f
/*
 * This is a very crappy per vertex shader.
 *
 * Code below takes a normal for each vertex pair
 * (perp. to line made by pair) and then calculates the light
 * for the quad as a modification to the tint. Light is
 * an ambient value pointing along the negative y axis.
 *
 * TODO: Variable light angle.
 *
 * Funky normal smoothing by taking the
 * average normal of adjacent vertex pairs and using a 
 * texture combine at 2x the rgb scale so brights look
 * brighter and darks look darker.
 */
void DrawExtrusionLighting(hdVec3* vertices, hdVec2* textureCoordinates, 
						   const int vertexCount, const hdAABB& aabb, 
						   hdTexture* texture, const float* colorTint, 
						   const float alpha, const float depth,bool reflected,
						   const hdVec3& screenCenter)
{
	unsigned i;
	short i1, i2, i3, i4, stride;
	
	hdMatrix inColors, normals, outColors;
	float fAlpha;
	float lightNormal;
	float prevNormal, currNormal, nextNormal;
	float norm0, norm1;
	
	unsigned char precalcAlpha;
	
	
	

	lightNormal = 0.0f;
	
	prevNormal = Get2DNormal(vertices[vertexCount - 1], vertices[0]);
	currNormal = Get2DNormal(vertices[0], vertices[1]);
	nextNormal = Get2DNormal(vertices[1], vertices[2]);
	
	float firstVertexNormal, secondVertexNormal;
	
	/*
	 * Precalcs
	 */ 
	precalcAlpha = hdClamp((int)(255 * alpha), 0, (int)(255 * colorTint[3]));
	fAlpha = hdClamp(alpha, 0.0f, colorTint[3]);
	
	MatrixIdentity(inColors);
	MatrixIdentity(normals);	
	MatrixIdentity(outColors);

#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1) && defined(__ARM_NEON__)
	for(i=0; i < 16; ++i)
	{
		inColors.f[i] = colorTint[i >> 2];
	}
#else
	for(i=0; i < 16; ++i)
	{
		inColors.f[i] = colorTint[i & 3];
	}
#endif
	
	

	

	
	for (int i = 0; i < vertexCount; ++i)
	{
		i1 = i;
		i2 = ((i+1) >= vertexCount) ? (i+1)-vertexCount : (i+1); // shitty modulus
		i3 = ((i+2) >= vertexCount) ? (i+2)-vertexCount : (i+2);
		i4 = ((i+3) >= vertexCount) ? (i+3)-vertexCount : (i+3);
		stride = i * 4;

		hdVec3 normal = vertices[i2] - vertices[i1];
		//int cross = hdCrossProductSign(screenCenter, vertices[i1], (vertices[i1]-normal));
		
		//if ((cross <= 0) || (reflected && cross > 0))
		{
			// no smoothing for objects with sharp angles
			// TODO: use angle beween i1, i2, i3 for this. 
			if (vertexCount < 6) 
			{
				lightNormal = hdClamp((1.0f + currNormal) * SHADING_MODIFIER, 0.25f, 1.0f);
				norm0 = FRONT_VERTEX_SHADE * lightNormal;
				norm1 = BACK_VERTEX_SHADE * lightNormal;
				
				normals.f[__11] = norm0;
				normals.f[__22] = norm1;
				normals.f[__33] = norm1;
				normals.f[__44] = norm0;
				
				hdMatrixMultiply(outColors, inColors, normals);
			}
			else
			{
				firstVertexNormal = hdClamp((1.0f + (prevNormal + currNormal) * 0.5f) * SHADING_MODIFIER, 0.2f, 1.0f);
				secondVertexNormal = hdClamp((1.0f + (currNormal + nextNormal) * 0.5f) * SHADING_MODIFIER, 0.2f, 1.0f);
				
				normals.f[__11] = FRONT_VERTEX_SHADE * firstVertexNormal;
				normals.f[__22] = BACK_VERTEX_SHADE * firstVertexNormal;
				normals.f[__33] = BACK_VERTEX_SHADE * secondVertexNormal;
				normals.f[__44] = FRONT_VERTEX_SHADE * secondVertexNormal;
				
				hdMatrixMultiply(outColors, inColors, normals);
			}

			
			
			
			
			//BIG HACK
			// HACK HACK HACK
			// Vals come out of NEON in the wrong order.
#if (TARGET_IPHONE_SIMULATOR == 0) && (TARGET_OS_IPHONE == 1) && defined(__ARM_NEON__)
			if (texture != NULL) 
			{ 
				hdglTexCoord2f(textureCoordinates[0 + stride].x, 
							   textureCoordinates[0 + stride].y);
			}
			
			hdglColor4f(outColors.f[__11], outColors.f[__12], outColors.f[__13], fAlpha);
			hdglVertex3f(vertices[i1].x, vertices[i1].y, vertices[i1].z - depth);
			
			if (texture != NULL) 
			{
				hdglTexCoord2f(textureCoordinates[1 + stride].x, 
							   textureCoordinates[1 + stride].y);
			}
			
			hdglColor4f(outColors.f[__21], outColors.f[__22], outColors.f[__23], fAlpha);
			
			hdglVertex3f(vertices[i1].x, vertices[i1].y, vertices[i1].z + depth);
			
			if (texture != NULL) 
			{
				hdglTexCoord2f(textureCoordinates[2 + stride].x, 
							   textureCoordinates[2 + stride].y);
			}
			
			
			hdglColor4f(outColors.f[__31], outColors.f[__32], outColors.f[__33], fAlpha);
			hdglVertex3f(vertices[i2].x, vertices[i2].y, vertices[i2].z + depth);
			
			if (texture != NULL) 
			{
				hdglTexCoord2f(textureCoordinates[3 + stride].x, 
							   textureCoordinates[3 + stride].y);
			}
			hdglColor4f(outColors.f[__41], outColors.f[__42], outColors.f[__43], fAlpha);
			
			hdglVertex3f(vertices[i2].x, vertices[i2].y, vertices[i2].z - depth);
#else
			 
			
			if (texture != NULL) 
			{ 
				hdglTexCoord2f(textureCoordinates[0 + stride].x, 
							   textureCoordinates[0 + stride].y);
			}
			hdglColor4f(outColors.f[__11], outColors.f[__21], outColors.f[__31], fAlpha);
			hdglVertex3f(vertices[i1].x, vertices[i1].y, vertices[i1].z - depth);
			
			if (texture != NULL) 
			{
				hdglTexCoord2f(textureCoordinates[1 + stride].x, 
							   textureCoordinates[1 + stride].y);
			}
			hdglColor4f(outColors.f[__12], outColors.f[__22], outColors.f[__32], fAlpha);
			hdglVertex3f(vertices[i1].x, vertices[i1].y, vertices[i1].z + depth);
			
			if (texture != NULL) 
			{
				hdglTexCoord2f(textureCoordinates[2 + stride].x, 
							   textureCoordinates[2 + stride].y);
			}
			hdglColor4f(outColors.f[__13], outColors.f[__23], outColors.f[__33], fAlpha);
			hdglVertex3f(vertices[i2].x, vertices[i2].y, vertices[i2].z + depth);
			
			if (texture != NULL) 
			{
				hdglTexCoord2f(textureCoordinates[3 + stride].x, 
							   textureCoordinates[3 + stride].y);
			}
			hdglColor4f(outColors.f[__14], outColors.f[__24], outColors.f[__34], fAlpha);
			hdglVertex3f(vertices[i2].x, vertices[i2].y, vertices[i2].z - depth);
#endif
			
		}

		
		prevNormal = currNormal;
		currNormal = nextNormal;
		nextNormal = Get2DNormal(vertices[i3], vertices[i4]);
	}

	

	 
}







static hdTexture* nullTexture = NULL;
void DrawVerticesWithTint(hdVec3* vertices, hdVec2* textureCoordinates, 
						  const int vertexCount, const hdAABB& aabb,
						  hdTexture* texture, const float* colorTint, 
						  const float alpha)
{
	if (vertexCount < 3) return;
	
	GLenum prim = 0;
	
	if (vertexCount == 3)
	{
		prim = GL_TRIANGLES;
	} 
	else if (vertexCount == 4)
	{
		prim = GL_QUADS;
	}
	else 
	{
#if TARGET_GL_OPENGL == 1
		prim = GL_POLYGON;
#else
		prim = GL_TRIANGLE_FAN;
#endif
	}
	
	
	DrawPrimitiveWithTint(prim, vertices, textureCoordinates, vertexCount, aabb, texture, colorTint, alpha);
}


void DrawVerticesWithColor(hdVec3* vertices, hdVec2* textureCoordinates, 
						  const int vertexCount, const hdAABB& aabb,
						  hdTexture* texture, const hdColor4& color, 
						  const float alpha)
{
	float colorTint[4];
	
	if (vertexCount < 3) return;
	
	colorTint[0] = ((float)color.r) / 255.0f;
	colorTint[1] = ((float)color.g) / 255.0f;
	colorTint[2] = ((float)color.b) / 255.0f;	
	colorTint[3] = ((float)color.a) / 255.0f;
	
	DrawVerticesWithTint(vertices, textureCoordinates, vertexCount, aabb, texture, colorTint, alpha);
}



void DrawPrimitiveWithTint(GLenum prim, hdVec3* vertices, hdVec2* textureCoordinates, const int vertexCount, const hdAABB& aabb, hdTexture* texture, const float* colorTint, const float alpha)
{
	GLboolean blendEnabled;
	GLint blendFuncSrc, blendFuncDst;
	
	if (vertexCount < 3) return;
	
	if (nullTexture == NULL)
	{
		nullTexture = hdTextureManager::Instance()->GetNullTexture();
	}
	
	glMatrixMode(GL_MODELVIEW);
	
	if (texture == NULL || texture == nullTexture)
	{
		hdglBindTexture(NULL);
	} 
	else 
	{
		hdglBindTexture(texture);
		
		if (texture->isPremultipliedAlpha)
		{
			glGetBooleanv(GL_BLEND, &blendEnabled);
			glGetIntegerv(GL_BLEND_SRC, &blendFuncSrc);
			glGetIntegerv(GL_BLEND_DST, &blendFuncDst);
			
			if (hdClamp(alpha, 0.0f, colorTint[3]) == 1.0f)
			{
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			}
			else
			{
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
		}
	}
	
	
	

	
	hdVec3 center(0.0f, 0.0f, 0.0f);
	
	hdglBegin(prim);
	hdglColor4f(colorTint[0], colorTint[1], colorTint[2], hdClamp(alpha, 0.0f, colorTint[3]));
	
	for (int i = 0; i < vertexCount; ++i)
	{
		hdVec3 v = center + vertices[i];
		if (textureCoordinates != NULL && texture != nullTexture)
		{
			hdglTexCoord2f(textureCoordinates[i].x, textureCoordinates[i].y);
		}
		hdglVertex3f(v.x, v.y, v.z);
	}
	
	hdglEnd();
	
	if (texture == NULL || texture == nullTexture)
	{
	} 
	else
	{
		if (texture->isPremultipliedAlpha)
		{
			glBlendFunc(blendFuncSrc, blendFuncDst);
		}
		
		//glBindTexture(GL_TEXTURE_2D, 0);
	}
#ifdef LEVEL_EDITOR	
	hdglBegin(GL_LINE_LOOP);
	hdglColor4f(0.0f, 0.0f, 0.0f, hdClamp(alpha, 0.0f, colorTint[3]));
	for (int32 i = 0; i < vertexCount; ++i)
	{
		hdVec3 v = center + vertices[i];
		hdglVertex3f(v.x, v.y, v.z);
	}
	hdglEnd();
#endif
}



void AddVerticesForTQ(GLenum prim, hdVec3* vertices, hdVec2* textureCoordinates, 
					  const int vertexCount, hdTexture* texture, const float* colorTint, const float alpha)
{
	hdglColor4f(colorTint[0], colorTint[1], colorTint[2], hdClamp(alpha, 0.0f, colorTint[3]));
	//if(prim == GL_TRIANGLES || prim == GL_QUADS)
	//{
		for (int i = 0; i < vertexCount; ++i)
		{
			if (texture != NULL) 
			{
				hdglTexCoord2f(textureCoordinates[i].x, textureCoordinates[i].y);
			}
			hdglVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
		}
	/*}
	else if(prim == GL_TRIANGLE_STRIP)
	{
		for (int i = 0; i < vertexCount - 2; ++i)
		{
			if (texture != NULL) 
			{
				hdglTexCoord2f(textureCoordinates[i].x, textureCoordinates[i].y);
				hdglTexCoord2f(textureCoordinates[i+1].x, textureCoordinates[i+1].y);
				hdglTexCoord2f(textureCoordinates[i+2].x, textureCoordinates[i+2].y);
			}
			hdglVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
			hdglVertex3f(vertices[i+1].x, vertices[i+1].y, vertices[i+1].z);
			hdglVertex3f(vertices[i+2].x, vertices[i+2].y, vertices[i+2].z);
		}
	}
	else
	{
		for (int i = 0; i < vertexCount; ++i)
		{
			if (texture != NULL) 
			{
				hdglTexCoord2f(textureCoordinates[0].x, textureCoordinates[0].y);
				hdglTexCoord2f(textureCoordinates[i].x, textureCoordinates[i].y);
				hdglTexCoord2f(textureCoordinates[(i+1)%vertexCount].x, textureCoordinates[(i+1)%vertexCount].y);
			}
			hdglVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
			hdglVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
			hdglVertex3f(vertices[(i+1)%vertexCount].x, vertices[(i+1)%vertexCount].y, vertices[(i+1)%vertexCount].z);
		}
	}*/
}



static float t_trans = 0.0f;
void DrawMultiTextureQuad(hdVec3* vertices, hdTexture *firstTexture, hdTexture *secondTexture, float alpha)
{
	
#if 1
	hdglBindTexture(firstTexture);
	
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glTranslatef(t_trans, t_trans, 0.0f);
	
	//glPushMatrix();
	//glTranslatef(-t_trans, t_trans*3, 0.0f);
	
	t_trans = t_trans+0.0005f;
	
	if (t_trans >= 2.0f) t_trans = 0.0f;
	
	hdglBegin(GL_QUADS);
	//hdglColor4f(1.0f, 1.0f, 1.0f, 0.05f);
	hdglTexCoord2f(0.0f, 0.0f);
	hdglColor4ub(255, 255, 255, 13);
	hdglVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
	
	hdglTexCoord2f(0.0f, 2.0f);
	hdglColor4ub(255, 255, 255, 255);
	//hdglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	hdglVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
	
	hdglTexCoord2f(2.0f, 2.0f);
	hdglColor4ub(255, 255, 255, 255);
	//hdglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	hdglVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
	
	hdglTexCoord2f(2.0f, 0.0f);
	hdglColor4ub(255, 255, 255, 13);
	//hdglColor4f(1.0f, 1.0f, 1.0f, 0.05f);
	hdglVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
	hdglEnd();
	
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	
	//glBindTexture(GL_TEXTURE_2D, 0);
#else
	
	const GLfloat squareTexCoords[] = {
		//-0.5, 0.0,
		//-0.5, 1.0,
		//0.5, 1.0,
		//0.5, 0.0
		
		0.0f, 0.0,
		0.0f, 2.0,
		2.0f, 2.0,
		2.0f, 0.0
		
		//-2.0, 0.0,
		//-0.5, 1.0,
		//2.0, 1.0,
		//0.5, 0.0
	};
	
	hdglError("before DrawMultiTextureQuad");

	hdglBegin(GL_QUADS);	
	glDisableClientState(GL_COLOR_ARRAY);
	
	glClientActiveTexture(GL_TEXTURE0); // first texture
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, squareTexCoords);
	
	glClientActiveTexture(GL_TEXTURE1); // second texture
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, squareTexCoords);
	
	// Enable 2D texturing for the first texture.
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	
	// Enable 2D texturing for the second texture.
	glActiveTexture(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	
	// We are trying for the multi-textured lighting effect from the OpenGL
	// ES 2.0 book, page 223, Figure 10-3. The relevant shader equation is
	// basecolor * (lightcolor + 0.25), so we set the constant as a base colour
	glColor4f(0.25f, 0.25f, 0.25f, 0.75f); // hdClamp(alpha, 0.0f, 1.0f));
	
	// Set the light map has the current texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, firstTexture->texnum);
	
	// add the light map to the constant 0.25
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_TEXTURE);
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_PREVIOUS);
	
	// Now set the background map (bricks) and modulate (multiply)
	glActiveTexture(GL_TEXTURE1);
	// we'll keep this code here as _texSelection will never be our lightmap
	glBindTexture(GL_TEXTURE_2D, secondTexture->texnum);
	
	/* Set the texture environment mode for this texture to combine */
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
	
	/* Set the method we're going to combine the two textures by. */
	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
	
	/* Use the previous combine texture as source 0*/
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC0_RGB, GL_PREVIOUS);
	
	/* Use the current texture as source 1 */
	glTexEnvi(GL_TEXTURE_ENV, GL_SRC1_RGB, GL_TEXTURE);
	
	/*
	 Set what we will operate on, in this case we are going to use
	 just the texture colours.
	 */
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
	//glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB, GL_SRC_COLOR);
	//glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA, GL_SRC_ALPHA);
	
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glActiveTexture(GL_TEXTURE1);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glTranslatef(t_trans, t_trans, 0.0f);
	
	glActiveTexture(GL_TEXTURE0);
	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glTranslatef(-t_trans, t_trans*3, 0.0f);
	
	t_trans = t_trans+0.0005f;
	
	if (t_trans >= 2.0f) t_trans = 0.0f;
	
	glActiveTexture(GL_TEXTURE1);
	glEnableClientState(GL_COLOR_ARRAY);
	//hdglColor4f(1.0f, 1.0f, 1.0f, 0.05f);
	hdglColor4ub(255, 255, 255, 13);
	hdglVertex3f(vertices[0].x, vertices[0].y, vertices[0].z);
	hdglColor4ub(255, 255, 255, 255);
	//hdglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	hdglVertex3f(vertices[1].x, vertices[1].y, vertices[1].z);
	hdglColor4ub(255, 255, 255, 255);
	//hdglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	hdglVertex3f(vertices[2].x, vertices[2].y, vertices[2].z);
	hdglColor4ub(255, 255, 255, 13);
	//hdglColor4f(1.0f, 1.0f, 1.0f, 0.05f);
	hdglVertex3f(vertices[3].x, vertices[3].y, vertices[3].z);
	hdglEnd();
	
	glActiveTexture(GL_TEXTURE0);
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	
	glActiveTexture(GL_TEXTURE1);
	glMatrixMode(GL_TEXTURE);
	glPopMatrix();
	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
	
	glClientActiveTexture(GL_TEXTURE0);
	hdResetArrayPointers();
	glActiveTexture(GL_TEXTURE0);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	hdglError("After DrawMultiTextureQuad");
#endif
}



