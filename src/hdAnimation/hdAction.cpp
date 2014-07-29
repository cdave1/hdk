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

#include "hdAction.h"
#include <float.h>

hdAction::hdAction()
{
    m_duration = 0.0;
    m_timingFunction = hdAnimationTimingFunctionLinear;
    m_progress = 0.0;
    m_HACKHACKHACK_logTiming = false;
}


hdAction::~hdAction()
{
}


const bool hdAction::isFinished()
{
    return ((m_duration - m_progress) < 0.000001);
}


void hdAction::AddToProgress(const hdTimeInterval elapsed)
{
#if 0
    m_progress += (this->GetTimingFunctionDelta(elapsed) * elapsed);
    if (m_progress > m_duration) m_progress = m_duration;
#else
    m_progress += elapsed;
    if (m_progress > m_duration) m_progress = m_duration;
#endif
}


void hdAction::FinishWithoutInterpolation()
{
    if (m_progress < m_duration) m_progress = m_duration;
}


const hdTimeInterval hdAction::ClampElapsed(const hdTimeInterval elapsed)
{
    if (m_progress + elapsed > m_duration)
        return (m_duration - m_progress);
    else return elapsed;
}


const float hdAction::GetTimingFunctionDelta(const hdTimeInterval elapsed)
{
    if (m_duration == 0.0f) return 1.0f;

    if (m_timingFunction == hdAnimationTimingFunctionDecelerate)
    {
        float prev = hdClamp((m_progress - elapsed)/m_duration, 0.0f, 1.0f);
        float curr = hdClamp((m_progress)/m_duration, 0.0f, 1.0f);
        if (curr == 0.0f) return 1.0f;

        float s = sinf(acosf(1.0f - curr)) - sinf(acosf(1.0f - prev));
        return hdClamp(s/(elapsed/m_duration), 0.001f, 20.0f);
    }
    return 1.0f;
}


const float hdAction::GetTimingFunctionProportion(const hdTimeInterval elapsed)
{
    if (m_timingFunction == hdAnimationTimingFunctionDecelerate)
    {
        float prev = hdClamp((m_progress - elapsed)/m_duration, 0.0f, 1.0f);
        if (prev == 0.0f) return 1.0f;
        return sinf(acosf(1.0f - prev));
    }
    else return (m_progress - elapsed)/m_duration;
}


const float hdAction::GetTimingFunctionElapsedProportion()
{
    return this->GetTimingFunctionProportion(0.0f);
}



const hdTimeInterval hdAction::GetProgress()
{
    return m_progress;
}


void hdAction::SetProgress(const hdTimeInterval progress)
{
    m_progress = progress;
}


const hdTimeInterval hdAction::GetDuration()
{
    return m_duration;
}


void hdAction::SetDuration(const hdTimeInterval duration)
{
    m_duration = duration;
}


const hdAnimationTimingFunction_t hdAction::GetTimingFunction() const
{
    return m_timingFunction;
}


void hdAction::SetTimingFunction(const hdAnimationTimingFunction_t func)
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