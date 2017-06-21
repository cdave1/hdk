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
 *
 * Controller for:
 * - Levels - which one player is on.
 * - Currently selected world.
 * - Level picker menu.
 */

#ifndef _HDK_LEVEL_CONTROLLER_H_
#define _HDK_LEVEL_CONTROLLER_H_

#include "Box2D.h"

#include <hdk/game.h>

#include "totemEffects.h"
#include "SmashedUtils.h"
#include "TowerPlayerStateClasses.h"
#include "b2BuoyancyController.h"

#define LEVELPICKER_NEW_LEVEL_MESSAGE_TAG 6001
#define LEVELPICKER_LEVEL_PREV_TAG 500

#define LEVEL_CONTROLLER_TITLE_FONT "Fonts/bebas18.f"
#define LEVEL_CONTROLLER_MESSAGE_FONT "Fonts/collegiate16.f"
#define LEVEL_CONTROLLER_TINY_FONT "Fonts/bebas10.f"

#define kGravityMultiplier 10.0f

class TotemLevelController : public hdInterfaceController, public totemContactManager
{
public:
    TotemLevelController(const hdInterfaceController* parentController);

    ~TotemLevelController();

    void Step(double interval);


    void HandleTapUp(float x, float y, int tapCount);

    void HandleTapDown(float x, float y, int tapCount);

    void HandleSingleTap(const float x, const float y);

    void HandleDoubleTap(float x, float y);

    void HandleTapMovedSingle(const float previousX, const float previousY,
                              const float currentX, const float currentY);

    void HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
                              const float aCurrentX, const float aCurrentY,
                              const float bPreviousX, const float bPreviousY,
                              const float bCurrentX, const float bCurrentY);

    void HandleOrientationVectorUpdate(const float x, const float y, const float z) {}

    void HandleOrientationChanged();

    const bool InitLevel(const totemLevel* level, const int levelId);

    const totemLevel* GetCurrentLevel() const;

    void ResetCurrentLevel();

    void RestartCurrentLevel();


protected:
    void DrawInternal();

    void PushProjectionMatrix();

    void PopProjectionMatrix();

    void DrawInternalInterface();

    const totemBlock* GetSelectedTotemBlock() const;

    void SetSelectedTotemBlock(const totemBlock *block);

    void DrawSelectedBlockHighlight();

    const b2World* GetPhysicsWorld() const;

    bool AddBlockToCurrentLevel(totemBlock *block);

    totemBlock* FindBlockWithTag(const int tag) const;

    void RefreshLayout();

    const hdSound* GetCollisionSoundForMaterial(const e_totemMaterial material) const;


    /* Messaging Functions */
    void LoadScriptMessageForTag(const int tag);

    void LoadScriptMessageForTag(const int tag, const bool addToQueue);

    void LoadScriptMessageForTagAndLevel(const int tag, const int levelId, const bool addToQueue);

    void LoadScriptMessageForTagAndLevel(const int tag, const int levelId, const bool addToQueue, const bool noShow);

    void SetMessage(const totemMessage *msg);

    void SetMessage(const hdTexture *texture);

    void ShowMessage();

    void HideMessage();

    const bool IsShowingMessage() const;


    virtual const bool Game_InitLevel(const totemLevel *level, const int levelId);

    virtual void Game_Step(double interval);

    virtual void Game_ResetCurrentLevel();

    virtual const bool Game_CanSelectBlock(const totemBlock *block);

    virtual void Game_BlockWasSelected(const totemBlock *block) const;

    virtual const bool Game_HandleTapUp(float x, float y, int tapCount);

    virtual const bool Game_HandleTapDown(float x, float y, int tapCount);

    virtual const bool Game_HandleSingleTap(float x, float y);

    virtual const bool Game_HandleDoubleTap(float x, float y);

    virtual const bool Game_HandleTapMovedSingle(const float previousX, const float previousY,
                                                 const float currentX, const float currentY);

    virtual const bool Game_HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
                                                 const float aCurrentX, const float aCurrentY,
                                                 const float bPreviousX, const float bPreviousY,
                                                 const float bCurrentX, const float bCurrentY);

    virtual const bool Game_HandleBlockTap(totemBlock *block, const hdVec3& impactPoint, bool hazardCollision);

    virtual const bool Game_HandleParticleTap(hdPhysicsParticle* particle, const hdVec3& impactPoint, bool hazardCollision);

    virtual const bool Game_HandleBlockLoss(totemBlock* block, const hdVec3& contactPoint);

    virtual void Game_HandleCollision(const ContactPoint* point);

    virtual void Game_ShowMessage();

    virtual void Game_HideMessage();

    virtual void Game_PlayLoudCollisionSound(const totemBlock* block, const b2Body* body);

    virtual void Game_PlaySoftCollisionSound(const totemBlock* block, const b2Body* body);

    virtual void Game_AddBlockToLevel(const totemBlock* block);

    virtual void Game_HandleOrientationChanged();

    virtual void Game_HandleGoalCollision(totemBlock *goalBlock, totemBlock *block, const hdVec3& impactPoint);

    char m_currentWorldName[128];

    hdPhysicsParticleEmitter* m_physicsParticleEmitter;

