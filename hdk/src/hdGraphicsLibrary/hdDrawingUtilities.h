/*
 *  hdGraphicsLibrary
 */

#ifndef _HD_DRAWING_UTILITIES_H_
#define _HD_DRAWING_UTILITIES_H_

#include "hdGraphicsLibrary/hdGraphicsInterface.h"
#include "hdGraphicsLibrary/hdPolygon.h"
#include "hdGraphicsLibrary/hdTexture.h"
#include "hdGraphicsLibrary/hdTextureManager.h"

#define HDGL_EFFECT_DOUBLE_INTENSITY 1



#ifdef __cplusplus
extern "C" {
#endif

	extern void SetupPointSprites();
	
	extern void EnablePointSprite();
	
	extern void DisablePointSprite();
	
extern void DrawPolygon(hdPolygon* polygon, hdTexture* texture);

extern void DrawPolygonWithTint(hdPolygon* polygon, hdTexture* texture, 
								const float* colorTint, const float alpha);

extern void DrawExtrudedPolygonWithTint(hdPolygon* polygon, 
										hdTexture* texture, const float* colorTint, 
										const float alpha, const float depth);
	
extern void DrawVertices(hdVec3* vertices, hdVec2* textureCoordinates, 
						 const int vertexCount, const hdAABB& aabb, 
						 hdTexture* texture);

extern void DrawVerticesWithTint(hdVec3* vertices, hdVec2* textureCoordinates, 
								 const int vertexCount, const hdAABB& aabb, 
								 hdTexture* texture, const float* colorTint, 
								 const float alpha);
	
	extern void DrawVerticesWithColor(hdVec3* vertices, hdVec2* textureCoordinates, 
									 const int vertexCount, const hdAABB& aabb, 
									 hdTexture* texture, const hdColor4& color, 
									 const float alpha);

	
	//extern void DrawQuadWithTint(const hdVec3& center, const float halfWidth,  hdTexture* texture, const float* colorTint, 
	//							 const float alpha, const float depth);
	
extern	void DrawExtrusion(hdVec3* vertices, hdVec2* textureCoordinates, 
						   const int vertexCount, const hdAABB& aabb, 
						   hdTexture* texture, const float* colorTint, 
						   const float alpha, const float depth, bool lighting,
						   bool reflected, 
						   bool doDrawing,
						   const hdVec3& screenCenter);


/*
 * Generates texture coordinates to be used in the extrusion drawing functions.
 */
extern	int GenerateExtrusionTextureVertices(hdVec2* outTexCoords, hdVec3* vertices, hdVec2* textureCoordinates, 
												 const int vertexCount, const hdAABB& aabb, 
												 hdTexture* texture, const float depth,
												const float textureRepeat);
	
extern	void DrawExtrusionFlatQuad(hdVec3* vertices, hdVec2* textureCoordinates, 
									   const int vertexCount, const hdAABB& aabb, 
									   hdTexture* texture, const float* colorTint, 
									   const float alpha, const float depth,bool reflected,
								    const hdVec3& screenCenter);
	
	
extern	void DrawExtrusionLighting(hdVec3* vertices, hdVec2* textureCoordinates, 
							   const int vertexCount, const hdAABB& aabb, 
							   hdTexture* texture, const float* colorTint, 
							   const float alpha, const float depth,bool reflected,
								    const hdVec3& screenCenter);

extern void DrawPrimitiveWithTint(GLenum prim, hdVec3* vertices, hdVec2* textureCoordinates, const int vertexCount, const hdAABB& aabb, hdTexture* texture, const float* colorTint, const float alpha);

/*
 * Simply adds the vertices and texture coordindates for either triangles or quads
 * drawing.
 */
extern void AddVerticesForTQ(GLenum prim, hdVec3* vertices, hdVec2* textureCoordinates, const int vertexCount, hdTexture* texture, const float* colorTint, const float alpha);
	
extern void DrawMultiTextureQuad(hdVec3* vertices, hdTexture *firstTexture, hdTexture *secondTexture, float alpha);

	
	extern void StartTextureEffect(uint8 effect);
	
	extern void StopTextureEffect(uint8 effect);
	
#ifdef __cplusplus
}
#endif


#endif