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

#include "hdUIImage.h"

hdUIImage::hdUIImage(const char* texture,
                     hdGameWorld* gameWorld) : hdReceiver(gameWorld)
{
    m_texture = hdTextureManager::Instance()->FindTexture(texture, TT_16Pic);
    this->SetAs2DBox(hdVec3(0, 0, 0), hdVec3(1.0f, 1.0f, 1.0f));

    m_tint = new float[4];
    m_tint[0] = 1.0f;
    m_tint[1] = 1.0f;
    m_tint[2] = 1.0f;
    m_tint[3] = 1.0f;
}


hdUIImage::hdUIImage(const char* texture,
                     hdGameWorld* gameWorld,
                     const hdAABB& aabb) : hdReceiver(gameWorld)
{
    m_texture = hdTextureManager::Instance()->FindTexture(texture, TT_16Pic);
    this->SetAs2DBox(aabb.lower, aabb.upper);

    m_tint = new float[4];
    m_tint[0] = 1.0f;
    m_tint[1] = 1.0f;
    m_tint[2] = 1.0f;
    m_tint[3] = 1.0f;
}


hdUIImage::hdUIImage(const char* texture,
                     hdGameWorld* gameWorld,
                     const float x, const float y,
                     const float w, const float h) : hdReceiver(gameWorld)
{
    m_texture = hdTextureManager::Instance()->FindTexture(texture, TT_16Pic);
    this->SetAs2DBox(hdVec3(x,y,0.0f), hdVec3(x+w,y+h,0.0f));

    m_tint = new float[4];
    m_tint[0] = 1.0f;
    m_tint[1] = 1.0f;
    m_tint[2] = 1.0f;
    m_tint[3] = 1.0f;
}


hdUIImage::~hdUIImage()
{
    delete [] m_tint;
}


bool hdUIImage::MouseDown(float x, float y)
{
    if (this->IsHidden()) return false;
    return (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)));
}


// If we did not click on the item, then state can not go into mouse over.
bool hdUIImage::MouseOver(float x, float y)
{
    if (this->IsHidden()) return false;
    return (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)));
}


bool hdUIImage::MouseUp(float x, float y)
{
    if (this->IsHidden()) return false;

    if (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)))
    {
        return true;
    }
    else
    {
        return false;
    }
}


void hdUIImage::Draw() const
{
    if (this->IsHidden()) return;
    DrawVerticesWithTint(((hdReceiver*)this)->GetVertices(), 
                         ((hdReceiver*)this)->GetTextureCoordinates(), 
                         4, m_aabb, m_texture, m_tint, m_alpha);
}
