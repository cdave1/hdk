/*
 *  hdIBButton.cpp
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
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