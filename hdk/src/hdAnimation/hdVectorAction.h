//
//  hdVectorAction.h
//  AnimationEngine
//
//  Created by david on 16/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#ifndef _HD_VECTOR_ACTION_H_
#define _HD_VECTOR_ACTION_H_

#include "hdAnimation/hdAction.h"

class hdVectorAction : public hdAction
{
public:
	hdVectorAction();
		
	~hdVectorAction() {}
		
	void Apply(hdTimeInterval elapsed, hdGameObject* gameObject);
	
	const hdVec3 GetDestination() const
	{
		return m_destination;
	}
		
	void SetDestination(const hdVec3& pos)
	{
		m_destination = pos;
	}
		
		
private:
	hdVec3 m_destination;
		
};

#endif
