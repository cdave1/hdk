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

#include "ChapterBlock.h"

ChapterBlock::ChapterBlock(totemBlock *parent, const char *chapterTitle, const bool isLocked, const float completionRatio) : totemBlock()
{
    this->SetWorld(NULL);
    this->SetDepth(parent->GetDepth());
    this->SetZOffset(parent->GetZOffset());
    this->SetTag(0);
    this->AddGameFlag((uint32)e_totemGameFlagsBlockDrawRaw);

    m_parent = parent;

    this->SetAs2DBox(hdVec3(parent->GetAABB().lower.x, parent->GetAABB().lower.y, parent->GetAABB().lower.z),
                     hdVec3(parent->GetAABB().upper.x, parent->GetAABB().upper.y, parent->GetAABB().upper.z));

    this->SetMaterial(e_totemMaterialRawPolygon);
    this->SetBlockType(e_totemBlockTypeCosmetic);

    snprintf(m_chapterTitle, 128, "%s", chapterTitle);

    m_isLocked = isLocked;
    m_completionRatio = completionRatio;

    this->InitInterface();
}


ChapterBlock::~ChapterBlock()
{
    delete m_chapterFont;
    delete m_lockImage;
}


void ChapterBlock::InitInterface()
{
    hdVec3 startingPos, lockImageCenter;
    float halfWidth;
    hdAABB parentAABB;

    startingPos = m_parent->GetWorldCenter();
    halfWidth = fabs(m_parent->GetDepth());
    parentAABB = m_parent->GetAABB();

    m_container = new hdUIContainer(m_world,
                                    hdVec3(parentAABB.lower.x, parentAABB.lower.y, 0.0f),
                                    hdVec3(parentAABB.Width(), parentAABB.Height(), 0.0f));

    m_chapterFont = new hdFontPolygon(CONFIG_CHAPTERBLOCK_FONT, "", NULL, parentAABB.lower.x + 0.05f, parentAABB.upper.y, 0.9f * parentAABB.Width(), 0.8f * parentAABB.Height());
    m_chapterFont->SetTint(0.0f, 0.0f, 0.0f, 1.0f);
    m_chapterFont->SetScale(0.019f);
    m_chapterFont->AlignCenter();
    m_chapterFont->SetText(m_chapterTitle);

    m_lockImage = new hdUIImage(Scripts_GetStringForKey("Menu_ChapterLockedImage").c_str(),
                                NULL,
                                startingPos.x - (0.5f), startingPos.y - (0.45f * parentAABB.Height()),
                                1.0f, 1.0f);
    m_lockImage->ResetOBB();
    m_lockImage->Hide();

    if (m_completionRatio >= 1.0f)
    {
        m_lockImage->Show();
    }

    m_container->Add(m_chapterFont);
    m_container->Add(m_lockImage);

    m_container->ResetAABB();
}


void ChapterBlock::Step()
{
    if (m_parent->IsPhysicsActor() == false) return;

    // Static bodies do not translate or rotate
    if (m_parent->IsStatic()) return;

    if (m_parent->GetPhysicsBody() == NULL) return;

    m_parent->Step();
    b2XForm xform = m_parent->GetPhysicsBody()->GetXForm();

    m_container->RotateAndTranslate(hdVec3(0.0f, 0.0f, m_parent->GetPhysicsBody()->GetAngle()),
                                    hdVec3(xform.position.x, xform.position.y, 0.0f));
    m_container->ResetAABB();
}


void ChapterBlock::Draw() const
{
    GLboolean blendEnabled, depthEnabled, depthMaskEnabled;
    GLint blendFuncSrc, blendFuncDst, texture2D;
    float depth;
    float zOffset;

    if (m_parent == NULL) return;

    zOffset = m_parent->GetZOffset();
    depth = fabs(m_parent->GetDepth());

    glGetBooleanv(GL_DEPTH_WRITEMASK, &depthMaskEnabled);
    glGetBooleanv(GL_BLEND, &blendEnabled);
    glGetBooleanv(GL_DEPTH_TEST, &depthEnabled);
    glGetIntegerv(GL_BLEND_SRC, &blendFuncSrc);
    glGetIntegerv(GL_BLEND_DST, &blendFuncDst);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_MODELVIEW);
    
    glDepthMask(GL_FALSE);
    
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, zOffset+abs(depth)+0.04f);
    m_container->Draw();
    glPopMatrix();
    
    if (depthMaskEnabled == GL_TRUE)
    {
        glDepthMask(GL_TRUE);
    }
    
    if (depthEnabled == GL_FALSE)
    {
        glDisable(GL_DEPTH_TEST);
    }
    if (blendEnabled == GL_FALSE)
    {
        glDisable(GL_BLEND);
    }

    glDisable(GL_TEXTURE_2D);
    
    glBlendFunc(blendFuncSrc, blendFuncDst);
}
