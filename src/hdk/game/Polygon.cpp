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

#include "Polygon.h"

Polygon::Polygon() : hdPolygon()
{
    m_userType = (int)e_hdkTypePolygon;
    m_relativeObject = NULL;
    m_colorTint[0] = 1.0f;
    m_colorTint[1] = 1.0f;
    m_colorTint[2] = 1.0f;
    m_colorTint[3] = 1.0f;
}


Polygon::Polygon(hdGameWorld *gameWorld, const char* textureName) : hdPolygon(gameWorld)
{
    m_userType = (int)e_hdkTypePolygon;
    m_relativeObject = NULL;
    m_isHidden = false;
    if (strlen(textureName) == 0)
    {
        m_texture = NULL;
    }
    else
    {
        snprintf(m_textureName, kMaxTexturePathSize, "%s", textureName);
        m_texture = hdTextureManager::Instance()->FindTexture(m_textureName, TT_Wall);
    }
    m_colorTint[0] = 1.0f;
    m_colorTint[1] = 1.0f;
    m_colorTint[2] = 1.0f;
    m_colorTint[3] = 1.0f;
}


Polygon::Polygon(hdGameWorld *gameWorld, const char* textureName, const float* colorTint) : hdPolygon(gameWorld)
{
    m_userType = (int)e_hdkTypePolygon;
    m_relativeObject = NULL;
    m_isHidden = false;
    if (strlen(textureName) == 0)
    {
        m_texture = NULL;
    }
    else
    {
        snprintf(m_textureName, kMaxTexturePathSize, "%s", textureName);
        m_texture = hdTextureManager::Instance()->FindTexture(m_textureName, TT_Wall);
    }
    m_colorTint[0] = colorTint[0];
    m_colorTint[1] = colorTint[1];
    m_colorTint[2] = colorTint[2];
    m_colorTint[3] = colorTint[3];
}


void Polygon::Init(hdGameWorld *gameWorld)
{ 
    ((hdPolygon *)this)->Init(gameWorld);
}


Polygon::~Polygon()
{
}


void Polygon::Draw() const
{
    DrawPolygonWithTint((hdPolygon *)this, m_texture, m_colorTint, m_alpha);
}
