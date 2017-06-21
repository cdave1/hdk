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

#ifndef _HD_DRAWING_UTILITIES_H_
#define _HD_DRAWING_UTILITIES_H_

#include <hdk/graphics/hdGraphicsInterface.h>
#include <hdk/graphics/hdPolygon.h>
#include <hdk/graphics/hdTexture.h>
#include <hdk/graphics/hdTextureManager.h>

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

    extern void DrawPrimitiveWithTint(GLenum prim, hdVec3* vertices, hdVec2* textureCoordinates,
                                      const int vertexCount, const hdAABB& aabb, hdTexture* texture,
                                      const float* colorTint, const float alpha);

    /*
     * Simply adds the vertices and texture coordindates for either triangles or quads
     * drawing.
     */
    extern void AddVerticesForTQ(GLenum prim, hdVec3* vertices, hdVec2* textureCoordinates,
                                 const int vertexCount, hdTexture* texture, const float* colorTint,
                                 const float alpha);

    extern void DrawMultiTextureQuad(hdVec3* vertices, hdTexture *firstTexture,
                                     hdTexture *secondTexture, float alpha);

    extern void StartTextureEffect(uint8 effect);

    extern void StopTextureEffect(uint8 effect);
    
#ifdef __cplusplus
}
#endif

#endif
