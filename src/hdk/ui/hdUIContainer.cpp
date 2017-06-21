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

#include "hdUIContainer.h"

hdUIContainer::hdUIContainer(hdGameWorld* gameWorld) : hdReceiver(gameWorld)
{
    this->Init();
}


hdUIContainer::hdUIContainer(hdGameWorld* gameWorld, const hdVec3& lower, const hdVec3& upper) : hdReceiver(gameWorld)
{
    m_aabb.lower = lower;
    m_aabb.upper = upper;

    this->SetAs2DBox(hdVec3toVec2(lower), hdVec3toVec2(upper));

    this->ResetAABB();

    this->Init();
}


void hdUIContainer::Init()
{
    m_receivers = new hdTypedefList<hdReceiver*, 32>();
    m_isEnabled = true;
}


hdUIContainer::~hdUIContainer()
{
    delete m_receivers;
}


int hdUIContainer::Add(hdReceiver *receiver)
{
    return m_receivers->Add(receiver);
}


bool hdUIContainer::Remove(hdReceiver *receiver)
{
    // TODO: Fixme
    //return m_receivers->Remove(receiver);
    return true;
}


void hdUIContainer::Hide()
{
    for (int i = 0; i < m_receivers->GetItemCount(); ++i)
    {
        m_receivers->GetItems()[i]->Hide();
    }
    ((hdGameObject *)this)->Hide();
}


void hdUIContainer::Show()
{
    for (int i = 0; i < m_receivers->GetItemCount(); ++i)
    {
        m_receivers->GetItems()[i]->Show();
    }
    ((hdGameObject *)this)->Show();
}


void hdUIContainer::Enable()
{
    for (int i = 0; i < m_receivers->GetItemCount(); ++i)
    {
        m_receivers->GetItems()[i]->Enable();
    }
    ((hdReceiver *)this)->Enable();
}


void hdUIContainer::Disable()
{
    for (int i = 0; i < m_receivers->GetItemCount(); ++i)
    {
        m_receivers->GetItems()[i]->Disable();
    }
    ((hdReceiver *)this)->Disable();
}


void hdUIContainer::Translate(const hdVec3& translationVector, const hdVec3& translationAxis)
{
    for (int i = 0; i < m_receivers->GetItemCount(); ++i)
    {
        m_receivers->GetItems()[i]->Translate(translationVector, translationAxis);
    }

    hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), translationVector);
    this->GetTransform().translation = translationAxis + translationVector;
    this->ResetAABB();
}


void hdUIContainer::RotateAndTranslate(const hdVec3& rotVector, const hdVec3& transVector)
{
    for (int i = 0; i < m_receivers->GetItemCount(); ++i)
    {
        m_receivers->GetItems()[i]->RotateAndTranslate(rotVector, transVector);
    }

    hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -(this->GetWorldCenter()));
    hdRotateVertices(this->GetVertices(), this->GetVertexCount(), rotVector - m_obb.transform.rotation);
    hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), (this->GetWorldCenter()) + (transVector - (this->GetWorldCenter())));

    m_obb.transform.translation = transVector;
    m_obb.transform.rotation = rotVector;
    this->ResetAABB();
}


// Check whether the touch event was in the bounds of any buttons.
bool hdUIContainer::MouseDown(float x, float y)
{
    if (this->IsHidden()) return false;
    if (m_isEnabled == false) return false;

    // TODO: fixme
    //for (int i = 0; i < m_receivers->GetItemCount(); ++i)
    //{
    //	if (m_receivers->GetItems()[i]->MouseDown(x, y))
    //	{
    //		return true;
    //	}
    //}
    return false;
}


bool hdUIContainer::MouseOver(float x, float y)
{
    if (this->IsHidden()) return false;
    if (m_isEnabled == false) return false;

    // TODO: fixme
    //for (int i = 0; i < m_receivers->GetItemCount(); ++i)
    //{
    //	m_receivers->GetItems()[i]->MouseOver(x, y);
    //}
    return false;
}


bool hdUIContainer::MouseUp(float x, float y)
{
    if (this->IsHidden()) return false;
    if (m_isEnabled == false) return false;

    // TODO: fixme
    //for (int i = 0; i < m_receivers->GetItemCount(); ++i)
    //{
    //	if (m_receivers->GetItems()[i]->MouseUp(rOut, x, y))
    //	{
    //		break;
    //	}
    //}
    return false; //true;
}


void hdUIContainer::Draw() const
{
    if (this->IsHidden()) return;

    for (int i = 0; i < m_receivers->GetItemCount(); ++i)
    {
        m_receivers->GetItems()[i]->Draw();
    }
}
