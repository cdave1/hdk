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

#include "hdIBButton.h"
#include "hdButton.h"

hdIBButton::hdIBButton() : hdIBProxy()
{
    m_interfaceElement = NULL;
}


hdIBButton::hdIBButton(const char *textureNormal,
                       const char *textureOver,
                       const char *textureUp,
                       const hdVec3 &lower,
                       const hdVec3 &upper) : hdIBProxy()
{
    this->SetNormalTexture(textureNormal);
    this->SetOverTexture(textureOver);
    this->SetUpTexture(textureUp);
    this->SetAs2DBox(lower, upper);
    m_interfaceElement = NULL;
}


hdIBButton::~hdIBButton()
{
}


hdReceiver * hdIBButton::CreateInterfaceElement()
{
    this->ResetAABB();
    this->ResetOBB();
    if (!m_interfaceElement)
    {
        m_interfaceElement = new hdButton(m_textureNormal,
                                          m_textureOver,
                                          m_textureUp,
                                          NULL,
                                          this->GetAABB());
    }
    return m_interfaceElement;
}


void hdIBButton::Draw() const
{
    if (m_interfaceElement)
        m_interfaceElement->Draw();
}
