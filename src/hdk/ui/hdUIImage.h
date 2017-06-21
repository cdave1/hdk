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

#ifndef _HD_UI_IMAGE_H_
#define _HD_UI_IMAGE_H_

#include <hdk/ui/hdReceiver.h>
#include <hdk/collections.h>


class hdUIImage : public hdReceiver
{
public:
    hdUIImage() {}

    hdUIImage(const char* texture,
              hdGameWorld* gameWorld);


    hdUIImage(const char* texture,
              hdGameWorld* gameWorld,
              const hdAABB& aabb);

    hdUIImage(const char* texture,
              hdGameWorld* gameWorld,
              const float x, const float y,
              const float w, const float h);

    ~hdUIImage();

    bool MouseDown(float x, float y);

    bool MouseOver(float x, float y);

    bool MouseUp(float x, float y);

    void Draw() const;

    const hdTexture* GetTexture() const;

    void SetTexture(const hdTexture *texture);

    void SetTint(float r, float g, float b, float a);

private:
    hdTexture* m_texture;

    float* m_tint;

};


inline const hdTexture* hdUIImage::GetTexture() const
{
    return m_texture;
}


inline void hdUIImage::SetTexture(const hdTexture *texture)
{
    m_texture = (hdTexture *)texture;
}


inline void hdUIImage::SetTint(float r, float g, float b, float a)
{
    m_tint[0] = r;
    m_tint[1] = g;
    m_tint[2] = b;
    m_tint[3] = a;
}

#endif
