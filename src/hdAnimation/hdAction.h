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

#ifndef _HD_ACTION_H_
#define _HD_ACTION_H_

#include <float.h>
#include "hdGame/hdGameObject.h"
#include "hdAnimation/hdAnimationTimingFunction.h"

class hdAction
{
public:
    hdAction();

    virtual ~hdAction();

    const bool isFinished();

    void AddToProgress(const hdTimeInterval elapsed);

    void FinishWithoutInterpolation();

    const hdTimeInterval ClampElapsed(const hdTimeInterval elapsed);

    virtual void Apply(hdTimeInterval elapsed, hdGameObject* gameObject) = 0;

    const hdTimeInterval GetProgress();

    void SetProgress(const hdTimeInterval progress);

    const hdTimeInterval GetDuration();

    void SetDuration(const hdTimeInterval duration);

    const hdAnimationTimingFunction_t GetTimingFunction() const;

    void SetTimingFunction(const hdAnimationTimingFunction_t func);

    bool m_HACKHACKHACK_logTiming;

    const float GetTimingFunctionDelta(const hdTimeInterval elapsed);

    const float GetTimingFunctionProportion(const hdTimeInterval elapsed);

    const float GetTimingFunctionElapsedProportion();

protected:
    hdAnimationTimingFunction_t m_timingFunction;

    hdTimeInterval m_duration;

    hdTimeInterval m_progress;
};


inline const hdTimeInterval hdAction::GetProgress()
{
    return m_progress;
}


inline void hdAction::SetProgress(const hdTimeInterval progress)
{
    m_progress = progress;
}


inline const hdTimeInterval hdAction::GetDuration()
{
    return m_duration;
}


inline void hdAction::SetDuration(const hdTimeInterval duration)
{
    m_duration = duration;
}


inline const hdAnimationTimingFunction_t hdAction::GetTimingFunction() const
{
    return m_timingFunction;
}


inline void hdAction::SetTimingFunction(const hdAnimationTimingFunction_t func)
{
    if (m_progress >= 0.0f && m_progress < FLT_EPSILON)
    {
        m_timingFunction = func;
    }
    else 
    {
        hdError(0, "Cannot change animation timing function while action is in progress.");
    }
}

#endif
