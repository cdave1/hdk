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

#ifndef _HD_BUTTON_H_
#define _HD_BUTTON_H_

#include "hdInterface/hdReceiver.h"

typedef enum
{
	e_buttonWaiting = 0,
	e_buttonMouseDown,
	e_buttonMouseOver,
	e_buttonMouseOut,
	e_buttonMouseUp,
	e_buttonStateDisabled,
	e_buttonStateCount
} e_buttonState;


class hdButton : public hdReceiver
{
public:
	hdButton() {} 
	
	
	hdButton(std::string textureNormal, 
			 std::string textureOver, 
			 std::string textureUp);
	
	
	hdButton(const char* textureNormal, 
			 const char* textureOver, 
			 const char* textureUp, 
			 hdGameWorld* gameWorld);
	
	
	hdButton(const char* textureNormal, 
			 const char* textureOver, 
			 const char* textureUp, 
			 hdGameWorld* gameWorld, 
			 const hdAABB& aabb);
	
	hdButton(const char* textureNormal, 
			 const char* textureOver, 
			 const char* textureUp, 
			 hdGameWorld* gameWorld, 
			 const float x, const float y, 
			 const float w, const float h);
	
	~hdButton();
	
	bool MouseDown(float x, float y);
	
	bool MouseOver(float x, float y);
	
	bool MouseUp(float x, float y);
	
	bool MouseDoubleClick(float x, float y);
	
	hdReceiver* MouseUpFirstReceiver(float x, float y);
	
	void Draw() const;
	
	void ResetButton()
	{
		m_buttonState = e_buttonWaiting;
	}
	
	void SetMouseUpListener(void *obj, void (*func)(void *, void *));
	
	void SetMouseOverListener(void *obj, void (*func)(void *, void *));
	
	void SetMouseDownListener(void *obj, void (*func)(void *, void *));

private:
	
	void Init();
	
	void DoMouseDownCallback();
	
	void DoMouseOverCallback();
	
	void DoMouseUpCallback();
	
	e_buttonState m_buttonState;
	
	hdTexture* m_textureNormal;
	
	hdTexture* m_textureOver;
	
	hdTexture* m_textureUp;
	
	
	
	void (*m_mouseDownCallback)(void *, void *);
	
	void *m_mouseDownCallbackObject;
	
	void (*m_mouseOverCallback)(void *, void *);
	
	void *m_mouseOverCallbackObject;
	
	void (*m_mouseUpCallback)(void *, void *);
	
	void *m_mouseUpCallbackObject;
};




#endif