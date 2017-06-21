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

#ifndef _H_TOTEM_WORLD_
#define _H_TOTEM_WORLD_

#include "Box2D.h"
#include "totemGame.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

const int kMaxTotemWorldLevels = 128;

class totemLevel;

// Stores a list of levels
class totemWorld
{
private:

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & m_levelCount;
        ar & m_levels;
        ar & m_name;
    }

    int m_levelCount;

    totemLevel* m_levels[kMaxTotemWorldLevels];

    std::string m_name;


public:
    totemWorld();

    totemWorld(const char* name);

    ~totemWorld();

    int AddLevel(const totemLevel* level);

    int AddLevelAtIndex(const totemLevel* level, const int index);

    bool RemoveLevelAtIndex(const int index);

    totemLevel** GetLevels() const;

    const int GetLevelCount() const;

    void SetName(const char* name);

    const std::string GetName() const;

    void Print();
};


inline totemLevel** totemWorld::GetLevels() const
{
    return (totemLevel**)&m_levels;
}


inline const int totemWorld::GetLevelCount() const
{
    return m_levelCount;
}


inline void totemWorld::SetName(const char* name)
{
    m_name = name;
}


inline const std::string totemWorld::GetName() const
{
    return m_name;
}	


inline void totemWorld::Print()
{
    std::cout << "totem world: \"" << m_name << "\", levels: " << m_levelCount << '\n';
}

#endif