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

#include "hdCheckbox.h"

hdCheckbox::hdCheckbox(const char *textureOnNormal,
                       const char *textureOnOver,
                       const char *textureOffNormal,
                       const char *textureOffOver,
                       hdGameWorld* gameWorld)  : hdReceiver(gameWorld, true)
{
    // create two buttons
    m_onButton = new hdButton(textureOnNormal,
                              textureOnOver,
                              textureOnOver);

    m_offButton = new hdButton(textureOffNormal,
                               textureOffOver,
                               textureOffOver);

    m_activeButton = m_onButton;

    m_callbackObject = NULL;
    m_valueChangedCallback = NULL;
}


hdCheckbox::~hdCheckbox()
{
    if (m_onButton)
        delete m_onButton;

    if (m_offButton)
        delete m_offButton;
}


void hdCheckbox::SetAs2DBox(const float& x, const float& y, const float& w, const float& h)
{
    m_onButton->SetAs2DBox(x, y, w, h);
    m_offButton->SetAs2DBox(x, y, w, h);
}

void hdCheckbox::AddValueChangedListener(void *obj, void (*func)(void *, void *))
{
    m_callbackObject = obj;
    m_valueChangedCallback = func;
}



bool hdCheckbox::MouseDown(float x, float y)
{
    if (!this->isEnabled()) return false;
    if (this->IsHidden()) return false;

    return m_activeButton->MouseDown(x, y);
}


bool hdCheckbox::MouseOver(float x, float y)
{
    if (!this->isEnabled()) return false;
    if (this->IsHidden()) return false;

    return m_activeButton->MouseOver(x, y);
}


bool hdCheckbox::MouseUp(float x, float y)
{
    bool res;

    if (!this->isEnabled()) return false;
    if (this->IsHidden()) return false;

    res = m_activeButton->MouseUp(x, y);
    if (res)
    {
        Toggle();
        if (m_callbackObject != NULL && m_valueChangedCallback != NULL)
        {
            (*m_valueChangedCallback)(m_callbackObject, this);
        }
    }

    return res;
}

bool hdCheckbox::MouseDoubleClick(float x, float y)
{
    if (!this->isEnabled()) return false;
    if (this->IsHidden()) return false;

    return m_activeButton->MouseDoubleClick(x, y);
}


void hdCheckbox::Toggle()
{
    if (m_activeButton == m_onButton)
        m_activeButton = m_offButton;
    else
        m_activeButton = m_onButton;
}


void hdCheckbox::SetOn()
{
    hdAssert(m_onButton != NULL);
    m_activeButton = m_onButton;
}


void hdCheckbox::SetOff()
{
    hdAssert(m_offButton != NULL);
    m_activeButton = m_offButton;
}


void hdCheckbox::Draw() const
{
    m_activeButton->Draw();
}


bool hdCheckbox::IsOn() const
{
    return (m_activeButton == m_onButton);
}
