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

#include "hdViewController.h"
#include <hdk/ui.h>

hdViewController::hdViewController(const char *hdIBViewControllerFilePath) : hdUIContainer(NULL)
{
    m_hdIBViewController = NULL;
    if ((m_hdIBViewController = hdIBLoader::Instance()->LoadViewControllerFromTextPakFile(hdIBViewControllerFilePath)))
    {
        InitInterface();
        InitSounds();
        m_width = m_hdIBViewController->GetWidth();
        m_height = m_hdIBViewController->GetHeight();

        this->SetAs2DBox(hdVec3(0,0,0), hdVec3(m_width, m_height, 0));
    }
    else
    {
        hdError(0, "Could not load hdViewController at %s\n",hdIBViewControllerFilePath);
    }
}


hdViewController::~hdViewController()
{
}


void hdViewController::SetCloseListener(void *obj, void (*func)(void *, void *))
{
    m_closePrefsCallbackObject = obj;
    m_closePrefsCallback = func;
}


void hdViewController::DoClosePrefsCallback()
{
    if (m_closePrefsCallbackObject != NULL && m_closePrefsCallback != NULL)
    {
        (*m_closePrefsCallback)(m_closePrefsCallbackObject, this);
    }
}


void hdViewController::InitInterface()
{
    if (!m_hdIBViewController) return;

    ((hdIBViewController *)m_hdIBViewController)->CreateInterfaceObjects();
    for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
    {
        hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
        this->Add((hdReceiver *)(proxy->GetInterfaceElement()));
    }
}


void hdViewController::SetMouseDownCallbackForTaggedItems(const char *tag, void *obj, void (*func)(void *, void *))
{
    for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
    {
        hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
        if (tag == NULL || strcmp(tag, proxy->GetTag()) == 0)
        {
            if (proxy->GetInterfaceBuilderProxyType() == hdInterfaceBuilderProxyTypeButton)
                ((hdButton *)(proxy->GetInterfaceElement()))->SetMouseDownListener(obj, func);
        }
    }
}


void hdViewController::SetMouseUpCallbackForTaggedItems(const char *tag, void *obj, void (*func)(void *, void *))
{
    for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
    {
        hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
        if (tag == NULL || strcmp(tag, proxy->GetTag()) == 0)
        {
            if (proxy->GetInterfaceBuilderProxyType() == hdInterfaceBuilderProxyTypeButton)
                ((hdButton *)(proxy->GetInterfaceElement()))->SetMouseUpListener(obj, func);
        }
    }
}


const hdReceiver * hdViewController::GetInterfaceItemWithTag(const char *tag)
{
    for (unsigned i = 0; i < ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItemCount(); ++i)
    {
        hdIBProxy *proxy = ((hdIBViewController *)m_hdIBViewController)->GetProxyList()->GetItems()[i];
        if (tag && strcmp(tag, proxy->GetTag()) == 0)
        {
            return proxy->GetInterfaceElement();
        }
    }
    return NULL;
}


void hdViewController::InitSounds() {}


void hdViewController::RefreshLayout(const e_hdInterfaceOrientation &orientation) {}


void hdViewController::WillDraw() const
{
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();

    hdglOrthof(0.0f, m_width,
               0.0f, m_height,
               10.0f, -10.0f);
}


void hdViewController::Draw()
{
    this->WillDraw();
    glEnable(GL_TEXTURE_2D);
    ((hdUIContainer *)this)->Draw();
    glDisable(GL_TEXTURE_2D);
    this->DidDraw();
}


void hdViewController::DidDraw() const
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
}
