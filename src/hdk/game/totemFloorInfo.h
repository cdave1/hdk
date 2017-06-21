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

#ifndef _HDK_FLOOR_INFO_
#define _HDK_FLOOR_INFO_

#include <hdk/hdk.h>

#include "totemEnums.h"
#include "totemConstants.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class totemFloorInfo
{
public:
    totemFloorInfo();

    totemFloorInfo(const char *firstTextureName, const char *secondTextureName, const hdVec3& flowVector, float floorLevel, bool isReflective);

    ~totemFloorInfo();


    const bool IsDrawable() const;

    char m_firstTextureName[kMaxTexturePathSize];

    char m_secondTextureName[kMaxTexturePathSize];

    hdVec3 m_flowVector;

    float m_floorLevel;

    bool m_isReflective;

    bool m_isWater;

    float m_tint[4];

#ifdef LEVEL_EDITOR
    void ResetTextures();
#endif

    hdTexture* GetFirstTexture();

    hdTexture* GetSecondTexture();


private:
    friend class boost::serialization::access;

    hdTexture* m_firstTexture;

    hdTexture* m_secondTexture;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & m_firstTextureName;
        ar & m_secondTextureName;
        ar & m_floorLevel;
        ar & m_isReflective;
        if (version >= 1)
        {
            ar & m_isWater;
        }
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & m_firstTextureName;
        ar & m_secondTextureName;
        ar & m_floorLevel;
        ar & m_isReflective;

        if (version >= 1)
        {
            ar & m_isWater;
        }
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

BOOST_CLASS_VERSION(totemFloorInfo, 1)


inline const bool totemFloorInfo::IsDrawable() const
{
    return (((totemFloorInfo *)this)->GetFirstTexture() != NULL && m_isWater);
}


#ifdef LEVEL_EDITOR
inline void totemFloorInfo::ResetTextures()
{
    m_firstTexture = m_secondTexture = NULL;
}
#endif

inline hdTexture* totemFloorInfo::GetFirstTexture()
{
    if (m_firstTexture == NULL && strlen(m_firstTextureName) > 0)
    {
        m_firstTexture = hdTextureManager::Instance()->FindTexture(m_firstTextureName, TT_Wall);
    }
    
    return m_firstTexture;
}


inline hdTexture* totemFloorInfo::GetSecondTexture()
{
    if (m_secondTexture == NULL && strlen(m_secondTextureName) > 0)
    {
        m_secondTexture = hdTextureManager::Instance()->FindTexture(m_secondTextureName, TT_Wall);
    }
    
    return m_secondTexture;
}

#endif
