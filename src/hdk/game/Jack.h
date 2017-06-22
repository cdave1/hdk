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

#ifndef _HDK_GAME_JACK_H
#define _HDK_GAME_JACK_H

#include "Enums.h"

#include <hdk/physics.h>
#include <hdk/graphics.h>

class Jack : public hdPolygon, public hdPhysicsObject, public hdDrawable
{
    friend class hdGameWorld;
    friend class hdGameObject;
    friend class hdPolygon;

public:
    Jack();

    Jack(hdGameWorld *gameWorld,
              b2World *physicsWorld,
              const hdVec2& position);

    ~Jack();

    void Init(hdGameWorld *gameWorld)
    {
        ((hdPolygon *)this)->Init(gameWorld);
    }

    void Init(hdGameWorld *gameWorld,
              b2World *physicsWorld);

    void Draw() const;

    const hdVec3* GetAABB();

    void Step();

    void CreatePhysicsBody();

    void Reset();


private:
    hdTexture* m_texture;

    float m_colorTint[4];

    friend class boost::serialization::access;

    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & boost::serialization::base_object<hdPolygon>(*this);
    }
    
    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<hdPolygon>(*this);
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
};

#endif
