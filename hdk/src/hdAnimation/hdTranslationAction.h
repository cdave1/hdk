//
//  hdTranslationAction.h
//  AnimationEngine
//
//  Created by david on 14/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#ifndef _HD_TRANSLATION_ACTION_H_
#define _HD_TRANSLATION_ACTION_H_

#include "hdAnimation/hdAction.h"

class hdTranslationAction : public hdAction
{
public:
	hdTranslationAction();
		
	~hdTranslationAction() {}
		
	void Apply(hdTimeInterval elapsed, hdGameObject* gameObject);
		
	const hdVec3 GetStart() const
	{
		return m_start;
	}
		
	void SetStart(const hdVec3& pos)
	{
		m_start = pos;
	}
	
	const hdVec3 GetDestination() const
	{
		return m_destination;
	}
	
	void SetDestination(const hdVec3& pos)
	{
		m_destination = pos;
	}
		
		
private:
	hdVec3 m_start;
	
	hdVec3 m_destination;
		
};

#endif
