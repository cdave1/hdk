//
//  hdScaleAction.h
//  AnimationEngine
//
//  Created by david on 16/02/09.
//  Copyright 2009 n/a. All rights reserved.
//

#ifndef _HD_SCALE_ACTION_HD_
#define _HD_SCALE_ACTION_HD_

#include "hdAnimation/hdAction.h"

class hdScaleAction : public hdAction
{
public:
	hdScaleAction();
		
	~hdScaleAction() {}
		
	void Apply(hdTimeInterval elapsed, hdGameObject* gameObject);
		
	const hdVec3 GetScale() const
	{
		return m_scale;
	}
		
	void SetScale(const hdVec3& scale)
	{
		m_scale = scale;
	}
		
		
private:
	hdVec3 m_scale;
		
};

#endif