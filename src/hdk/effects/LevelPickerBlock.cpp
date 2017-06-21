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

#include "LevelPickerBlock.h"

LevelPickerBlock::LevelPickerBlock(hdGameWorld *gameWorld, const b2World *physicsWorld,
                                             TotemLevelStats* levelStats, totemLevel *level,
                                             const hdVec3& startingPos, float halfWidth,
                                             hdOrthographicProjection *projection) : totemBlock()
{
    this->SetWorld(gameWorld);
    this->SetDepth(-hdMin(1.0f, float(fabs(halfWidth/2.0f))));
    this->SetTag((int)e_totemTypeLevelPickerBlock);
    this->AddGameFlag((uint32)e_totemGameFlagsBlockDrawRaw);

    m_halfWidth = fabs(halfWidth);
    m_level = level;
    m_levelId = levelStats->levelNumber;
    m_levelStats = levelStats;

    m_messageFadeAnim = NULL;
    m_messageFadeIn = NULL;

    this->InitInterface();
    this->InitAnimations();

    hdVec2 aa(startingPos.x - m_halfWidth, startingPos.y - m_halfWidth);
    hdVec2 bb(startingPos.x + m_halfWidth, startingPos.y + m_halfWidth);

    this->SetAs2DBox(aa, bb);

    this->SetMaterial(e_totemMaterialRawPolygon);
    this->SetBlockType(e_totemBlockTypeDraggable);

    m_projection = projection;
}


LevelPickerBlock::~LevelPickerBlock()
{
    delete m_fontPolygon;
    delete m_lvlStatsPolygon;
    delete m_glow;
    delete m_messagePolygon;

    delete m_imgBronzeStar;
    delete m_imgSilverStar;
    delete m_imgGoldStar;
    delete m_levelInfoContainer;
}


