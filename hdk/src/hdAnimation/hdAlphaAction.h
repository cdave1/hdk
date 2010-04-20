/*
 *  hdAlphaAction.h
 *  TotemGame
 *
 *  Created by david on 23/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_ALPHA_ACTION_H_
#define _HD_ALPHA_ACTION_H_

#include "hdAnimation/hdAction.h"

class hdAlphaAction : public hdAction
{
public:
	hdAlphaAction();
	
	~hdAlphaAction();
	
	void Apply(hdTimeInterval elapsed, hdGameObject* gameObject);
	
	const float GetStartAlpha() const
	{
		return m_startAlpha;
	}
	
	void SetStartAlpha(const float& alpha)
	{
		m_startAlpha = alpha;
	}
	
	const float GetEndAlpha() const
	{
		return m_endAlpha;
	}
	
	void SetEndAlpha(const float& alpha)
	{
		m_endAlpha = alpha;
	}
	
	hdGameObject* GetAlphaObject() const;
	
	
private:
	hdGameObject* m_alphaObject;
	
	float m_startAlpha;
	
	float m_endAlpha;
	
};


inline hdGameObject* hdAlphaAction::GetAlphaObject() const
{
	return m_alphaObject;
}




#endif