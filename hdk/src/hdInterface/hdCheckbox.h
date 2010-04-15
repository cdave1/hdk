/*
 *  hdCheckbox.h
 *  TotemGame
 *
 *  Created by david on 20/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_UI_CHECKBOX_H
#define HD_UI_CHECKBOX_H

#include "hdButton.h"

class hdCheckbox : public hdReceiver
	{
	public:
		hdCheckbox(const char *textureOnNormal, 
				 const char *textureOnOver, 
				   const char *textureOffNormal,
				   const char *textureOffOver,
				 hdGameWorld* gameWorld);
		
		~hdCheckbox();
		
		void SetAs2DBox(const float& x, const float& y, const float& w, const float& h);
		
		bool MouseDown(float x, float y);
		
		bool MouseOver(float x, float y);
		
		bool MouseUp(float x, float y);
		
		bool MouseDoubleClick(float x, float y);
		
		void Draw() const;
		
		void Disable();
		
		void Enable();
		
		void Toggle();
		
		bool IsOn() const;
		
		void SetOn();
		
		void SetOff();
		
		void AddValueChangedListener(void *obj, void (*func)(void *, void *));
	
	protected:
		//static void CheckMouseUpCallback(void *handler, void *sender);
		
	
	private:
		hdButton *m_onButton;
		
		hdButton *m_offButton;
		
		hdButton *m_activeButton;
		
		void (*m_valueChangedCallback)(void *, void *);
		
		void *m_callbackObject;
	};



inline void hdCheckbox::Disable()
{
	m_onButton->Disable();
	m_offButton->Disable();
	m_isEnabled = false;
}


inline void hdCheckbox::Enable()
{
	m_onButton->Enable();
	m_offButton->Enable();
	m_isEnabled = true;
}

#endif