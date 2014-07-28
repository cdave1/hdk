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

#include "hdIBImage.h"
#include "hdUIImage.h"

hdIBImage::hdIBImage() : hdIBProxy()
{
    m_interfaceElement = NULL;
}


hdIBImage::hdIBImage(const char *texture,
                     const float x, const float y,
                     const float w, const float h) : hdIBProxy()
{
    this->SetTexture(texture);
    this->SetAs2DBox(hdVec3(x,y,0.0f), hdVec3(x+w,y+h,0.0f));
    m_interfaceElement = NULL;
}


hdIBImage::hdIBImage(const char *texture,
                     const hdVec3 &lower, const hdVec3 &upper) : hdIBProxy()
{
    this->SetTexture(texture);
    this->SetAs2DBox(lower, upper);
    m_interfaceElement = NULL;
}


hdIBImage::~hdIBImage()
{
}


hdReceiver * hdIBImage::CreateInterfaceElement()
{
    this->ResetAABB();
    if (!m_interfaceElement)
    {
        m_interfaceElement = new hdUIImage(m_texture, NULL, this->GetAABB());
    }
    return m_interfaceElement;
}


void hdIBImage::Draw() const
{
    if (m_interfaceElement)
        m_interfaceElement->Draw();
}
