/*
 *  hdCheckbox.cpp
 *  TotemGame
 *
 *  Created by david on 20/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdCheckbox.h"

hdCheckbox::hdCheckbox(const char *textureOnNormal, 
					   const char *textureOnOver, 
					   const char *textureOffNormal,
					   const char *textureOffOver,
					   hdGameWorld* gameWorld)  : hdReceiver(gameWorld, true)
{
	// create two buttons
	
	m_onButton = new hdButton(textureOnNormal, 
							  textureOnOver, 
							  textureOnOver);
	
	//m_onButton->SetMouseUpListener(this, &hdCheckbox::CheckMouseUpCallback);
	
	m_offButton = new hdButton(textureOffNormal, 
							   textureOffOver, 
							   textureOffOver);
	
	//m_offButton->SetMouseUpListener(this, &hdCheckbox::CheckMouseUpCallback);
	
	m_activeButton = m_onButton;
	
	m_callbackObject = NULL;
	m_valueChangedCallback = NULL;
}


hdCheckbox::~hdCheckbox()
{
	if (m_onButton)
		delete m_onButton;
	
	if (m_offButton)
		delete m_offButton;
}


void hdCheckbox::SetAs2DBox(const float& x, const float& y, const float& w, const float& h)
{
	m_onButton->SetAs2DBox(x, y, w, h);
	m_offButton->SetAs2DBox(x, y, w, h);
}

void hdCheckbox::AddValueChangedListener(void *obj, void (*func)(void *, void *))
{
	m_callbackObject = obj;
	m_valueChangedCallback = func;
}



bool hdCheckbox::MouseDown(float x, float y)
{
	if (!this->isEnabled()) return false;
	if (this->IsHidden()) return false;
	
	return m_activeButton->MouseDown(x, y);
}


bool hdCheckbox::MouseOver(float x, float y)
{
	if (!this->isEnabled()) return false;
	if (this->IsHidden()) return false;
	
	return m_activeButton->MouseOver(x, y);
}


bool hdCheckbox::MouseUp(float x, float y)
{
	bool res;
	
	if (!this->isEnabled()) return false;
	if (this->IsHidden()) return false;
	
	res = m_activeButton->MouseUp(x, y);
	if (res)
	{
		Toggle();
		if (m_callbackObject != NULL && m_valueChangedCallback != NULL)
		{
			(*m_valueChangedCallback)(m_callbackObject, this); //, (void *)this);
		}
	}
	
	return res;
}

bool hdCheckbox::MouseDoubleClick(float x, float y)
{
	if (!this->isEnabled()) return false;
	if (this->IsHidden()) return false;
	
	return m_activeButton->MouseDoubleClick(x, y);
}


void hdCheckbox::Toggle()
{
	if (m_activeButton == m_onButton)
		m_activeButton = m_offButton;
	else
		m_activeButton = m_onButton;
}


void hdCheckbox::SetOn()
{
	hdAssert(m_onButton != NULL);
	m_activeButton = m_onButton;
}


void hdCheckbox::SetOff()
{
	hdAssert(m_offButton != NULL);
	m_activeButton = m_offButton;
}


void hdCheckbox::Draw() const
{
	m_activeButton->Draw();
}


bool hdCheckbox::IsOn() const
{
	return (m_activeButton == m_onButton);
}
