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

#include "hdReceiver.h"
#include "hdInterfaceContext.h"

hdReceiver::hdReceiver()
{
    m_isEnabled = true;
    m_canRotate = true;

    Init(false);
}

hdReceiver::hdReceiver(hdGameWorld* gameWorld) : hdPolygon(gameWorld)
{
    m_isEnabled = true;
    m_canRotate = true;

    Init(false);
}


hdReceiver::hdReceiver(hdGameWorld *gameWorld, bool addToContext) : hdPolygon(gameWorld)
{
    m_isEnabled = true;
    m_canRotate = true;
    Init(addToContext);
}


void hdReceiver::Init(bool addToContext)
{
    if (addToContext)
    {
        hdInterfaceContext::AddReceiver(this);
    }
}


void hdReceiver::RotateAndTranslate2(const hdVec3& rotVector, const hdVec3& rotationAxis,
                                     const hdVec3& transVector, const hdVec3& transVectorAxis)
{
    if (m_canRotate)
    {
        hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -rotationAxis);
        hdRotateVertices(this->GetVertices(), this->GetVertexCount(), rotVector - m_obb.transform.rotation);
        hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), rotationAxis);

        m_obb.transform.translation.Set(transVector.x, transVector.y, transVector.z);
        m_obb.transform.rotation.Set(rotVector.x, rotVector.y, rotVector.z);
    }
    else
    {
        hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), transVector - transVectorAxis);

        m_obb.transform.translation.Set(transVector.x, transVector.y, transVector.z);
    }
    this->ResetAABB();
}


// This function is broken - rotation needs to happen around some axis; if
// we assume this is the transVector, then it works okay, but if it isn't then
// the object will end up AT transvector.
void hdReceiver::RotateAndTranslate(const hdVec3& rotVector, const hdVec3& transVector)
{
    if (m_canRotate)
    {
        // rotate around transvector
        hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -m_obb.transform.translation);
        hdRotateVertices(this->GetVertices(), this->GetVertexCount(), rotVector - m_obb.transform.rotation);
        hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), m_obb.transform.translation + (transVector - m_obb.transform.translation));

        m_obb.transform.translation.Set(transVector.x, transVector.y, transVector.z);
        m_obb.transform.rotation.Set(rotVector.x, rotVector.y, rotVector.z);
    }
    else
    {
        hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -m_obb.transform.translation);
        hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), m_obb.transform.translation + (transVector - m_obb.transform.translation));
        
        m_obb.transform.translation.Set(transVector.x, transVector.y, transVector.z);		
    }
    this->ResetAABB();
}


bool hdReceiver::MouseDoubleClick(float x, float y)
{
    return false;
}
