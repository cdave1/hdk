/*
 *  hdIBViewController.cpp
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#include "hdIBViewController.h"
#include "hdINterface/hdInterfaceContext.h"

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