/*
 *  hdIBProxy.cpp
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#include "hdIBProxy.h"

hdIBProxy::hdIBProxy() : hdPolygon(NULL)
{
	m_zOffset = 0.0f;
	bzero(m_tag, sizeof(m_tag));
	bzero(m_group, sizeof(m_group));
}


hdIBProxy::~hdIBProxy() 
{
}


hdInterfaceBuilderProxyType_t hdIBProxy::GetInterfaceBuilderProxyType() 
{ 
	return hdInterfaceBuilderProxyTypeNone; 
}


hdReceiver * hdIBProxy::CreateInterfaceElement()
{
	return NULL;
}


void hdIBProxy::Draw() const
{
}


void hdIBProxy::RotateDelta(const hdVec3& rotationVector, const hdVec3& rotationAxis)
{
	this->Rotate(rotationVector, rotationAxis);
	
	if (m_interfaceElement)
	{
		m_interfaceElement->Scale(rotationVector, rotationAxis);
	}
	this->ResetAABB();
	this->ResetOBB();
	this->SetStartingVerticesToCurrent();
}


void hdIBProxy::ScaleDelta(const hdVec3& scaleVector, const hdVec3& scaleAxis)
{
	this->Scale(scaleVector, scaleAxis);
	
	if (m_interfaceElement)
	{
		m_interfaceElement->Scale(scaleVector, scaleAxis);
	}
	this->ResetAABB();
	this->ResetOBB();
	this->SetStartingVerticesToCurrent();
}


void hdIBProxy::MoveDelta(const hdVec3 &delta)
{
	this->MoveTo(delta + this->GetWorldCenter());
	
	if (m_interfaceElement)
	{
		m_interfaceElement->MoveTo(delta + m_interfaceElement->GetWorldCenter());
	}
	this->ResetAABB();
	this->ResetOBB();
	this->SetStartingVerticesToCurrent();
}


void hdIBProxy::SetZOffset(const float offset)
{
	// All vertices have the same z offset
	for (int i = 0; i < this->GetVertexCount(); ++i)
	{
		this->GetVertices()[i].Set(this->GetVertices()[i].x, 
								   this->GetVertices()[i].y, 
								   offset);
		this->GetStartingVertices()[i].Set(this->GetStartingVertices()[i].x, 
										   this->GetStartingVertices()[i].y, 
										   offset);
	}
	
	//((hdPolygon *)this)->SetStartingVerticesToCurrent();
	this->ResetAABB();
	
	m_zOffset = offset;
	m_obb.transform.translation = hdVec3(m_obb.transform.translation.x, m_obb.transform.translation.y, m_zOffset);
}