void LevelPickerBlock::InitInterface()
{
    hdVec3 startingPos;
    startingPos = this->GetWorldCenter();

    SetTint(1.0f, 1.0f, 1.0f, 1.0f);

    char tmp[256];
    snprintf(tmp, 256, "Level %d:\n %s", m_levelStats->levelNumber, m_level->GetLevelName());

    m_fontPolygon = new hdFontPolygon(CONFIG_MED_FONT, "", m_world, startingPos.x, startingPos.y, m_halfWidth*1.8f, m_halfWidth*1.8f);
    m_fontPolygon->SetTint(0.0f, 0.0f, 0.0f, 0.75f);
    m_fontPolygon->SetRenderToTexture(false);
#ifdef IPHONE_BUILD
    m_fontPolygon->SetScale(0.0125f);
#else
    m_fontPolygon->SetScale(0.00975f);
#endif
    m_fontPolygon->AlignCenter();
    m_fontPolygon->SetTextFormatted("%s %d:\n %s",
                                    Scripts_GetStringForKey("Level").c_str(),
                                    m_levelStats->levelNumber,
                                    Scripts_GetStringForKey(m_level->GetLevelName()).c_str());

    if (m_levelStats != NULL)
    {
        snprintf(tmp, 256, "\nHigh Score:\n%d", m_levelStats->highestScore);
    }
    else
    {
        snprintf(tmp, 256, " ");
    }

    m_lvlStatsPolygon = new hdFontPolygon(CONFIG_MED_FONT, tmp, m_world, startingPos.x - (m_halfWidth * 0.9f), startingPos.y - (m_halfWidth * 0.7f),
                                          2.0f * (m_halfWidth * 0.9f), 0.0f);
    m_lvlStatsPolygon->SetTint(0.0f, 0.0f, 0.0f, 0.75f);
    m_lvlStatsPolygon->SetRenderToTexture(false);
    m_lvlStatsPolygon->SetScale(0.01f);
    m_lvlStatsPolygon->AlignCenter();
    m_lvlStatsPolygon->Hide();

    m_glow = new hdUIImage("/Materials/s_glow_15.tga", m_world,
                           startingPos.x - fabs(3.0f * m_halfWidth), startingPos.y - fabs(3.0f * m_halfWidth),
                           fabs(6.0f * m_halfWidth), fabs(6.0f * m_halfWidth));


    m_messagePolygon = new hdUIImage("/Interface/Textures/imgNewLevel.png", m_world,
                                     startingPos.x - (0.8f), startingPos.y + fabs(m_halfWidth) + 0.1f,
                                     1.6f, 1.1f);
    m_messagePolygon->SetCanRotate(false);


    m_imgBronzeStar = new hdUIImage("/Materials/s_stargoalbronze_15.tga", m_world,
                                    startingPos.x - (0.425f * m_halfWidth), startingPos.y - (0.85f * m_halfWidth),
                                    (m_halfWidth * 0.85f), (m_halfWidth * 0.85f));

    m_imgSilverStar = new hdUIImage("/Materials/s_stargoalsilver_15.tga", m_world,
                                    startingPos.x - (0.425f * m_halfWidth), startingPos.y - (0.85f * m_halfWidth),
                                    (m_halfWidth * 0.85f), (m_halfWidth * 0.85f));
    m_imgGoldStar = new hdUIImage("/Materials/s_stargoalgold_15.tga", m_world,
                                  startingPos.x - (0.425f * m_halfWidth), startingPos.y - (0.85f * m_halfWidth),
                                  (m_halfWidth * 0.85f), (m_halfWidth * 0.85f));

    // Force image resampling to 16bit mipmapped spr:
    m_messagePolygon->SetTexture(hdTextureManager::Instance()->ReloadTexture((hdTexture *)m_messagePolygon->GetTexture(), TT_Sprite));
    m_imgBronzeStar->SetTexture(hdTextureManager::Instance()->ReloadTexture((hdTexture *)m_imgBronzeStar->GetTexture(), TT_Sprite));
    m_imgSilverStar->SetTexture(hdTextureManager::Instance()->ReloadTexture((hdTexture *)m_imgSilverStar->GetTexture(), TT_Sprite));
    m_imgGoldStar->SetTexture(hdTextureManager::Instance()->ReloadTexture((hdTexture *)m_imgGoldStar->GetTexture(), TT_Sprite));


    m_levelInfoContainer = new hdUIContainer(m_world,
                                             hdVec3(startingPos.x - m_halfWidth, startingPos.y - m_halfWidth, 0.0f),
                                             hdVec3(startingPos.x + m_halfWidth, startingPos.y + m_halfWidth, 0.0f));

    m_levelInfoContainer->Add(m_fontPolygon);
    m_levelInfoContainer->Add(m_imgGoldStar);
    m_levelInfoContainer->Add(m_imgSilverStar);
    m_levelInfoContainer->Add(m_imgBronzeStar);
    m_levelInfoContainer->Add(m_glow);
    m_levelInfoContainer->Add(m_messagePolygon);

    this->UpdateInterface();
}


void LevelPickerBlock::InitAnimations()
{
    m_messageFadeAnim = hdAnimationController::CreateAnimation(this, false);
    m_messageFadeIn = new hdAlphaAction();

    m_messageFadeIn->SetDuration(0.75f);
    m_messageFadeIn->SetStartAlpha(0.0f);
    m_messageFadeIn->SetEndAlpha(1.0f);

    m_messageFadeAnim->AddGameObject(m_messagePolygon);
    m_messageFadeAnim->AddAction(m_messageFadeIn);
}


