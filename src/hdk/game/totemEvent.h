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

#ifndef _TOTEM_EVENT_H_
#define _TOTEM_EVENT_H_

#include "totemEnums.h"
#include "totemBlock.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class totemEvent : public hdDrawable, public hdPolygon
{
public:
    totemEvent();

    totemEvent(hdGameWorld *gameWorld);

    totemEvent(hdGameWorld *gameWorld,
               e_totemEventType eventType,
               hdGameObject* firstObject,
               hdGameObject* secondObject);

    ~totemEvent();

    void Init(hdGameWorld *gameWorld);

    void Step();

    void Draw() const;

#ifdef LEVEL_EDITOR
    bool AABBIntersection(const hdAABB& aabb);
#endif

    const e_totemEventType GetEventType() const;

    const int SetFirstObject(const hdGameObject* body);

    const hdGameObject* GetFirstObject() const;

    const int SetSecondObject(const hdGameObject* body);

    const hdGameObject* GetSecondObject() const;

protected:
    float m_colorTint[4];

    e_totemEventType m_eventType;

    hdGameObject* m_firstObject;

    hdGameObject* m_secondObject;


private:
    void InternalDrawEvent(float x, float y, float z) const;

    friend class boost::serialization::access;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        if (version >= 1)
        {
            ar & boost::serialization::base_object<hdPolygon>(*this);
        }
        ar & m_eventType;
        ar & m_firstObject;
        ar & m_secondObject;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        if (version >= 1)
        {
            ar & boost::serialization::base_object<hdPolygon>(*this);
        }
        ar & m_eventType;
        ar & m_firstObject;
        ar & m_secondObject;
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};


BOOST_CLASS_VERSION(totemEvent, 1)

inline 	const e_totemEventType totemEvent::GetEventType() const
{
    return m_eventType;
}


inline const hdGameObject* totemEvent::GetFirstObject() const
{
    return m_firstObject;
}


inline const hdGameObject* totemEvent::GetSecondObject() const
{
    return m_secondObject;
}

#endif
