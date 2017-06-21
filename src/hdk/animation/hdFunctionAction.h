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

#ifndef _HD_FUNCTION_ACTION_H_
#define _HD_FUNCTION_ACTION_H_

#include <hdk/animation/hdAction.h>

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
