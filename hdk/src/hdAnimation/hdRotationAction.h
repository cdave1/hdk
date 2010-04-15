//
//  hdRotationAction.h
//  AnimationEngine
//
//  Created by david on 16/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#ifndef _HD_ROTATION_ACTION_H_
#define _HD_ROTATION_ACTION_H_

#include "hdAnimation/hdAction.h"

class hdRotationAction : public hdAction
{
public:
	hdRotationAction();
	
	~hdRotationAction() {}
	
	void Apply(hdTimeInterval elapsed, hdGameObject* gameObject);
	
	const hdVec3 GetRotation() const
	{
		return m_rotation;
	}
	
	void SetRotation(const hdVec3& rotation)
	{
		m_rotation = rotation;
	}
	
	
private:
	hdVec3 m_rotation;
	
};

#endif