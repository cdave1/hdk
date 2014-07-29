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

#ifndef _HD_TRANSLATION_ACTION_H_
#define _HD_TRANSLATION_ACTION_H_

#include <hdAnimation/hdAction.h>

class hdTranslationAction : public hdAction
{
public:
    hdTranslationAction();

    ~hdTranslationAction() {}

    void Apply(hdTimeInterval elapsed, hdGameObject* gameObject);

    const hdVec3 GetStart() const
    {
        return m_start;
    }

    void SetStart(const hdVec3& pos)
    {
        m_start = pos;
    }

    const hdVec3 GetDestination() const
    {
        return m_destination;
    }

    void SetDestination(const hdVec3& pos)
    {
        m_destination = pos;
    }
    
    
private:
    hdVec3 m_start;
    
    hdVec3 m_destination;

};

#endif
