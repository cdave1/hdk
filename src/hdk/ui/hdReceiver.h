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

#ifndef _HD_RECEIVER_H_
#define _HD_RECEIVER_H_

#include <hdk/physics.h>
#include <hdk/graphics.h>

class hdReceiver : public hdPolygon, public hdDrawable
{
public:
    hdReceiver();

    hdReceiver(hdGameWorld* gameWorld);

    hdReceiver(hdGameWorld* gameWorld, bool addToContext);

    virtual ~hdReceiver() {}

    virtual bool MouseDown(float x, float y) = 0;

    virtual bool MouseOver(float x, float y) = 0;

    virtual bool MouseUp(float x, float y) = 0;

    virtual bool MouseDoubleClick(float x, float y);

    virtual void RotateAndTranslate(const hdVec3& rotVector, const hdVec3& transVector);

    virtual void RotateAndTranslate2(const hdVec3& rotVector, const hdVec3& rotationAxis,
                                     const hdVec3& transVector, const hdVec3& transVectorAxis);

    virtual void Draw() const = 0;

    const bool isEnabled();

    virtual void Disable();

    virtual void Enable();

    void SetCanRotate(const bool canRotate);

    const bool GetCanRotate() const;

    int TestFunc();

protected:
    bool m_isEnabled;

    bool m_canRotate;

private:
    void Init(bool addToContext);
};


inline const bool hdReceiver::isEnabled()
{
    return m_isEnabled;
}


inline void hdReceiver::Disable()
{
    m_isEnabled = false;
}


inline void hdReceiver::Enable()
{
    m_isEnabled = true;
}


inline void hdReceiver::SetCanRotate(const bool canRotate)
{
    m_canRotate = canRotate;
}


inline const bool hdReceiver::GetCanRotate() const
{
    return m_canRotate;
}

inline int hdReceiver::TestFunc()
{
    return 1;
}

#endif
