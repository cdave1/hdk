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

#ifndef _HD_INTERFACE_LAYER_H_
#define _HD_INTERFACE_LAYER_H_

#include <hdk/hdk.h>

// Defines a flat space that contains interface items.
//
// The layer is responsible for distributing events to children.

class hdInterfaceLayer : public hdDrawable, public hdGameObject
{
public:
    hdInterfaceLayer(hdGameWorld* gameWorld, const hdAABB& aabb);

    ~hdInterfaceLayer();

    int AddButton(hdButton* button);

    bool RemoveButton(hdButton* button);

    void Rotate(const hdVec3& rotationVector, const hdVec3& rotationAxis) {}

    void Scale(const hdVec3& scaleVector, const hdVec3& scaleAxis) {}

    void ResetOBB() {}

    hdVec3* GetVertices()
    {
        return m_obb.vertices;
    }

    const int GetVertexCount() const
    {
        return m_obb.GetVertexCount();
    }

    void ApplyTransform(const hdTransform& transform)
    {
        m_obb.transform = transform;
        ResetOBB();
    }

    const bool isEnabled()
    {
        return m_isEnabled;
    }

    void Disable()
    {
        m_isEnabled = false;
    }

    void Enable()
    {
        m_isEnabled = true;
    }


    // Check whether the touch event was in the bounds of any buttons.
    bool MouseDown(float x, float y);

    void MouseOver(float x, float y);

    hdButton* MouseUp(float x, float y);
    
    
    // Check whether the double click was in the bounds of any buttons.
    void HandleDoubleTouch(float x, float y);
    
    void Draw() const;
    
    
private:
    
    hdPointerList<hdButton, 64>* m_buttons;
    
    bool m_isEnabled;
};

#endif
