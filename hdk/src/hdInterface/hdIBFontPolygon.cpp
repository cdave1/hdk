/*
 *  hdIBFontPolygon.cpp
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#include "hdIBFontPolygon.h"
#include "hdFontPolygon.h"

hdIBFontPolygon::hdIBFontPolygon() : hdIBProxy() 
{
	m_interfaceElement = NULL;
}


hdIBFontPolygon::hdIBFontPolygon(const char *fontFileName, 
							const hdVec3 &lower,
							const hdVec3 &upper) : hdIBProxy()
{
	this->SetFontFileName(fontFileName);
	this->SetAs2DBox(lower, upper);
	m_interfaceElement = NULL;
}


hdIBFontPolygon::~hdIBFontPolygon()
{
	delete m_interfaceElement;
}


hdReceiver * hdIBFontPolygon::CreateInterfaceElement()
{
	this->ResetAABB();
	if (!m_interfaceElement)
	{
		m_interfaceElement = new hdFontPolygon(m_fontFileName, "This is some placeholder text", NULL, this->GetAABB());
		((hdFontPolygon *)m_interfaceElement)->SetAs2DBox(this->GetAABB().lower, this->GetAABB().upper);
		((hdFontPolygon *)m_interfaceElement)->SetTint(0.0f, 0.0f, 0.0f, 1.0f);
		((hdFontPolygon *)m_interfaceElement)->AlignCenter();
	}
	return m_interfaceElement;
}


void hdIBFontPolygon::Draw() const
{
	if (m_interfaceElement)
		m_interfaceElement->Draw();
}