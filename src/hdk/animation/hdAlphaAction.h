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

#ifndef _HD_ALPHA_ACTION_H_
#define _HD_ALPHA_ACTION_H_

#include <hdk/animation/hdAction.h>

class hdAlphaAction : public hdAction
{
public:
    hdAlphaAction();

    ~hdAlphaAction();

    void Apply(hdTimeInterval elapsed, hdGameObject* gameObject);

    const float GetStartAlpha() const
    {
        return m_startAlpha;
    }

    void SetStartAlpha(const float& alpha)
    {
        m_startAlpha = alpha;
    }

    const float GetEndAlpha() const
    {
        return m_endAlpha;
    }

    void SetEndAlpha(const float& alpha)
    {
        m_endAlpha = alpha;
    }

    hdGameObject* GetAlphaObject() const;


private:
    hdGameObject* m_alphaObject;

    float m_startAlpha;
    
    float m_endAlpha;
    
};


inline hdGameObject* hdAlphaAction::GetAlphaObject() const
{
    return m_alphaObject;
}

#endif
