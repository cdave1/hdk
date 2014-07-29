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

#ifndef HD_UI_CHECKBOX_H
#define HD_UI_CHECKBOX_H

#include <hdk/hdButton.h>

class hdCheckbox : public hdReceiver
{
public:
    hdCheckbox(const char *textureOnNormal,
               const char *textureOnOver,
               const char *textureOffNormal,
               const char *textureOffOver,
               hdGameWorld* gameWorld);

    ~hdCheckbox();

    void SetAs2DBox(const float& x, const float& y, const float& w, const float& h);

    bool MouseDown(float x, float y);

    bool MouseOver(float x, float y);

    bool MouseUp(float x, float y);

    bool MouseDoubleClick(float x, float y);

    void Draw() const;

    void Disable();

    void Enable();

    void Toggle();

    bool IsOn() const;

    void SetOn();

    void SetOff();

    void AddValueChangedListener(void *obj, void (*func)(void *, void *));

private:
    hdButton *m_onButton;

    hdButton *m_offButton;

    hdButton *m_activeButton;

    void (*m_valueChangedCallback)(void *, void *);

    void *m_callbackObject;
};



inline void hdCheckbox::Disable()
{
    m_onButton->Disable();
    m_offButton->Disable();
    m_isEnabled = false;
}


inline void hdCheckbox::Enable()
{
    m_onButton->Enable();
    m_offButton->Enable();
    m_isEnabled = true;
}

#endif
