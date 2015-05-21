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

#include "hdIBViewController.h"
#include <hdk/hdInterface/hdInterfaceContext.h>

hdIBViewController::hdIBViewController(const char *name)
{
    this->SetName(name);
    this->SetDimensions(480.0f, 320.0f);
    m_proxies = new hdSerializablePointerList<hdIBProxy, MAX_VIEW_CONTROLLER_PROXIES>();
}


hdIBViewController::hdIBViewController(const char *name, const float width, const float height)
{
    this->SetName(name);
    this->SetDimensions(width, height);
    m_proxies = new hdSerializablePointerList<hdIBProxy, MAX_VIEW_CONTROLLER_PROXIES>();
}


hdIBViewController::~hdIBViewController()
{
    delete m_proxies;
}


void hdIBViewController::CreateInterfaceObjects()
{
    for (int i = 0; i < m_proxies->GetItemCount(); ++i)
    {
        if (m_proxies->GetItems()[i])
        {
            hdInterfaceContext::AddReceiver(m_proxies->GetItems()[i]->CreateInterfaceElement());
        }
    }
}


const int hdIBViewController::AABBQuery(const hdAABB& aabb, hdIBProxy ** objects, int maxObjects)
{
    int count = 0;

    hdIBProxy * object = NULL;

    for (int i = 0; i < m_proxies->GetItemCount(); ++i)
    {
        if (m_proxies->GetItems()[i])
        {
            object = m_proxies->GetItems()[i];
            if (object->AABBIntersection(aabb))
            {
                objects[(count++)%maxObjects] = object;
            }
        }
    }
    return count;
}


void hdIBViewController::Draw() const
{
    for (int i = 0; i < m_proxies->GetItemCount(); ++i)
    {
        if (m_proxies->GetItems()[i])
        {
            m_proxies->GetItems()[i]->Draw();
        }
    }
}