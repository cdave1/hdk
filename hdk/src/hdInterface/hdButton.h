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