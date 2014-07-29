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

#ifndef _HD_SCALE_ACTION_HD_
#define _HD_SCALE_ACTION_HD_

#include "hdAnimation/hdAction.h"

class hdScaleAction : public hdAction
{
public:
    hdScaleAction();

    ~hdScaleAction() {}

    void Apply(hdTimeInterval elapsed, hdGameObject* gameObject);

    const hdVec3 GetScale() const
    {
        return m_scale;
    }

    void SetScale(const hdVec3& scale)
    {
        m_scale = scale;
    }
    
    
private:
    hdVec3 m_scale;

};

#endif