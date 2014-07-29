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

/*
 * Mimics a thumb pad.
 *
 * Used to return a normalized vector indicating the difference between thumb's current position,
 * and center of bounding polygon.
 *
 * Use an octagon to indicate the bounds of the pad.  The pad icon is always at thumb's position;
 * on mouse up it snaps back to the origin.
 */

#ifndef HD_THUMB_PAD
#define HD_THUMB_PAD

#include "hdInterface/hdReceiver.h"
#include "hdInterface/hdFontManager.h"
#include "hdInterface/hdUIImage.h"

typedef enum
{
    e_padWaiting = 0,
    e_padMoving,
    e_padDisabled,
} e_padState;

class hdThumbPad : public hdReceiver
{
public:
    hdThumbPad() {}

    hdThumbPad(const char *bgTexture,
               const char *padTexture,
               const float x, const float y,
               const float w, const float h);

    ~hdThumbPad();

    void Draw() const;

    bool MouseDown(float x, float y);

    bool MouseOver(float x, float y);

    bool MouseUp(float x, float y);

    bool IsMoving();

    const hdVec2 GetPadDirection();


private:
    void Init();
    
    void DrawImpl();
    
    void MoveThumbPadTo(float x, float y);
    
    hdUIImage *m_pad;
    
    hdTexture *m_bgTexture;
    
    e_padState m_state;
    
    float m_radius;
};

#endif
