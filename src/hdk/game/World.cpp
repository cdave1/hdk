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

#include "World.h"

World::World()
{
    m_levelCount = 0;
    m_name = "";
    memset(m_levels, 0, sizeof(m_levels));
    hdPrintf("###### Creating world\n");
}


World::World(const char* name)
{
    m_levelCount = 0;
    m_name = name;
    memset(m_levels, 0, sizeof(m_levels));
}


World::~World()
{
    for (int i = 0; i < m_levelCount; ++i)
    {
        delete m_levels[i];
        m_levels[i] = NULL;
    }
    m_levelCount = 0;
}


int World::AddLevel(const Level* level)
{
    hdAssert(m_levelCount >= 0 && m_levelCount <= kMaxTotemWorldLevels);

    if (m_levelCount == kMaxTotemWorldLevels) return false;

    return this->AddLevelAtIndex(level, (const int)m_levelCount);
}


int World::AddLevelAtIndex(const Level* level, const int index)
{
    hdAssert(m_levelCount >= 0 && m_levelCount <= kMaxTotemWorldLevels);
    if (m_levelCount == kMaxTotemWorldLevels) return -1;
    if (index > m_levelCount) return -1;
    if (index < 0) return -1;

    // Make room for the new level
    for (int i = m_levelCount; i >= index && i < (kMaxTotemWorldLevels - 1); --i)
    {
        m_levels[i+1] = m_levels[i];
    }
    m_levels[index] = (Level *)level;
    ++m_levelCount;
    return index;
}


bool World::RemoveLevelAtIndex(const int index)
{
    hdAssert(m_levelCount >= 0 && m_levelCount <= kMaxTotemWorldLevels);
    if (m_levelCount == 0) return false;
    if (index >= m_levelCount) return false;
    if (index < 0) return false;

    delete m_levels[index];
    m_levels[index] = NULL;
    
    // move everything down
    for (int i = index; i < m_levelCount-1; ++i)
    {
        m_levels[i] = m_levels[i+1];
    }
    m_levels[m_levelCount-1] = NULL;
    --m_levelCount;
    return true;
}
