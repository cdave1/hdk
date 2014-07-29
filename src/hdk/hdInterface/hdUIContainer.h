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

#ifndef _HD_INTERFACE_CONTAINER_H_
#define _HD_INTERFACE_CONTAINER_H_

#include <hdk/hdInterface/hdReceiver.h>
#include <hdk/hdCollections.h>

class hdUIContainer : public hdReceiver
{
public:
    hdUIContainer(hdGameWorld* gameWorld);

    hdUIContainer(hdGameWorld* gameWorld,  const hdVec3& lower, const hdVec3& upper);

    ~hdUIContainer();

    int Add(hdReceiver *receiver);

    bool Remove(hdReceiver *receiver);

    void Translate(const hdVec3& translationVector, const hdVec3& translationAxis);

    void RotateAndTranslate(const hdVec3& rotVector, const hdVec3& transVector);

    // Check whether the touch event was in the bounds of any buttons.
    bool MouseDown(float x, float y);

    bool MouseOver(float x, float y);

    bool MouseUp(float x, float y);

    void Draw() const;

    void Hide();

    void Show();

    void Enable();

    void Disable();
    
private:
    void Init();
    
    hdTypedefList<hdReceiver*, 32>* m_receivers;
    
    bool m_isEnabled;
    
    void *m_tag;
};

#endif
