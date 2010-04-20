/*
 * hdReceiver means a potential receiver of user input
 *
 *
 */


#ifndef _HD_RECEIVER_H_
#define _HD_RECEIVER_H_

#include "hdGame.h"
#include "hdGraphicsLibrary.h"
#include "hdMath.h"


class hdReceiver : public hdPolygon, public hdDrawable
{
public:
	hdReceiver();
	
	hdReceiver(hdGameWorld* gameWorld);
	
	hdReceiver(hdGameWorld* gameWorld, bool addToContext);
	
	virtual ~hdReceiver() {}
	
	virtual bool MouseDown(float x, float y) = 0;

	virtual bool MouseOver(float x, float y) = 0;

	virtual bool MouseUp(float x, float y) = 0;
	
	virtual bool MouseDoubleClick(float x, float y);
	
	virtual void RotateAndTranslate(const hdVec3& rotVector, const hdVec3& transVector);
	
	virtual void RotateAndTranslate2(const hdVec3& rotVector, const hdVec3& rotationAxis,
									 const hdVec3& transVector, const hdVec3& transVectorAxis);
	
	virtual void Draw() const = 0;
	
	const bool isEnabled();
	
	virtual void Disable();
	
	virtual void Enable();
	
	void SetCanRotate(const bool canRotate);
	
	const bool GetCanRotate() const;
	
	int TestFunc();
	
protected:
	bool m_isEnabled;
	
	bool m_canRotate;

private:
	void Init(bool addToContext);
};


inline const bool hdReceiver::isEnabled()
{
	return m_isEnabled;
}


inline void hdReceiver::Disable()
{
	m_isEnabled = false;
}


inline void hdReceiver::Enable()
{
	m_isEnabled = true;
}


inline void hdReceiver::SetCanRotate(const bool canRotate)
{
	m_canRotate = canRotate;
}


inline const bool hdReceiver::GetCanRotate() const
{
	return m_canRotate;
}

inline int hdReceiver::TestFunc()
{
	return 1;
}


#endif