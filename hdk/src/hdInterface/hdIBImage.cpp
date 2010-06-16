/*
 *  HDIBImage.cpp
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
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