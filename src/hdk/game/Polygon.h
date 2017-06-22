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

#ifndef _HDK_POLYGON_H
#define _HDK_POLYGON_H

#include <hdk/hdk.h>
#include "Enums.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

struct hdGameObjectDef;
class hdPolygon;

class Polygon : public hdDrawable, public hdPolygon
{
public:
    Polygon();

    Polygon(hdGameWorld *gameWorld, const char* textureName);

    Polygon(hdGameWorld *gameWorld, const char* textureName, const float* colorTint);

    ~Polygon();

    void Init(hdGameWorld *gameWorld);

    void Draw() const;

    hdGameObject* GetRelative();

    void SetRelative(hdGameObject* gameObject);

    void SetUpdateCallback(const void* obj, void (*func)(void *, void *));

    void RemoveCallbackFunctions();

    void DoUpdateCallback(hdTimeInterval);

private:
    friend class boost::serialization::access;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & boost::serialization::base_object<hdPolygon>(*this);
        ar & m_textureName;
        if (version >= 1)
        {
            ar & m_colorTint;
        }
    }


    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<hdPolygon>(*this);
        ar & m_textureName;
        m_texture = hdTextureManager::Instance()->FindTexture(m_textureName, TT_Wall);
        if (version >= 1)
        {
            ar & m_colorTint;
        }
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    char m_textureName[kMaxTexturePathSize];

    float m_colorTint[4];

    hdTexture* m_texture;

    hdGameObject* m_relativeObject;

    void *m_callbackObj;

    void (*m_updateFunc)(void *, void *);

};
BOOST_CLASS_VERSION(Polygon, 1)


inline hdGameObject* Polygon::GetRelative()
{
    return m_relativeObject;
}


inline void Polygon::SetRelative(hdGameObject* object)
{
    m_relativeObject = object;
}


inline void Polygon::SetUpdateCallback(const void* obj, void (*func)(void *, void *))
{
    m_callbackObj = (void *)obj;
    m_updateFunc = func;
}


inline void Polygon::RemoveCallbackFunctions()
{	
    m_callbackObj = NULL;
    m_updateFunc = NULL;
}


inline void Polygon::DoUpdateCallback(hdTimeInterval)
{
    if (m_callbackObj == NULL) return;
    (*m_updateFunc)(m_callbackObj, this);
}

#endif
