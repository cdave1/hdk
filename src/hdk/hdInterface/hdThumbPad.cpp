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

#include "hdThumbPad.h"



hdThumbPad::hdThumbPad(const char *bgTexture,
                       const char *padTexture,
                       const float x, const float y,
                       const float w, const float h) : hdReceiver(NULL, true)
{
    m_bgTexture = hdTextureManager::Instance()->FindTexture(bgTexture, TT_16Pic);

    SetAs2DBox(hdVec2(x,y), hdVec2(x+w, y+h));

    hdVec3 tmp = GetWorldCenter();

    m_radius = w;

    m_pad = new hdUIImage(padTexture, NULL);
    m_pad->SetAs2DBox(tmp.x - m_radius/3.0f, tmp.y - m_radius/3.0f,
                      (m_radius/1.5f), (m_radius/1.5f));

    m_pad->SetCanRotate(false);
}


hdThumbPad::~hdThumbPad()
{
    delete m_pad;
}


bool hdThumbPad::IsMoving()
{
    return (m_state == e_padMoving);
}


bool hdThumbPad::MouseDown(float x, float y)
{
    if (!this->isEnabled())
    {
        m_state = e_padWaiting;
        return false;
    }

    if (this->IsHidden())
    {
        m_state = e_padWaiting;
        return false;
    }

    if (hdPolygonContainsPoint(GetVertices(), GetVertexCount(), hdVec3(x,y,0)))
    {
        if (m_state == e_padWaiting)
        {
            MoveThumbPadTo(x, y);

            m_state = e_padMoving;
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        m_state = e_padWaiting;
        return false;
    }
}


bool hdThumbPad::MouseOver(float x, float y)
{
    if (!this->isEnabled())
    {
        m_state = e_padWaiting;
        return false;
    }

    if (this->IsHidden())
    {
        m_state = e_padWaiting;
        return false;
    }

    // If we're moving it means
    // players started on the pad.
    if (m_state == e_padMoving)
    {
        MoveThumbPadTo(x, y);
        return true;
    }
    else
    {
        m_state = e_padWaiting;
        return false;
    }
}


bool hdThumbPad::MouseUp(float x, float y)
{
    if (!this->isEnabled())
    {
        m_state = e_padWaiting;
        return false;
    }

    if (this->IsHidden())
    {
        m_state = e_padWaiting;
        return false;
    }

    if (m_state == e_padMoving)
    {
        MoveThumbPadTo(this->GetWorldCenter().x,
                       this->GetWorldCenter().y);

        m_state = e_padWaiting;
        return true;
    }
    else
    {
        m_state = e_padWaiting;
        return false;
    }
}


const hdVec2 hdThumbPad::GetPadDirection()
{
    return hdVec2((m_pad->GetWorldCenter().x - this->GetWorldCenter().x)/m_radius,
                  (m_pad->GetWorldCenter().y - this->GetWorldCenter().y)/m_radius);
}


void hdThumbPad::MoveThumbPadTo(float x, float y)
{
    hdVec3 diff, tmp, norm;
    hdAABB aabb;

    diff.Set(x - this->GetWorldCenter().x,
             y - this->GetWorldCenter().y,
             0);

    aabb = ((hdThumbPad *)this)->GetAABB();

    if (hdFastHypotenuse(diff.x, diff.y) > (m_radius * 0.5f))
    {
        norm = hdNormalize3(diff);

        tmp.Set(this->GetWorldCenter().x + (norm.x * m_radius * 0.5f),
                this->GetWorldCenter().y + (norm.y * m_radius * 0.5f),
                0.0f);
    }
    else
    {
        tmp.Set(x, y, 0);
    }

    m_pad->SetAs2DBox(tmp.x - m_radius/3.0f, tmp.y - m_radius/3.0f,
                      (m_radius/1.5f), (m_radius/1.5f));
}


void hdThumbPad::Init()
{
}


void hdThumbPad::Draw() const
{
    ((hdThumbPad *)this)->DrawImpl();
}


void hdThumbPad::DrawImpl()
{
    if (IsHidden()) return;

    if (isEnabled())
    {
        if (m_bgTexture)
        {
            DrawVertices(GetVertices(), GetTextureCoordinates(), GetVertexCount(), m_aabb, m_bgTexture);
        }
        m_pad->Draw();
    }
    else
    {
        float tint[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
        DrawVerticesWithTint(GetVertices(),
                             GetTextureCoordinates(),
                             GetVertexCount(),
                             m_aabb,
                             m_bgTexture,
                             tint, 0.4f);
    }
}
