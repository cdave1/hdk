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

#include "hdButton.h"

hdButton::hdButton(std::string textureNormal, 
				   std::string textureOver, 
				   std::string textureUp) : hdReceiver(NULL, true)
{
	m_textureNormal = hdTextureManager::Instance()->FindTexture(textureNormal.c_str(), TT_16Pic);
	m_textureOver = hdTextureManager::Instance()->FindTexture(textureOver.c_str(), TT_16Pic);
	m_textureUp = hdTextureManager::Instance()->FindTexture(textureUp.c_str(), TT_16Pic);
	
	
	
	this->SetAs2DBox(hdVec3(0,0,0), hdVec3(1.0f, 1.0f, 1.0f));
	
	this->Init();
}


hdButton::hdButton(const char* textureNormal, 
				   const char* textureOver, 
				   const char* textureUp, 
				   hdGameWorld* gameWorld) : hdReceiver(gameWorld, true)
{
	m_textureNormal = hdTextureManager::Instance()->FindTexture(textureNormal, TT_16Pic);
	m_textureOver = hdTextureManager::Instance()->FindTexture(textureOver, TT_16Pic);
	m_textureUp = hdTextureManager::Instance()->FindTexture(textureUp, TT_16Pic);
	
	this->SetAs2DBox(hdVec3(0,0,0), hdVec3(1.0f, 1.0f, 1.0f));
	
	this->Init();
}


hdButton::hdButton(const char* textureNormal, 
					 const char* textureOver, 
					 const char* textureUp, 
					 hdGameWorld* gameWorld, 
					 const hdAABB& aabb) : hdReceiver(gameWorld, true)
{
	m_textureNormal = hdTextureManager::Instance()->FindTexture(textureNormal, TT_16Pic);
	m_textureOver = hdTextureManager::Instance()->FindTexture(textureOver, TT_16Pic);
	m_textureUp = hdTextureManager::Instance()->FindTexture(textureUp, TT_16Pic);
	
	this->SetAs2DBox(aabb.lower, aabb.upper);
	
	this->Init();
}


hdButton::hdButton(const char* textureNormal, 
		 const char* textureOver, 
		 const char* textureUp, 
		 hdGameWorld* gameWorld, 
		 const float x, const float y, 
		 const float w, const float h) : hdReceiver(gameWorld, true)
{
	m_textureNormal = hdTextureManager::Instance()->FindTexture(textureNormal, TT_16Pic);
	m_textureOver = hdTextureManager::Instance()->FindTexture(textureOver, TT_16Pic);
	m_textureUp = hdTextureManager::Instance()->FindTexture(textureUp, TT_16Pic);
	
	this->SetAs2DBox(hdVec3(x,y,0.0), hdVec3(x+w,y+h,0.0));
	
	this->Init();
}


void hdButton::Init()
{
	m_buttonState = e_buttonWaiting;
	
	m_mouseDownCallbackObject = NULL;
	m_mouseDownCallback = NULL;

	m_mouseOverCallbackObject = NULL;
	m_mouseOverCallback = NULL;

	m_mouseUpCallbackObject = NULL;
	m_mouseUpCallback = NULL;
}


hdButton::~hdButton() 
{
}


void hdButton::SetMouseDownListener(void *obj, void (*func)(void *, void *))
{
	m_mouseDownCallbackObject = obj;
	m_mouseDownCallback = func;
}


void hdButton::SetMouseOverListener(void *obj, void (*func)(void *, void *))
{
	m_mouseOverCallbackObject = obj;
	m_mouseOverCallback = func;
}


void hdButton::SetMouseUpListener(void *obj, void (*func)(void *, void *))
{
	m_mouseUpCallbackObject = obj;
	m_mouseUpCallback = func;
}


void hdButton::DoMouseDownCallback()
{
	if (m_mouseDownCallbackObject != NULL && m_mouseDownCallback != NULL)
	{
		(*m_mouseDownCallback)(m_mouseDownCallbackObject, this);
	}
}


void hdButton::DoMouseOverCallback()
{
	if (m_mouseOverCallbackObject != NULL && m_mouseOverCallback != NULL)
	{
		(*m_mouseOverCallback)(m_mouseOverCallbackObject, this);
	}
}


void hdButton::DoMouseUpCallback()
{
	if (m_mouseUpCallbackObject != NULL && m_mouseUpCallback != NULL)
	{
		(*m_mouseUpCallback)(m_mouseUpCallbackObject, this);
	}
}


