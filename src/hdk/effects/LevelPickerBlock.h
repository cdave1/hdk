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

#ifndef _HDK_LEVEL_PICKER_BLOCK_
#define _HDK_LEVEL_PICKER_BLOCK_

#include "Box2D.h"
#include <hdk/game.h>
#include "TowerPlayerStateClasses.h"

class LevelPickerBlock : public Block
{
public:
    LevelPickerBlock(hdGameWorld *gameWorld, const b2World *physicsWorld,
                          TotemLevelStats* levelStats, Level *level,
                          const hdVec3& startingPos, float halfWidth,
                          hdOrthographicProjection *projection);

    ~LevelPickerBlock();

    void Step();

    void Draw() const;

    void DrawInfo() const;

    void DrawSpecial() const;

    void DrawFonts() const;

    void AnimateNewLevelMessage();

    void UpdateInterface();

    Level *m_level;

    TotemLevelStats *m_levelStats;

    int m_levelId;

    float m_halfWidth;

    hdUIContainer *m_levelInfoContainer;

    hdFontPolygon *m_fontPolygon;


private:

    void InitInterface();

    void InitAnimations();

    hdOrthographicProjection *m_projection;

    hdAnimation *m_messageFadeAnim;
    hdAlphaAction *m_messageFadeIn;

    hdAnimation *m_messageBounceAnim;
    hdVectorAction *m_messageBounceAction;
    
    hdUIImage *m_glow;
    hdUIImage *m_messagePolygon;
    
    hdUIImage *m_imgBronzeStar;
    hdUIImage *m_imgSilverStar;	
    hdUIImage *m_imgGoldStar;
    
    hdUIImage *m_imgBarWhite;
    hdFontPolygon *m_lvlStatsPolygon;

};

#endif
