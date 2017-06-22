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

#include "GoalBlock.h"

GoalBlock::GoalBlock(Block* parent) : Block()
{
    this->SetWorld(NULL);
    this->SetDepth(parent->GetDepth());
    this->SetTag(0);
    this->AddGameFlag((uint32)e_hdkGameFlagsBlockDrawRaw);

    m_parent = parent;

    this->SetAs2DBox(hdVec3(parent->GetAABB().lower.x, parent->GetAABB().upper.y, parent->GetAABB().lower.z),
                     hdVec3(parent->GetAABB().upper.x, parent->GetAABB().upper.y + 1.0f, parent->GetAABB().upper.z));

    this->InitInterface();
    this->InitAnimations();

    this->SetMaterial(e_hdkMaterialRawPolygon);
    this->SetBlockType(e_hdkBlockTypeCosmetic);

    m_emberEmitter = new WeatherParticleEmitter(16, "Materials/p_glow_15_16.tga", 0.25f, 2.0f, 3.25f, 0.0f, parent,
                                                     hdVec3(0, 0.75f * 3.25f, 0),
                                                     hdVec3(0, 1.0f * 3.25f, 0));

    m_emberEmitter->Start(this->GetAABB());
}


GoalBlock::~GoalBlock()
{
    delete m_emberEmitter;
}


void GoalBlock::InitInterface()
{
}


void GoalBlock::InitAnimations()
{
}


void GoalBlock::Step()
{
    m_emberEmitter->Step(0.0);
}


void GoalBlock::Draw() const
{
    GLboolean depthTest;
    hdAABB parentAABB;

    if (m_parent == NULL) return;

    parentAABB = m_parent->GetAABB();

    // White square along parent AABB...
    hdglBindTexture(NULL);

    // Get depth test value, because we're going to turn this off
    glGetBooleanv(GL_DEPTH_TEST, &depthTest);

    glEnable(GL_DEPTH_TEST);

    hdglBegin(GL_QUADS);
    hdglColor4f(1.0f, 1.0f, 0.75f, 1.0f);
    hdglVertex3f(parentAABB.lower.x+0.15f, parentAABB.upper.y+0.02f, -m_parent->GetDepth());
    hdglVertex3f(parentAABB.lower.x+0.15f, parentAABB.upper.y+0.02f, m_parent->GetDepth());
    hdglVertex3f(parentAABB.upper.x-0.15f, parentAABB.upper.y+0.02f, m_parent->GetDepth());
    hdglVertex3f(parentAABB.upper.x-0.15f, parentAABB.upper.y+0.02f, -m_parent->GetDepth());
    hdglEnd();

    if (depthTest == GL_FALSE)
    {
        glDisable(GL_DEPTH_TEST);
    }
}


void GoalBlock::DrawSpecial() const
{
    GLboolean blendEnabled, depthEnabled;
    GLint blendFuncSrc, blendFuncDst, texture2D;
    float depth;
    hdAABB parentAABB;

    if (m_parent == NULL) return;

    parentAABB = m_parent->GetAABB();
    depth = fabs(m_parent->GetDepth())-0.15f;

    glGetBooleanv(GL_BLEND, &blendEnabled);
    glGetBooleanv(GL_DEPTH_TEST, &depthEnabled);
    glGetIntegerv(GL_BLEND_SRC, &blendFuncSrc);
    glGetIntegerv(GL_BLEND_DST, &blendFuncDst);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    hdglBegin(GL_QUADS);
    hdglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    hdglVertex3f(parentAABB.lower.x+0.15f, parentAABB.upper.y, -depth);

    hdglColor4f(1.0f, 1.0f, 1.0f, 0.0f);
    hdglVertex3f(parentAABB.lower.x-1.0f, parentAABB.upper.y+3.0f, -(depth+1.0f));

    hdglColor4f(1.0f, 1.0f, 1.0f, 0.0f);
    hdglVertex3f(parentAABB.upper.x+1.0f, parentAABB.upper.y+3.0f, -(depth+1.0f));

    hdglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    hdglVertex3f(parentAABB.upper.x-0.15f, parentAABB.upper.y, -depth);
    hdglEnd();

    hdglBegin(GL_QUADS);
    hdglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    hdglVertex3f(parentAABB.lower.x+0.15f, parentAABB.upper.y, depth);

    hdglColor4f(1.0f, 1.0f, 1.0f, 0.0f);
    hdglVertex3f(parentAABB.lower.x-1.0f, parentAABB.upper.y+3.0f, (depth+1.0f));

    hdglColor4f(1.0f, 1.0f, 1.0f, 0.0f);
    hdglVertex3f(parentAABB.upper.x+1.0f, parentAABB.upper.y+3.0f, (depth+1.0f));
    
    hdglColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    hdglVertex3f(parentAABB.upper.x-0.15f, parentAABB.upper.y, depth);
    hdglEnd();
    
    if (depthEnabled == GL_FALSE)
    {
        glDisable(GL_DEPTH_TEST);
    }
    
    m_emberEmitter->Draw();
    
    if (blendEnabled == GL_FALSE)
    {
        glDisable(GL_BLEND);
        
    }
    
    glBlendFunc(blendFuncSrc, blendFuncDst);
}
