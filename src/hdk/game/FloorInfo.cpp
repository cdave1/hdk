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

#include "FloorInfo.h"

FloorInfo::FloorInfo()
{
    m_firstTextureName[0] = 0;
    m_secondTextureName[0] = 0;

    m_flowVector.Set(0,0,0);
    m_floorLevel = 0.0f;
    m_isWater = false;
    m_isReflective = false;

    m_tint[0] = 0.25f;
    m_tint[1] = 0.25f;
    m_tint[2] = 0.25f;
    m_tint[3] = 0.25f;

    m_firstTexture = NULL;
    m_secondTexture = NULL;
}


FloorInfo::FloorInfo(const char *firstTextureName, const char *secondTextureName,
                     const hdVec3& flowVector, float floorLevel,
                     bool isReflective)
{
    snprintf(m_firstTextureName, kMaxTexturePathSize, "%s", firstTextureName);
    snprintf(m_secondTextureName, kMaxTexturePathSize, "%s", secondTextureName);

    m_flowVector = flowVector;
    m_floorLevel = floorLevel;
    m_isWater = false;
    m_isReflective = isReflective;

    m_tint[0] = 0.25f;
    m_tint[1] = 0.25f;
    m_tint[2] = 0.25f;
    m_tint[3] = 0.25f;

    m_firstTexture = NULL;
    m_secondTexture = NULL;
}


FloorInfo::~FloorInfo()
{
}
