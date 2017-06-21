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

#ifndef _HD_PAUSE_ACTION_H_
#define _HD_PAUSE_ACTION_H_

#include <hdk/hdAnimation/hdAction.h>

class hdPauseAction : public hdAction
{
public:
    hdPauseAction() {}

    ~hdPauseAction() {}

    void Apply(hdTimeInterval elapsed, hdGameObject* gameObject);

};

inline void hdPauseAction::Apply(hdTimeInterval elapsed, hdGameObject* gameObject)
{
#ifdef ANIMATION_DEBUG
    hdPrintf("hdPauseAction->Apply: Name: %d, Progress: %f, Elapsed: %f\n", gameObject->GetName(), m_progress, elapsed, m_func);
#endif			
}

#endif