private:

    void InitPhysics();

    void InitLevel(totemLevel *level);

    void InitEffects();

    void InitAnimations();

    void InitSounds();

    void InitInterface();


    void PhysicsStep(double interval);

    void PanToSelectedObject(double interval);

    void HandleCollision(const ContactPoint* point);

    void HandleParticleBlockCollision(hdPhysicsParticle *particle, totemBlock *block, const ContactPoint *point);

    void HandleBlockBlockCollision(totemBlock *block1, totemBlock *block2, const ContactPoint *point);

    void HandleFragmentSplash(hdPhysicsParticle *fragment, const ContactPoint *point);

    bool DoParticleTapDown(float screenX, float screenY);

    /*
     * Block under mouse
     */
    bool SelectBlockAtScreenPoint(float x, float y);

    bool SelectBlockAtScreenPoint(e_hdInterfaceClickState clickState, float x, float y);

    hdVec3 m_distanceFromScreenCenter;

    /*
     * Mouse Joints
     */
    void CreateMouseJoint();

    void UpdateMouseJoint(float x, float y);

    void DestroyMouseJoint();


    // Game related stuff:
    void HandleBlockLoss(const hdVec3& contactPoint, totemBlock* block);

    void HandleBlockWorldExit(const hdVec3& contactPoint, totemBlock* block);

    void HandleSenderBlockTapUpEvent(const totemBlock* sender, totemEvent* ev);

    void HandleSenderBlockTapDownEvent(const totemBlock* sender, totemEvent* ev);

    void HandleGameAreaTapUp(float x, float y);

    void HandleBlockTap(totemBlock* block, const hdVec3& impactPoint, bool hazardCollision);

    void HandleParticleTap(hdPhysicsParticle* particle, const hdVec3& impactPoint, bool hazardCollision);

    void PlayLoudCollisionSound(const totemBlock* block, const b2Body* body);

    void PlaySoftCollisionSound(const totemBlock* block, const b2Body* body);


    bool m_pushedProjectionMatrix;


    /* Messages */
    /*
     * Note: two versions of each, one for portrait mode, the other
     * for landscape mode.
     *
     * The final set of pointers are just pointers
     */
    totemMessage* m_scriptMessages[10];

    int m_messageCount;

    int m_currentMessageIndex;

    hdTypedefList<totemMessage, 16> *m_messageTagQueue;

    const bool ShowNextScriptMessage();

    static void HideMessageCallback(void *handler, hdAnimation *anim);

    hdUIContainer * GetCurrentMessageContainer() const;

    void SetMessageTexture(const char *texturePrefix);

    void ShowMessageInterface(hdUIContainer *container, hdUIContainer *buttonContainer,
                              const float screenWidth, const float screenHeight);

    void HideMessageInterface(hdUIContainer *container, hdUIContainer *buttonContainer,
                              const float screenWidth, const float screenHeight);

    /* Title Message */
    hdUIImage *m_uiLandscapeTitleMessageBackground;

    hdFontPolygon *m_uiLandscapeTitleMessageFont;

    hdUIImage *m_uiPortraitTitleMessageBackground;

    hdFontPolygon *m_uiPortraitTitleMessageFont;


    // Small Message queue button
    hdUIContainer *m_uiMessageQueueContainer;

    hdButton *m_uiMessageQueueButton;

    hdFontPolygon *m_uiMessageQueueCount;


    hdUIContainer *m_uiLandscapeMessageContainer;

    hdUIContainer *m_uiLandscapeMessageButtonContainer;

    hdUIContainer *m_uiPortraitMessageContainer;

    hdUIContainer *m_uiPortraitMessageButtonContainer;


    hdUIImage *m_uiLandscapeImageMessage;

    hdFontPolygon *m_uiLandscapeTextMessage;

    hdUIImage *m_uiLandscapeTextMessageBackground;

    hdButton *m_uiLandscapeMessageOKButton;

    hdButton *m_uiLandscapeMessageDoneButton;

    hdButton *m_uiLandscapeMessageSkipButton;

    hdButton *m_uiLandscapeMessagePrevButton;

    hdButton *m_uiLandscapeMessageNextButton;


    hdUIImage *m_uiPortraitImageMessage;

    hdFontPolygon *m_uiPortraitTextMessage;

    hdUIImage *m_uiPortraitTextMessageBackground;

    hdButton *m_uiPortraitMessageOKButton;

    hdButton *m_uiPortraitMessageDoneButton;

    hdButton *m_uiPortraitMessageSkipButton;

    hdButton *m_uiPortraitMessagePrevButton;

    hdButton *m_uiPortraitMessageNextButton;

    // Avatar Messages
    hdUIImage *m_uiLandscapeAvatar;

    hdUIImage *m_uiLandscapeAvatarChrome;

    hdFontPolygon *m_uiLandscapeAvatarMessageText;

    hdUIImage *m_uiLandscapeAvatarMessageBackground;

    hdUIImage *m_uiLandscapeAvatarMessageChrome;




    hdUIImage *m_uiPortraitAvatar;

    hdUIImage *m_uiPortraitAvatarChrome;

    hdFontPolygon *m_uiPortraitAvatarMessageText;

    hdUIImage *m_uiPortraitAvatarMessageBackground;

    hdUIImage *m_uiPortraitAvatarMessageChrome;


    /* Particle Emitters */
    SmokeEmitter* m_smokeEmitter;

    SplashEmitter* m_splashEmitter;

    SplashEmitter* m_gunSmokeEmitter;

    SparkEmitter* m_gunSparkEmitter;

    quadEmitter* m_waveParticleEmitter;

    StarEmitter *m_starParticleEmitter;

    StarEmitter *m_questionEmitter;

    SplinterEmitter *m_splinterEmitter;

    SmokeEmitter *m_explosionEmitter;

    SmokeEmitter *m_explosionSmokeEmitter;

    hdUIImage *m_glow;

    int m_currentLevelId;


    /* Sound */
    hdPointerList<hdSound, 8>* m_blockExplosionSoundsGlass;
    hdPointerList<hdSound, 8>* m_blockExplosionSoundsMetal;
    hdPointerList<hdSound, 8>* m_blockExplosionSoundsStone;
    hdPointerList<hdSound, 8>* m_blockExplosionSoundsWood;

    hdPointerList<hdSound, 8>* m_blockCollisionSoundsGlass;
    hdPointerList<hdSound, 8>* m_blockCollisionSoundsMetal;
    hdPointerList<hdSound, 8>* m_blockCollisionSoundsStone;
    hdPointerList<hdSound, 8>* m_blockCollisionSoundsWood;

    hdPointerList<hdSound, 16>* m_lightSplashSounds;
    hdPointerList<hdSound, 8>* m_blockLossSounds;

    hdSound *m_shotty;

    hdSound *m_btnMessageClickDownSound;
    hdSound *m_btnMessageClickUpSound;
    hdSound *m_bombSelectSound;
    hdSound *m_dragSelectSound;
    
    
    /* Misc */
    b2World* m_physicsWorld;
    
    float m_minFrameDuration;
    
    float m_physicsFixedFrameDuration;
    
    bool m_fixedPhysicsInterval;
    
    float m_variablePhysicsRateMultiplier;
    
    int m_physicsIterationsCount;
    
    
    
    b2MouseJoint* m_mouseJoint;
    
    totemBlock *m_HACK_lastJointedBlock;
    
    totemBlock* m_topBlock;
    
    totemBlock* m_floor;
    
    totemBlock* m_bottomBoundary;
    
    bool m_reflectParticles;
    
    
    
    totemLevel* m_currentLevel;
    
    hdAABB m_blockDeselectThreshold;
    
    
    
    b2BuoyancyController* m_buoyancyController;
    
    hdVec3 m_testPoint;
    
    hdVec3 m_lastRayScreenPoint;
    
    int m_rayQueryCount;
    
    totemBlock* m_currBlock;
    
    bool m_currBlockTapDown;
    
    int m_currBlockTicksSinceTapDown;
    
    bool m_tapDown;
};


inline const b2World* TotemLevelController::GetPhysicsWorld() const
{
    return m_physicsWorld;
}


inline const totemLevel* TotemLevelController::GetCurrentLevel() const
{
    return m_currentLevel;
}


inline void TotemLevelController::SetSelectedTotemBlock(const totemBlock *block)
{
    m_currBlock = (totemBlock *)block;
}


inline const totemBlock* TotemLevelController::GetSelectedTotemBlock() const
{
    return m_currBlock;
}


inline hdUIContainer * TotemLevelController::GetCurrentMessageContainer() const
{
    if (IsLandscapeOrientation())
    {
        return m_uiLandscapeMessageContainer;
    }
    else
    {
        return m_uiPortraitMessageContainer;
    }
}

#endif
