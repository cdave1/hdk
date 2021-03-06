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

#ifndef _HDK_GAME_GAME_OBJECT_H_
#define _HDK_GAME_GAME_OBJECT_H_

#include "Box2D.h"
#include "Enums.h"

class GameObject
{
public:
    GameObject() {}
    GameObject(e_hdkType type, void * pointer)
    {
        m_type = type;
        m_pointer = pointer;
    }

    e_hdkType m_type;
    void * m_pointer;
};

#endif
