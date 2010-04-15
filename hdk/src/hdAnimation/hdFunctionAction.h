/*
 *  hdFunctionAction.h
 *  hdGameEngine
 *
 *  Created by david on 13/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_FUNCTION_ACTION_H_
#define _HD_FUNCTION_ACTION_H_

#include "hdAnimation/hdAction.h"

// An animation action that repeatedly calls a function on an object.
template<class T>
class hdFunctionAction : public hdAction
	{
	public:
		hdFunctionAction()
		{
			m_func = NULL;
			m_obj = NULL;
		}
		
		~hdFunctionAction() {}
		
		void Apply(hdTimeInterval elapsed, hdGameObject* gameObject);
		
		void SetFunction(void (T::*func)(hdTimeInterval));
		
		void SetFunctionObject(const T* obj);
		
		
	private:
		void (T::*m_func)(hdTimeInterval);
		T* m_obj;
	};


template<class T>
inline void hdFunctionAction<T>::SetFunction(void (T::*func)(hdTimeInterval))
{
	m_func = func;
}


template<class T>
inline void hdFunctionAction<T>::SetFunctionObject(const T* obj)
{
	hdAssert((void*)obj != (void*)this);
	m_obj = (T*)obj;
}


template<class T>
void hdFunctionAction<T>::Apply(hdTimeInterval elapsed, hdGameObject* gameObject)
{
	if (m_obj == NULL) {
		hdPrintf("Bad hdFunctionAction object.");
		return;
	}
	
	(m_obj->*m_func)(elapsed);
	
#ifdef ANIMATION_DEBUG
	hdPrintf("hdFunctionAction->Apply: Name: %d, Progress: %f, Elapsed: %f, Called Function: %d\n", gameObject->GetName(), m_progress, elapsed, m_func);
#endif
}

#endif