bool hdButton::MouseDown(float x, float y)
{
	if (!this->isEnabled()) 
	{
		m_buttonState = e_buttonWaiting;
		return false;
	}
	
	if (this->IsHidden())
	{
		m_buttonState = e_buttonWaiting;
		return false;
	}
	
	if (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)))
	{
		if (m_buttonState == e_buttonWaiting)
        {
			m_buttonState = e_buttonMouseDown;
			DoMouseDownCallback();
			return true;
		}
		else
		{
			return false;
		}
	} 
	else 
	{
		m_buttonState = e_buttonWaiting;
		return false;
	}
}


// If we did not click on the item, then state can not go into mouse over.
bool hdButton::MouseOver(float x, float y)
{
	if (!this->isEnabled()) 
	{
		m_buttonState = e_buttonWaiting;
		return false;
	}
	
	if (this->IsHidden())
	{
		m_buttonState = e_buttonWaiting;
		return false;
	}
	
	if (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)))
	{
		if (m_buttonState == e_buttonMouseDown)
		{
			m_buttonState = e_buttonMouseOver;
			DoMouseOverCallback();
			return true;
		}
		else if (m_buttonState == e_buttonMouseOver)
		{
			DoMouseOverCallback();
			return true;
		}
		else
		{
			return false;
		}
	} 
	else 
	{
		if (m_buttonState == e_buttonMouseDown || m_buttonState == e_buttonMouseOver)
		{
			m_buttonState = e_buttonMouseOut;
		} 
		else
		{
			m_buttonState = e_buttonWaiting;
		}
		return false;
	}
}


bool hdButton::MouseUp(float x, float y)
{
	if (!this->isEnabled()) 
	{
		m_buttonState = e_buttonWaiting;
		return false;
	}
	
	if (this->IsHidden())
	{
		m_buttonState = e_buttonWaiting;
		return false;
	}
	
	if (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)))
	{	
		// if state must be in mouseOver for anything to happen:
		if (m_buttonState == e_buttonMouseOver || m_buttonState == e_buttonMouseDown)
		{
			m_buttonState = e_buttonMouseUp;
			DoMouseUpCallback();
			return true;
		}
		return false;
	} 
	else 
	{ 
		if (m_buttonState == e_buttonMouseOver || m_buttonState == e_buttonMouseDown)
		{
			// The mouse was moved off the button and released, so reset the state.
			m_buttonState = e_buttonWaiting;
		}
		return false;
	}
}


bool hdButton::MouseDoubleClick(float x, float y)
{
	if (hdPolygonContainsPoint(((hdPolygon*)this)->GetVertices(), 4, hdVec3(x,y,0)))
	{	
		// if state must be in mouseOver for anything to happen:
		if (m_buttonState == e_buttonMouseUp)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	return false;
}


void hdButton::Draw() const
{
	if (this->IsHidden()) return;
	
	if (((hdButton*)this)->isEnabled())
	{
		switch (m_buttonState) {
			case e_buttonMouseDown:
			case e_buttonMouseOver:
				if (m_textureOver)
				{
					DrawVertices(((hdPolygon*)this)->GetVertices(), ((hdPolygon*)this)->GetTextureCoordinates(), 4, m_aabb, m_textureOver);
				}
				break;
			case e_buttonMouseUp:
				if (m_textureUp)
				{
					DrawVertices(((hdPolygon*)this)->GetVertices(), ((hdPolygon*)this)->GetTextureCoordinates(), 4, m_aabb, m_textureUp);
				}
				break;
			case e_buttonWaiting:
			default:
				if (m_textureNormal)
				{
					DrawVertices(((hdPolygon*)this)->GetVertices(), ((hdPolygon*)this)->GetTextureCoordinates(), 4, m_aabb, m_textureNormal);
				}
				break;
		}
	}
	else
	{
		const float tint[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
		DrawVerticesWithTint(((hdPolygon*)this)->GetVertices(), 
							 ((hdPolygon*)this)->GetTextureCoordinates(), 
							 4, 
							 m_aabb, 
							 m_textureNormal, 
							 tint, 0.4f);
	}
	
	// HACK HACK HACK: set state back to normal when state is up.
	if (m_buttonState == e_buttonMouseUp)
	{
		((hdButton*)this)->ResetButton();
	}
}