void LevelPickerBlock::UpdateInterface()
{
    m_imgBronzeStar->Hide();
    m_imgSilverStar->Hide();
    m_imgGoldStar->Hide();

    if (m_levelStats->highestCompletionPercentage >= 100 * _HDK_GAME_GOAL_MIN_PROPORTION)
    {
        m_imgBronzeStar->Show();
    }
    if (m_levelStats->highestCompletionPercentage >= 100 * _HDK_GAME_GOAL_MIDDLE_PROPORTION)
    {
        m_imgBronzeStar->Hide();
        m_imgSilverStar->Show();
    }
    if (m_levelStats->highestCompletionPercentage >= 100 * _HDK_GAME_GOAL_MAX_PROPORTION)
    {
        m_imgBronzeStar->Hide();
        m_imgSilverStar->Hide();
        m_imgGoldStar->Show();
    }
}


void LevelPickerBlock::AnimateNewLevelMessage()
{
    hdAssert(m_messageFadeAnim != NULL);

    m_messageFadeAnim->StartAnimation();
}


void LevelPickerBlock::Step()
{
    if (this->IsPhysicsActor() == false) return;

    /* Static bodies do not translate or rotate */
    if (this->IsStatic()) return;

    if (m_physicsBody == NULL) return;

    b2XForm xform = m_physicsBody->GetXForm();

    // Translate to origin, rotate, translate back again.
    hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), -m_obb.transform.translation);
    hdRotateVertices(this->GetVertices(), this->GetVertexCount(), hdVec3(0.0f, 0.0f, m_physicsBody->GetAngle()) - m_obb.transform.rotation);
    hdTranslateVertices(this->GetVertices(), this->GetVertexCount(), m_obb.transform.translation + (hdVec3(xform.position.x, xform.position.y, 0.0f) - m_obb.transform.translation));

    m_levelInfoContainer->RotateAndTranslate(hdVec3(0.0f, 0.0f, m_physicsBody->GetAngle()),
                                             hdVec3(xform.position.x, xform.position.y, 0.0f));

    m_obb.transform.translation = hdVec3(xform.position.x, xform.position.y, 0.0f);
    m_obb.transform.rotation =  hdVec3(0.0f, 0.0f, m_physicsBody->GetAngle());

    this->ResetAABB();
    m_levelInfoContainer->ResetAABB();


    if (m_levelStats != NULL)
    {
        m_levelStats->worldCenter.Set(xform.position.x, xform.position.y, 0.0f);

        if (m_levelStats->IsTouched() && m_messagePolygon->IsVisible() && m_glow->IsVisible())
        {
            m_glow->Hide();
            m_messagePolygon->Hide();
        }

        if (m_levelStats->IsCompleted())
        {
            m_lvlStatsPolygon->Show();
        }
    }
}


void LevelPickerBlock::Draw() const
{
    float depth;
    hdTexture *texture;
    float *tint;

    depth = ((LevelPickerBlock*)this)->GetDepth();
    texture = (hdTexture *)((LevelPickerBlock*)this)->GetTexture();
    tint = (float *)((LevelPickerBlock*)this)->GetTint();

    if (m_physicsBody == NULL) return;

    glEnable(GL_TEXTURE_2D);
    hdglBindTexture(NULL);
    hdglBegin(GL_QUADS); 
    ((totemBlock *)this)->DrawTrianglesFront();
    hdglEnd();
    
    ((totemBlock *)this)->DrawExtruded(texture, true, m_projection->GetWorldCenter());
    
    glEnable(GL_BLEND);
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 
                 abs(depth)+0.01f);
    m_imgBronzeStar->Draw();
    m_imgSilverStar->Draw();
    m_imgGoldStar->Draw();
    glPopMatrix();
    
    glDepthMask(GL_FALSE);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, 
                 abs(depth)+0.04f);
    m_fontPolygon->Draw();
    
    glPopMatrix();
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}


void LevelPickerBlock::DrawSpecial() const
{
    if (m_glow->IsVisible() && m_messagePolygon->IsVisible())
    {
        glEnable(GL_TEXTURE_2D);
        glEnable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -m_halfWidth);
        m_glow->Draw();
        glPopMatrix();
        
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, m_halfWidth+0.04f);
        m_messagePolygon->Draw();
        glPopMatrix();
        
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }
}
