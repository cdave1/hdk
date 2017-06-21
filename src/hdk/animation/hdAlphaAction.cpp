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

#include "hdAlphaAction.h"
#include <hdk/graphics/hdPolygon.h>

hdAlphaAction::hdAlphaAction()
{
    m_startAlpha = 0.0f;
    m_endAlpha = 0.0f;
    m_alphaObject = new hdPolygon();
}


hdAlphaAction::~hdAlphaAction()
{
    delete m_alphaObject;
}


void hdAlphaAction::Apply(hdTimeInterval elapsed, hdGameObject* gameObject)
{
    float current;
    if (m_endAlpha > m_startAlpha)
    {
        current = (m_endAlpha - m_startAlpha) * ((m_progress) / m_duration);
    }
    else
    {
        current = 1.0f + (m_endAlpha - m_startAlpha) * ((m_progress) / m_duration);
    }

    current = hdClamp(current, 0.0f, 1.0f);

    gameObject->SetAlpha(current);

#ifdef ANIMATION_DEBUG
    hdPrintf("Alpha Action:: Name: %d, Progress: %f, Elapsed: %f, New Alpha: %f\n",
             gameObject->GetName(), m_progress, elapsed, (float)current);
#endif
}
