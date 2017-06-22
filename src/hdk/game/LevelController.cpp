/*
 *  LevelController.cpp
 *  TotemGame
 *
 *  Created by david on 23/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

using namespace std;
#include "LevelController.h"



static hdUIImage *m_uiImageMessage = NULL;

static hdFontPolygon *m_uiTextMessage = NULL;

static hdUIImage *m_uiTextMessageBackground = NULL;

static hdButton *m_OkButton = NULL;

static hdButton *m_DoneButton = NULL;

static hdButton *m_SkipButton = NULL;

static hdButton *m_PrevButton = NULL;

static hdButton *m_NextButton = NULL;

static hdUIContainer *m_uiMessageContainer = NULL;

static hdUIContainer *m_uiMessageButtonContainer = NULL;

static int m_textureFrameBuffer = -1;


LevelController::LevelController(const hdInterfaceController* parentController) :
hdInterfaceController(e_hdInterfaceOrientationLandscapeLeft, parentController)
{
    m_interactionState = e_interactionDisabled;

    m_currentLevel = NULL;
    m_testPoint.Set(0,0,0);
    m_lastRayScreenPoint.Set(0,0,0);
    m_rayQueryCount = 0;
    m_currBlock = NULL;

    m_currBlockTapDown = false;
    m_currBlockTicksSinceTapDown = 1000;

    m_tapDown = false;

    m_pushedProjectionMatrix = false;

    if ("Low" == hdConfig::GetValueForKey(CONFIG_EFFECTSQUALITYLEVEL_KEY) ||
        "Medium" == hdConfig::GetValueForKey(CONFIG_EFFECTSQUALITYLEVEL_KEY))
    {
        m_reflectParticles = false;
    }
    else
    {
        m_reflectParticles = true;
    }

    m_messageTagQueue = new hdTypedefList<totemMessage, 16>();
    m_physicsParticleEmitter = NULL;

    m_minFrameDuration = strtof(hdConfig::GetValueForKey(CONFIG_MINFRAMEDURATION_KEY).c_str(), NULL);
    m_minFrameDuration = hdClamp(m_minFrameDuration, 0.016f, 0.05f);

    m_fixedPhysicsInterval = (true == strtol(hdConfig::GetValueForKey(CONFIG_FIXEDPHYSICSINTERVAL_KEY).c_str(), NULL, 0));

    m_physicsFixedFrameDuration = strtof(hdConfig::GetValueForKey(CONFIG_PHYSICSFIXEDFRAMEDURATION_KEY).c_str(), NULL);
    m_physicsFixedFrameDuration = hdClamp(m_physicsFixedFrameDuration, 0.016f, 0.05f);

    m_variablePhysicsRateMultiplier = strtof(hdConfig::GetValueForKey("VariablePhysicsRateMultiplier").c_str(), NULL);
    if (m_variablePhysicsRateMultiplier == 0.0f)
    {
        m_variablePhysicsRateMultiplier = 1.0f;
    }
    else
    {
        m_variablePhysicsRateMultiplier = hdClamp(m_variablePhysicsRateMultiplier, 0.2f, 10.0f);
    }

    m_physicsIterationsCount = strtol(hdConfig::GetValueForKey(CONFIG_PHYSICSITERATIONSCOUNT_KEY).c_str(), NULL, 0);
    m_physicsIterationsCount = hdClamp(m_physicsIterationsCount, 1, 20);

    m_HACK_lastJointedBlock = false;

    this->InitPhysics();
    this->InitAnimations();
    this->InitSounds();
    this->InitEffects();
    this->InitInterface();
    this->RefreshLayout();
}


LevelController::~LevelController()
{
    delete m_uiMessageQueueButton;
    delete m_uiMessageQueueCount;
    delete m_uiMessageQueueContainer;

    delete m_uiLandscapeImageMessage;
    delete m_uiLandscapeTextMessage;
    delete m_uiLandscapeTextMessageBackground;
    delete m_uiLandscapeMessageOKButton;
    delete m_uiLandscapeMessageSkipButton;
    delete m_uiLandscapeMessagePrevButton;
    delete m_uiLandscapeMessageNextButton;
    delete m_uiLandscapeMessageDoneButton;

    delete m_uiPortraitImageMessage;
    delete m_uiPortraitTextMessage;
    delete m_uiPortraitTextMessageBackground;
    delete m_uiPortraitMessageOKButton;
    delete m_uiPortraitMessageSkipButton;
    delete m_uiPortraitMessagePrevButton;
    delete m_uiPortraitMessageNextButton;
    delete m_uiPortraitMessageDoneButton;

    delete m_uiLandscapeMessageContainer;
    delete m_uiLandscapeMessageButtonContainer;
    delete m_uiPortraitMessageContainer;
    delete m_uiPortraitMessageButtonContainer;

    delete m_uiLandscapeAvatar;
    delete m_uiLandscapeAvatarChrome;
    delete m_uiLandscapeAvatarMessageText;
    delete m_uiLandscapeAvatarMessageBackground;
    delete m_uiLandscapeAvatarMessageChrome;

    delete m_uiPortraitAvatar;
    delete m_uiPortraitAvatarChrome;
    delete m_uiPortraitAvatarMessageText;
    delete m_uiPortraitAvatarMessageBackground;
    delete m_uiPortraitAvatarMessageChrome;

    delete m_glow;

    delete m_blockLossSounds;
    delete m_lightSplashSounds;
    delete m_blockExplosionSoundsGlass;
    delete m_blockExplosionSoundsMetal;
    delete m_blockExplosionSoundsStone;
    delete m_blockExplosionSoundsWood;
    delete m_blockCollisionSoundsGlass;
    delete m_blockCollisionSoundsMetal;
    delete m_blockCollisionSoundsStone;
    delete m_blockCollisionSoundsWood;

    delete m_uiLandscapeTitleMessageBackground;
    delete m_uiLandscapeTitleMessageFont;
    delete m_uiPortraitTitleMessageBackground;
    delete m_uiPortraitTitleMessageFont;

    delete m_physicsParticleEmitter;
    delete m_smokeEmitter;
    delete m_gunSmokeEmitter;
    delete m_splashEmitter;
    delete m_gunSparkEmitter;
    delete m_waveParticleEmitter;
    delete m_physicsWorld;
}


void LevelController::InitPhysics()
{
    b2AABB m_physicsWorldAABB;
    m_physicsWorldAABB.lowerBound.Set(-256.0f, -256.0f);
    m_physicsWorldAABB.upperBound.Set(256.0f, 256.0f);

    b2Vec2 gravity(0.0f, kGravityMultiplier * -1.0f);
    bool doSleep = true;
    m_physicsWorld = new b2World(m_physicsWorldAABB, gravity, doSleep);
    m_physicsWorld->SetContactListener(this);
}


void LevelController::InitAnimations() {}


void LevelController::InitSounds()
{
    m_btnMessageClickDownSound = hdSoundManager::FindSound("Sounds/btnClickDown.caf", e_soundTypeNormal);
    m_btnMessageClickUpSound = hdSoundManager::FindSound("Sounds/btnClick01.caf", e_soundTypeNormal);

    /*
     * Heavy splashes
     */
    m_blockLossSounds = new hdPointerList<hdSound, 8>();
    m_blockLossSounds->Add(hdSoundManager::FindSound("Sounds/heavy_splash_01.caf", e_soundTypeNormal));
    m_blockLossSounds->Add(hdSoundManager::FindSound("Sounds/heavy_splash_02.caf", e_soundTypeNormal));
    m_blockLossSounds->Add(hdSoundManager::FindSound("Sounds/heavy_splash_03.caf", e_soundTypeNormal));
    m_blockLossSounds->Add(hdSoundManager::FindSound("Sounds/heavy_splash_04.caf", e_soundTypeNormal));
    m_blockLossSounds->Add(hdSoundManager::FindSound("Sounds/heavy_splash_05.caf", e_soundTypeNormal));

    /*
     * Light splashes
     */
    m_lightSplashSounds = new hdPointerList<hdSound, 16>();
    m_lightSplashSounds->Add(hdSoundManager::FindSound("Sounds/light_splash_01.caf", e_soundTypeNormal));
    m_lightSplashSounds->Add(hdSoundManager::FindSound("Sounds/light_splash_02.caf", e_soundTypeNormal));
    m_lightSplashSounds->Add(hdSoundManager::FindSound("Sounds/light_splash_03.caf", e_soundTypeNormal));
    m_lightSplashSounds->Add(hdSoundManager::FindSound("Sounds/light_splash_04.caf", e_soundTypeNormal));
    m_lightSplashSounds->Add(hdSoundManager::FindSound("Sounds/light_splash_05.caf", e_soundTypeNormal));

    /*
     * Glass explosions
     */
    m_blockExplosionSoundsGlass = new hdPointerList<hdSound, 8>();

    /*
     * Metal 'splosions
     */
    m_blockExplosionSoundsMetal = new hdPointerList<hdSound, 8>();

    /*
     * Stone explosions
     */
    m_blockExplosionSoundsStone = new hdPointerList<hdSound, 8>();

    m_blockExplosionSoundsStone->Add(hdSoundManager::FindSound("Sounds/heavy_smashstone_01.caf", e_soundTypeFrequent));
    m_blockExplosionSoundsStone->Add(hdSoundManager::FindSound("Sounds/heavy_smashwood_01.caf", e_soundTypeFrequent));
    m_blockExplosionSoundsStone->Add(hdSoundManager::FindSound("Sounds/glassmetal_explosion.caf", e_soundTypeFrequent));

    /*
     * Wood explosions
     */
    m_blockExplosionSoundsWood = new hdPointerList<hdSound, 8>();


    /*
     * Glass Collision Sounds
     */
    m_blockCollisionSoundsGlass = new hdPointerList<hdSound, 8>();

    /*
     * Metal collisions
     */
    m_blockCollisionSoundsMetal = new hdPointerList<hdSound, 8>();
    m_blockCollisionSoundsMetal->Add(hdSoundManager::FindSound("Sounds/impactmetal_01.caf", e_soundTypeFrequent));
    m_blockCollisionSoundsMetal->Add(hdSoundManager::FindSound("Sounds/impactmetal_02.caf", e_soundTypeFrequent));
    m_blockCollisionSoundsMetal->Add(hdSoundManager::FindSound("Sounds/impactmetal_03.caf", e_soundTypeFrequent));

    /*
     * Stone collisions
     */
    m_blockCollisionSoundsStone = new hdPointerList<hdSound, 8>();
    m_blockCollisionSoundsStone->Add(hdSoundManager::FindSound("Sounds/impactstone_02.caf", e_soundTypeFrequent));
    m_blockCollisionSoundsStone->Add(hdSoundManager::FindSound("Sounds/impactstone_03.caf", e_soundTypeFrequent));

    /*
     * Wood collisions
     */
    m_blockCollisionSoundsWood = new hdPointerList<hdSound, 8>();
    m_blockCollisionSoundsWood->Add(hdSoundManager::FindSound("Sounds/light_impactwood_01.caf", e_soundTypeFrequent));
    m_blockCollisionSoundsWood->Add(hdSoundManager::FindSound("Sounds/light_impactwood_02.caf", e_soundTypeFrequent));
    m_blockCollisionSoundsWood->Add(hdSoundManager::FindSound("Sounds/heavy_impactwood_01.caf", e_soundTypeFrequent));
    m_shotty = hdSoundManager::FindSound("Sounds/shotty.caf", e_soundTypeNormal);

    m_dragSelectSound = hdSoundManager::FindSound("Sounds/blockSelect01.caf", e_soundTypeNormal);

    m_bombSelectSound = hdSoundManager::FindSound("Sounds/blockSelect03.caf", e_soundTypeNormal);
}


void LevelController::InitEffects()
{
    m_physicsParticleEmitter = new hdPhysicsParticleEmitter(m_gameWorld, m_physicsWorld, m_projection, m_buoyancyController, 3.0f, 8.0f);

    m_smokeEmitter = new SmokeEmitter(48, "Materials/p_smoke3_15_32.tga", 1.44f, 1.5f, -6.0f, 0.005f);
    m_splinterEmitter = new SplinterEmitter(32, 1, 1.25f, -8.0f, 5.0f);


    // Perf tweaks for iPhone 1G
    if ("Low" == hdConfig::GetValueForKey(CONFIG_EFFECTSQUALITYLEVEL_KEY))
    {
        m_smokeEmitter->SetNextParticleCount(0);
        m_splinterEmitter->SetNextParticleCount(0);
    }
    else if ("Medium" == hdConfig::GetValueForKey(CONFIG_EFFECTSQUALITYLEVEL_KEY))
    {
        m_smokeEmitter->SetNextParticleCount(0);
        m_splinterEmitter->SetNextParticleCount(6);
    }

    m_explosionEmitter = new SmokeEmitter(48, "Materials/p_explode2_15_32.tga", 2.5f, 0.5f, -2.0f, 0.5f);
    m_explosionEmitter->SetParticleBlendStyle(e_particleBlendStyleMultiply);
    m_explosionEmitter->SetNextParticleCount(4);

    m_explosionSmokeEmitter = new SmokeEmitter(48, "Materials/p_smoke2_15_32.tga", 3.2f, 1.25f, -1.0f, 1.0f);
    m_explosionSmokeEmitter->SetParticleBlendStyle(e_particleBlendStyleDarken);
    m_explosionSmokeEmitter->SetNextParticleCount(4);

    //m_gunSmokeEmitter = new SplashEmitter(16, "/Materials/p_smokea0002.tga", 32.0f, 0.32f, 15.0f, 6.0f);

    m_questionEmitter = new StarEmitter(8, "Materials/p_qmark_15_32.tga", 0.0f, 3.0f, 24.0f, 0.75f);
    m_questionEmitter->m_ySpreadLower = -2.0f;
    m_questionEmitter->m_ySpreadUpper = 2.0f;
    m_questionEmitter->m_onStopResume = true;

    //m_splashEmitter =  new SplashEmitter(48, "Materials/p_splasha0001.tga", 64.0f, 2.0f, -10.0f, 0.5f);
    m_splashEmitter =  new SplashEmitter(48, "Materials/p_splasha0001.tga", 1.8f, 2.0f, -10.0f, 0.005f);

    //m_gunSparkEmitter = new SparkEmitter(1, "Materials/p_spark_14_128.tga", 3.0f, 0.075f, 0.00001f, 0.0001f);

    m_waveParticleEmitter = new QuadEmitter(32, "Materials/p_splashb_15_64.tga", 2.0f, 1.5f, 0.0f, 0.0f);
}



void LevelController::InitInterface()
{
    float longSide;
    float shortSide;



    longSide = hdMax(m_PixelScreenWidth, m_PixelScreenHeight);
    shortSide = hdMin(m_PixelScreenWidth, m_PixelScreenHeight);

    /*
     * Landscape messages
     */
    m_uiLandscapeTitleMessageBackground = new hdUIImage("/Interface/imgTitleBackground.png", NULL);
    m_uiLandscapeTitleMessageBackground->SetAs2DBox((longSide-256.0f)/2.0f, (shortSide-100.0f)/2.0f, 256.0f, 100.0f);

    m_uiLandscapeTitleMessageFont = new hdFontPolygon(LEVEL_CONTROLLER_TITLE_FONT, NULL);
    m_uiLandscapeTitleMessageFont->SetTint(0.0f, 0.0f, 0.0f, 1.0f);
    m_uiLandscapeTitleMessageFont->SetAs2DBox((longSide-256.0f)/2.0f, (shortSide-150.0f)/2.0f, 256.0f, 100.0f);
    m_uiLandscapeTitleMessageFont->AlignCenter();

    m_uiPortraitTitleMessageBackground = new hdUIImage("/Interface/imgTitleBackground.png", NULL);
    m_uiPortraitTitleMessageBackground->SetAs2DBox((shortSide-256.0f)/2.0f, (longSide-100.0f)/2.0f, 256.0f, 100.0f);

    m_uiPortraitTitleMessageFont = new hdFontPolygon(LEVEL_CONTROLLER_TITLE_FONT, NULL);
    m_uiPortraitTitleMessageFont->SetTint(0.0f, 0.0f, 0.0f, 1.0f);
    m_uiPortraitTitleMessageFont->SetAs2DBox((shortSide-256.0f)/2.0f, (longSide-100.0f)/2.0f, 256.0f, 100.0f);
    m_uiPortraitTitleMessageFont->AlignCenter();



    /*
     * Landscape messagebox
     */
    //m_uiLandscapeImageMessage = new hdUIImage("", NULL);
    //m_uiLandscapeImageMessage->SetAs2DBox(((longSide-240.0f)/2.0f)-longSide, 70.0f, 240.0f, 195.0f);

    m_uiLandscapeTextMessage = new hdFontPolygon(LEVEL_CONTROLLER_MESSAGE_FONT, NULL);
    m_uiLandscapeTextMessage->SetTint(0.0f, 0.0f, 0.0f, 1.0f);
    m_uiLandscapeTextMessage->SetScale(0.75f);
    m_uiLandscapeTextMessage->SetAs2DBox(200.0f-longSide, 75.0f, 175.0f, 173.0f);
    m_uiLandscapeTextMessage->AlignLeft();

    m_uiLandscapeTextMessageBackground = new hdUIImage(NULL, NULL);
    m_uiLandscapeTextMessageBackground->SetAs2DBox(((longSide-240.0f)/2.0f), 60.0f, 240.0f, 240.0f);

    // Avatar message support
    m_uiLandscapeAvatar = new hdUIImage("", NULL);
    m_uiLandscapeAvatar->SetAs2DBox(20.0f, shortSide - (60.0f + 128.0f), 128.0f, 128.0f);
    m_uiLandscapeAvatar->Hide();

    m_uiLandscapeAvatarChrome = new hdUIImage(NULL, NULL);
    m_uiLandscapeAvatarChrome->SetTint(0.0f, 0.0f, 0.0f, 0.8f);
    m_uiLandscapeAvatarChrome->SetAs2DBox(10.0f, shortSide - (50.0f + 148.0f), 148.0f, 148.0f);
    m_uiLandscapeAvatarChrome->Hide();

    m_uiLandscapeAvatarMessageText = new hdFontPolygon(LEVEL_CONTROLLER_MESSAGE_FONT, NULL);
    m_uiLandscapeAvatarMessageText->SetTint(0.1f, 0.1f, 0.1f, 1.0f);
    m_uiLandscapeAvatarMessageText->SetAs2DBox(203.0f, 75.0f, 195.0f, 183.0f);
    m_uiLandscapeAvatarMessageText->SetScale(0.8f);
    m_uiLandscapeAvatarMessageText->AlignLeft();
    m_uiLandscapeAvatarMessageText->Hide();

    m_uiLandscapeAvatarMessageBackground = new hdUIImage("Interface/imgLandscapeMessageBackground.png", NULL);
    m_uiLandscapeAvatarMessageBackground->SetTint(1.0f, 1.0f, 1.0f, 1.0f);
    m_uiLandscapeAvatarMessageBackground->SetAs2DBox(168.0f, 60.0f, longSide - (10.0f + 168.0f), 220.0f);
    m_uiLandscapeAvatarMessageBackground->Hide();

    m_uiLandscapeAvatarMessageChrome = new hdUIImage(NULL, NULL);
    m_uiLandscapeAvatarMessageChrome->SetTint(0.0f, 0.0f, 0.0f, 0.0f);
    m_uiLandscapeAvatarMessageChrome->SetAs2DBox(168.0f, 50.0f, longSide - (10.0f + 168.0f), 220.0f);
    m_uiLandscapeAvatarMessageChrome->Hide();


    /*
     * Landscape Buttons
     */
    m_uiLandscapeMessageOKButton  = new hdButton("/Interface/btnOkNormal.png",
                                                 "/Interface/btnOkOver.png",
                                                 "/Interface/btnOkOver.png", NULL);
    m_uiLandscapeMessageOKButton->SetAs2DBox(((longSide-64.0f)/2.0f)-longSide, 5.0f, 64.0f, 40.0f);


    m_uiLandscapeMessageDoneButton  = new hdButton("/Interface/btnDoneNormal.png",
                                                   "/Interface/btnDoneOver.png",
                                                   "/Interface/btnDoneOver.png", NULL);
    m_uiLandscapeMessageDoneButton->SetAs2DBox((longSide/2.0f)+10.0f-longSide, 5.0f, 64.0f, 40.0f);

    m_uiLandscapeMessagePrevButton  = new hdButton("/Interface/btnPrevNormal.png",
                                                   "/Interface/btnPrevOver.png",
                                                   "/Interface/btnPrevOver.png", NULL);
    m_uiLandscapeMessagePrevButton->SetAs2DBox((longSide/2.0f)-74.0f-longSide, 5.0f, 64.0f, 40.0f);

    m_uiLandscapeMessageNextButton  = new hdButton("/Interface/btnNextNormal.png",
                                                   "/Interface/btnNextOver.png",
                                                   "/Interface/btnNextOver.png", NULL);
    m_uiLandscapeMessageNextButton->SetAs2DBox((longSide/2.0f)+10.0f-longSide, 5.0f, 64.0f, 40.0f);

    m_uiLandscapeMessageSkipButton = new hdButton("/Interface/btnSkipSmallNormal.png",
                                                  "/Interface/btnSkipSmallOver.png",
                                                  "/Interface/btnSkipSmallOver.png", NULL);
    m_uiLandscapeMessageSkipButton->SetAs2DBox((longSide-74.0f)-longSide, 5.0f, 48.0f, 30.0f);

    m_uiLandscapeMessageContainer = new hdUIContainer(NULL, hdVec3(-longSide, 0.0f, 0.0f),
                                                      hdVec3(0.0f, shortSide, 0.0f));
    m_uiLandscapeMessageContainer->Hide();


    //m_uiLandscapeMessageContainer->Add(m_uiLandscapeImageMessage);
    m_uiLandscapeMessageContainer->Add(m_uiLandscapeTextMessageBackground);
    m_uiLandscapeMessageContainer->Add(m_uiLandscapeTextMessage);

    m_uiLandscapeMessageContainer->Add(m_uiLandscapeTitleMessageBackground);
    m_uiLandscapeMessageContainer->Add(m_uiLandscapeTitleMessageFont);

    m_uiLandscapeMessageContainer->Add(m_uiLandscapeAvatarChrome);
    m_uiLandscapeMessageContainer->Add(m_uiLandscapeAvatarMessageChrome);
    m_uiLandscapeMessageContainer->Add(m_uiLandscapeAvatarMessageBackground);
    m_uiLandscapeMessageContainer->Add(m_uiLandscapeAvatar);
    m_uiLandscapeMessageContainer->Add(m_uiLandscapeAvatarMessageText);

    m_uiLandscapeMessageContainer->Translate(hdVec3(-longSide, 0.0f, 0.0f), m_uiLandscapeMessageContainer->GetWorldCenter());

    m_uiLandscapeMessageButtonContainer = new hdUIContainer(NULL, hdVec3(-longSide, 0.0f, 0.0f),
                                                            hdVec3(0.0f, shortSide, 0.0f));
    m_uiLandscapeMessageButtonContainer->Add(m_uiLandscapeMessageOKButton);
    m_uiLandscapeMessageButtonContainer->Add(m_uiLandscapeMessageDoneButton);
    m_uiLandscapeMessageButtonContainer->Add(m_uiLandscapeMessageSkipButton);
    m_uiLandscapeMessageButtonContainer->Add(m_uiLandscapeMessagePrevButton);
    m_uiLandscapeMessageButtonContainer->Add(m_uiLandscapeMessageNextButton);

    /*
     * Portrait messagebox
     */
    //m_uiPortraitImageMessage = new hdUIImage("", NULL);
    //m_uiPortraitImageMessage->SetAs2DBox(((shortSide-240.0f)/2.0f)-shortSide, 150.0f, 240.0f, 195.0f);

    m_uiPortraitTextMessage = new hdFontPolygon(LEVEL_CONTROLLER_MESSAGE_FONT, NULL);
    m_uiPortraitTextMessage->SetTint(0.0f, 0.0f, 0.0f, 1.0f);
    m_uiPortraitTextMessage->SetAs2DBox(30.0f, 140.0f, 200.0f, 130.0f);
    m_uiPortraitTextMessage->AlignLeft();

    m_uiPortraitTextMessageBackground = new hdUIImage(NULL, NULL);
    m_uiPortraitTextMessageBackground->SetAs2DBox(((shortSide-256.0f)/2.0f), 80.0f, 256.0f, 256.0f);

    m_uiPortraitAvatar = new hdUIImage("", NULL);
    m_uiPortraitAvatar->SetAs2DBox(20.0f, longSide - (60.0f + 128.0f), 128.0f, 128.0f);
    m_uiPortraitAvatar->Hide();

    m_uiPortraitAvatarChrome = new hdUIImage(NULL, NULL);
    m_uiPortraitAvatarChrome->SetTint(0.0f, 0.0f, 0.0f, 0.7f);
    m_uiPortraitAvatarChrome->SetAs2DBox(10.0f, longSide - (50.0f + 148.0f), 148.0f, 148.0f);
    m_uiPortraitAvatarChrome->Hide();

    m_uiPortraitAvatarMessageText = new hdFontPolygon(LEVEL_CONTROLLER_MESSAGE_FONT, NULL);
    m_uiPortraitAvatarMessageText->SetTint(0.0f, 0.0f, 0.0f, 1.0f);
    m_uiPortraitAvatarMessageText->SetAs2DBox(30.0f, 80.0f, 200.0f, 180.0f);
    m_uiPortraitAvatarMessageText->AlignLeft();
    m_uiPortraitAvatarMessageText->SetScale(0.8f);
    m_uiPortraitAvatarMessageText->Hide();



    m_uiPortraitAvatarMessageChrome = new hdUIImage(NULL, NULL);
    m_uiPortraitAvatarMessageChrome->SetTint(0.0f, 0.0f, 0.0f, 0.0f);
    m_uiPortraitAvatarMessageChrome->SetAs2DBox(10.0f, 70.0f, shortSide - 20.0f, longSide - (50.0f + 148.0f + 70.0f + 10.0f));
    m_uiPortraitAvatarMessageChrome->Hide();

    m_uiPortraitAvatarMessageBackground = new hdUIImage("Interface/imgPortraitMessageBackground.png", NULL);
    m_uiPortraitAvatarMessageBackground->SetTint(1.0f, 1.0f, 1.0f, 1.0f);
    m_uiPortraitAvatarMessageBackground->SetAs2DBox(10.0f, 70.0f, shortSide - 20.0f, longSide - (50.0f + 128.0f + 70.0f + 10.0f));
    m_uiPortraitAvatarMessageBackground->Hide();

    /*
     * Portrain Message buttons
     */
    m_uiPortraitMessageOKButton  = new hdButton("/Interface/btnOkNormal.png",
                                                "/Interface/btnOkOver.png",
                                                "/Interface/btnOkOver.png", NULL);
    m_uiPortraitMessageOKButton->SetAs2DBox(((shortSide-64.0f)/2.0f)-shortSide, 20.0f, 64.0f, 40.0f);


    m_uiPortraitMessageDoneButton  = new hdButton("/Interface/btnDoneNormal.png",
                                                  "/Interface/btnDoneOver.png",
                                                  "/Interface/btnDoneOver.png", NULL);
    m_uiPortraitMessageDoneButton->SetAs2DBox((shortSide/2.0f)+10.0f-shortSide, 20.0f, 64.0f, 40.0f);

    m_uiPortraitMessagePrevButton  = new hdButton("/Interface/btnPrevNormal.png",
                                                  "/Interface/btnPrevOver.png",
                                                  "/Interface/btnPrevOver.png", NULL);
    m_uiPortraitMessagePrevButton->SetAs2DBox((shortSide/2.0f)-74.0f-shortSide, 20.0f, 64.0f, 40.0f);

    m_uiPortraitMessageNextButton  = new hdButton("/Interface/btnNextNormal.png",
                                                  "/Interface/btnNextOver.png",
                                                  "/Interface/btnNextOver.png", NULL);
    m_uiPortraitMessageNextButton->SetAs2DBox((shortSide/2.0f)+10.0f-shortSide, 20.0f, 64.0f, 40.0f);


    m_uiPortraitMessageSkipButton = new hdButton("/Interface/btnSkipSmallNormal.png",
                                                 "/Interface/btnSkipSmallOver.png",
                                                 "/Interface/btnSkipSmallOver.png", NULL);
    m_uiPortraitMessageSkipButton->SetAs2DBox((shortSide-74.0f)-shortSide, 20.0f, 48.0f, 30.0f);


    m_uiPortraitMessageContainer = new hdUIContainer(NULL, hdVec3(-shortSide, 0.0f, 0.0f),
                                                     hdVec3(0.0f, longSide, 0.0f));
    m_uiPortraitMessageContainer->Hide();

    //m_uiPortraitMessageContainer->Add(m_uiPortraitImageMessage);
    m_uiPortraitMessageContainer->Add(m_uiPortraitTextMessageBackground);
    m_uiPortraitMessageContainer->Add(m_uiPortraitTextMessage);

    m_uiPortraitMessageContainer->Add(m_uiPortraitTitleMessageBackground);
    m_uiPortraitMessageContainer->Add(m_uiPortraitTitleMessageFont);

    m_uiPortraitMessageContainer->Add(m_uiPortraitAvatarChrome);
    m_uiPortraitMessageContainer->Add(m_uiPortraitAvatar);
    m_uiPortraitMessageContainer->Add(m_uiPortraitAvatarMessageChrome);
    m_uiPortraitMessageContainer->Add(m_uiPortraitAvatarMessageBackground);
    m_uiPortraitMessageContainer->Add(m_uiPortraitAvatarMessageText);


    m_uiPortraitMessageContainer->Translate(hdVec3(-shortSide, 0.0f, 0.0f), m_uiPortraitMessageContainer->GetWorldCenter());

    m_uiPortraitMessageButtonContainer = new hdUIContainer(NULL, hdVec3(-shortSide, 0.0f, 0.0f),
                                                           hdVec3(0.0f, longSide, 0.0f));
    m_uiPortraitMessageButtonContainer->Add(m_uiPortraitMessageOKButton);
    m_uiPortraitMessageButtonContainer->Add(m_uiPortraitMessageSkipButton);
    m_uiPortraitMessageButtonContainer->Add(m_uiPortraitMessagePrevButton);
    m_uiPortraitMessageButtonContainer->Add(m_uiPortraitMessageNextButton);
    m_uiPortraitMessageButtonContainer->Add(m_uiPortraitMessageDoneButton);

    m_glow = new hdUIImage("Materials/s_glow_15.tga", NULL, 0.0f, 0.0f, 10.0f, 10.0f);

    /*
     * Message queue container button
     */
    m_uiMessageQueueContainer = new hdUIContainer(NULL, hdVec3(-m_PixelScreenWidth, 0.0f, 0.0f),
                                                  hdVec3(0.0f, m_PixelScreenHeight, 0.0f));
    m_uiMessageQueueContainer->Hide();


    m_uiMessageQueueButton = new hdButton("/Interface/btnMessageQueueNormal.png",
                                          "/Interface/btnMessageQueueOver.png",
                                          "/Interface/btnMessageQueueOver.png", NULL);

    m_uiMessageQueueCount = new hdFontPolygon(LEVEL_CONTROLLER_TINY_FONT, NULL);
    m_uiMessageQueueCount->SetTint(1.0f, 0.75f, 0.0f, 1.0f);
    m_uiMessageQueueCount->AlignLeft();

    m_uiMessageQueueContainer->Add(m_uiMessageQueueButton);
    m_uiMessageQueueContainer->Add(m_uiMessageQueueCount);


}


void LevelController::RefreshLayout()
{
    m_uiMessageQueueButton->SetAs2DBox(5.0f, 5.0f, 45.0f, 45.0f);
    m_uiMessageQueueCount->SetAs2DBox(38.0f, 17.0f, 14.0f, 14.0f);



    if (IsLandscapeOrientation())
    {
        m_uiImageMessage = m_uiLandscapeImageMessage;
        m_uiTextMessage = m_uiLandscapeTextMessage;
        m_uiTextMessageBackground = m_uiLandscapeTextMessageBackground;

        m_OkButton = m_uiLandscapeMessageOKButton;
        m_DoneButton = m_uiLandscapeMessageDoneButton;
        m_SkipButton = m_uiLandscapeMessageSkipButton;
        m_PrevButton = m_uiLandscapeMessagePrevButton;
        m_NextButton = m_uiLandscapeMessageNextButton;

        m_uiMessageContainer = m_uiLandscapeMessageContainer;
        m_uiMessageButtonContainer = m_uiLandscapeMessageButtonContainer;
    }
    else
    {
        m_uiImageMessage = m_uiPortraitImageMessage;
        m_uiTextMessage = m_uiPortraitTextMessage;
        m_uiTextMessageBackground = m_uiPortraitTextMessageBackground;

        m_OkButton = m_uiPortraitMessageOKButton;
        m_DoneButton = m_uiPortraitMessageDoneButton;
        m_SkipButton = m_uiPortraitMessageSkipButton;
        m_PrevButton = m_uiPortraitMessagePrevButton;
        m_NextButton = m_uiPortraitMessageNextButton;

        m_uiMessageContainer = m_uiPortraitMessageContainer;
        m_uiMessageButtonContainer = m_uiPortraitMessageButtonContainer;
    }

    m_OkButton->Show();
    m_SkipButton->Hide();
    m_PrevButton->Hide();
    m_NextButton->Hide();
    m_DoneButton->Hide();
}



const bool LevelController::InitLevel(const Level* level, const int levelId)
{
    unsigned i;
    TextBlock *textBlock;
    Block *block;
    Event *event;
    Joint *joint;
    b2BuoyancyControllerDef bcd;
    totemMessage *scriptMessages[20];
    int msgCount = 0;


    if (level == NULL)
    {
        m_currentLevel = NULL;
        return false;
    }

    m_currentLevel = (Level *)level;
    m_currentLevelId = levelId;

    if (m_physicsParticleEmitter != NULL)
    {
        m_physicsParticleEmitter->RemoveAllParticles();
    }

    m_currentLevel->Restart();
    m_currentLevel->InitGameObjects(m_gameWorld, m_projection);
    m_currentLevel->InitPhysicsObjects(m_gameWorld, m_physicsWorld);

    // Buoyancy stuff...
    if (m_currentLevel->GetFloorInfo() != NULL)
    {
        bcd.offset = m_currentLevel->GetFloorInfo()->m_floorLevel; //(-kScreenHeight/2.0f)-0.6f;
        bcd.normal.Set(0,1);
        bcd.density = 15.0;
        bcd.linearDrag = 2;
        bcd.angularDrag = 1;

        m_buoyancyController = (b2BuoyancyController*)m_physicsWorld->CreateController(&bcd);
    }

    /* Question Particle Emitter */
    m_questionEmitter->m_onStopResume = false;
    m_questionEmitter->Stop();
    for (i = 0; i < m_currentLevel->GetEvents()->GetItemCount(); ++i)
    {
        event = (Event *)m_currentLevel->GetEvents()->GetItems()[i];

        if (event->GetEventType() == e_hdkEventTypeShowMessage)
        {
            hdGameObject *obj = (hdGameObject *)event->GetFirstObject();
            m_questionEmitter->Start(obj->GetWorldCenter().x, obj->GetWorldCenter().y);
            m_questionEmitter->m_onStopResume = true;
            break;
        }
    }

    /*
     * Only draggable blocks are buoyant
     */
    for (i= 0; i < m_currentLevel->GetBlockCount(); ++i)
    {
        block = (Block *)m_currentLevel->GetBlocks()[i];
        if (block->IsDraggable())
        {
            if (m_buoyancyController != NULL && block->GetPhysicsBody() != NULL)
            {
                m_buoyancyController->AddBody((b2Body*)block->GetPhysicsBody());
            }
        }

        if (block->GetMaterial() == e_hdkMaterialText)
        {
            textBlock = new TextBlock(block, levelId);
            ((Level *)level)->AddBlock(textBlock);
        }
    }

    /*
     * In game flags for jointed blocks.
     */
    for (i = 0; i < level->GetJointCount(); ++i)
    {
        joint = (Joint *)level->GetJoints()[i];
        if (joint->GetJointType() == e_hdkJointTypeRevolute)
        {
            block = (Block *)joint->GetFirstBody();
            if (block != NULL)
                block->AddGameFlag(e_hdkGameFlagsBlockHasRevJoint);

            block = (Block *)joint->GetSecondBody();
            if (block != NULL)
                block->AddGameFlag(e_hdkGameFlagsBlockHasRevJoint);
        }
    }


    m_projection->ResetAABB();
    m_worldAABB = m_currentLevel->GetAABB();

    // Barriers on top and either side of the world AABB.
    m_topBlock = new Block(m_gameWorld,
                                m_physicsWorld,
                                hdVec2(m_worldAABB.lower.x, m_worldAABB.upper.y),
                                hdVec2(m_worldAABB.upper.x, m_worldAABB.upper.y+0.2f),
                                e_hdkMaterialWood,
                                e_hdkShapeTypeRectangle,
                                e_hdkBlockTypeBoundary);
    m_topBlock->Hide();
    m_currentLevel->AddBlock(m_topBlock);


    Block* left = new Block(m_gameWorld,
                                      m_physicsWorld,
                                      hdVec2(m_worldAABB.lower.x - 0.1f, m_worldAABB.lower.y),
                                      hdVec2(m_worldAABB.lower.x, m_worldAABB.upper.y),
                                      e_hdkMaterialWood,
                                      e_hdkShapeTypeRectangle,
                                      e_hdkBlockTypeBoundary);
    left->Hide();
    m_currentLevel->AddBlock(left);

    Block* right = new Block(m_gameWorld,
                                       m_physicsWorld,
                                       hdVec2(m_worldAABB.upper.x, m_worldAABB.lower.y),
                                       hdVec2(m_worldAABB.upper.x+0.1f, m_worldAABB.upper.y),
                                       e_hdkMaterialWood,
                                       e_hdkShapeTypeRectangle,
                                       e_hdkBlockTypeBoundary);
    right->Hide();
    m_currentLevel->AddBlock(right);

    m_bottomBoundary = new Block(m_gameWorld,
                                      m_physicsWorld,
                                      hdVec2(m_worldAABB.lower.x, m_worldAABB.lower.y-0.1f),
                                      hdVec2(m_worldAABB.upper.x, m_worldAABB.lower.y),
                                      e_hdkMaterialWood,
                                      e_hdkShapeTypeRectangle,
                                      e_hdkBlockTypeHazard);
    m_bottomBoundary->Hide();
    m_currentLevel->AddBlock(m_bottomBoundary);


    // HACK HACK HACK
    // Floor info needs more options. The floor rules manifest themselves like this:
    // - no floor object means a hazard below the world AABB
    // - water floor means a water hanzard at floor level
    // - no textures mean an invisible floor hazard at the floor level
    // - otherwise the floor is a solid non-hazard.
    // WTF????
    m_floor = NULL;
    if (m_currentLevel->GetFloorInfo() != NULL)
    {
        if (m_currentLevel->GetFloorInfo()->m_isWater)
        {
            // Water is a hazard.
            m_floor = new Block(m_gameWorld,
                                     m_physicsWorld,
                                     hdVec2(m_worldAABB.lower.x, m_worldAABB.lower.y),
                                     hdVec2(m_worldAABB.upper.x, m_currentLevel->GetFloorInfo()->m_floorLevel),
                                     e_hdkMaterialWood,
                                     e_hdkShapeTypeRectangle,
                                     e_hdkBlockTypeHazard);
            //	m_floor->SetTint(0.5f, 0.7f, 1.0f, 0.8f);
            hdTranslateVertices(m_floor->GetVertices(), m_floor->GetVertexCount(), hdVec3(0.0f, 0.0f, 10.0f));
            m_floor->Hide();
            m_currentLevel->AddBlock(m_floor);
        }
        else if (m_currentLevel->GetFloorInfo()->IsDrawable() == false)
        {
            m_floor = new Block(m_gameWorld,
                                     m_physicsWorld,
                                     hdVec2(m_worldAABB.lower.x, m_worldAABB.lower.y),
                                     hdVec2(m_worldAABB.upper.x, m_currentLevel->GetFloorInfo()->m_floorLevel),
                                     e_hdkMaterialWood,
                                     e_hdkShapeTypeRectangle,
                                     e_hdkBlockTypeHazard);
            m_floor->Hide();
            m_currentLevel->AddBlock(m_floor);
        }
        else
        {
            // Floor is NOT a hazard block
            m_floor = new Block(m_gameWorld,
                                     m_physicsWorld,
                                     hdVec2(m_worldAABB.lower.x, m_worldAABB.lower.y),
                                     hdVec2(m_worldAABB.upper.x, m_currentLevel->GetFloorInfo()->m_floorLevel),
                                     e_hdkMaterialWood,
                                     e_hdkShapeTypeRectangle,
                                     e_hdkBlockTypeBoundary);
            m_floor->Hide();
            m_currentLevel->AddBlock(m_floor);
        }
    }


    m_physicsParticleEmitter->RemoveAllParticles();

    if (m_currBlock == NULL)
    {
        m_currBlock = (Block *)FindBlockWithTag(1001);
    }

    // Preload script textures:
    msgCount = Scripts_GetLevelMessagesForTag(scriptMessages,
                                              20, levelId, _HDK_SCRIPTS_WILDCARD_TAG);

    for (int i = 0; i < hdMin(msgCount, 20); i++)
    {
        if (scriptMessages[i]->messageType == e_hdkMessageTypeImage ||
            scriptMessages[i]->messageType == e_hdkMessageTypeCustomImageText ||
            scriptMessages[i]->messageType == e_hdkMessageTypeAvatar)
        {
            hdTextureManager::Instance()->FindTexture(scriptMessages[i]->texture, TT_Wall);
        }
    }

    Game_InitLevel(level, levelId);

    return true;
}



void LevelController::ResetCurrentLevel()
{
    if (m_currentLevel != NULL)
    {
        m_currentLevel->DestroyGameObjects();
        if (m_buoyancyController != NULL)
        {
            m_physicsWorld->DestroyController(m_buoyancyController);
            m_buoyancyController = NULL;
        }
    }
}


void LevelController::RestartCurrentLevel()
{
    Game_ResetCurrentLevel();

    this->ResetCurrentLevel();

    this->ResetGameWorld();

    this->InitLevel(m_currentLevel, m_currentLevelId);
}



Block* LevelController::FindBlockWithTag(const int tag) const
{
    Block *b = NULL;

    for (int i = 0; i < m_currentLevel->GetBlockCount(); ++i)
    {
        if (m_currentLevel->GetBlocks()[i]->GetTag() == tag)
        {
            b = (Block*)m_currentLevel->GetBlocks()[i];
            break;
        }
    }
    return b;
}


void LevelController::PhysicsStep(double interval)
{
    float proportion;

    if (m_currentLevel == NULL) return;
    if (m_physicsWorld == NULL) return;

    this->ResetContactPoints();

    // Paused
    if (m_interactionState != e_interactionDisabled)
    {
        if (m_fixedPhysicsInterval)
        {
            proportion = hdClamp(interval, 0.016, 2.0 * m_physicsFixedFrameDuration);

            // Try it twice...
            m_physicsWorld->Step(0.5f * proportion * m_variablePhysicsRateMultiplier, //0.016, //m_physicsFixedFrameDuration,
                                 m_physicsIterationsCount,
                                 m_physicsIterationsCount);

            m_physicsWorld->Step(0.5f * proportion * m_variablePhysicsRateMultiplier, //0.016, //m_physicsFixedFrameDuration,
                                 m_physicsIterationsCount,
                                 m_physicsIterationsCount);
        }
        else
        {
            //
            printf("Unfixed physics interval no longer supported. \n");
            hdAssert(false);
            /*
             proportion = (float)hdClamp(interval, 0.016, 0.04);
             m_physicsWorld->Step(proportion * m_variablePhysicsRateMultiplier,
             m_physicsIterationsCount,
             m_physicsIterationsCount);
             */
        }
    }
}


void LevelController::PanToSelectedObject(double interval)
{
    hdVec3 pan;
    if (m_currBlock != NULL &&
        m_interactionState != e_interactionDisabled &&
        m_currBlock->GetPhysicsBody() != NULL && m_currBlock->IsDraggable())
    {
        pan = PanToGameObject(m_currBlock,
                              m_projection->GetWorldCenter(),
                              m_projection->GetAABB(),
                              m_tapDown,
                              interval);

        // HACK HACK HACK
        //
        // OLD SKOOL PAN - THIS IS A FUCKING BIG, GAME SPECIFIC HACK
        // SPECIFICALLY FOR LEVEL BLOCKS.
        //
        // The entire camera system needs to be rethought in hdTech2.
        if (pan.x == 0 && pan.y == 0 && pan.z == 0)
        {
            if (m_currBlock->ContainsGameFlag((uint32)e_hdkGameFlagsBlockDrawRaw))
            {
                b2Vec2 center = m_currBlock->GetPhysicsBody()->GetWorldCenter();
                pan = PanToGameObject(hdVec3(center.x, center.y, 0.0f),
                                      m_projection->GetWorldCenter(),
                                      m_projection->GetAABB());

                if (IsLandscapeOrientation())
                {
                    PanProjection(0.0f, 0.0f, pan.y * 5.0f, pan.x * -5.0f);
                }
                else
                {
                    PanProjection(0.0f, 0.0f, pan.x * 5.0f, pan.y * 5.0f);
                }
            }
        }
        else
        {

            TranslateProjection(pan);
        }
    }

}


static bool playedLoudCollisionSound = false;
static bool playedSoftCollisionSound = false;
static bool playedHeavySplashSound = false;
static bool playedLightSplashSound = false;
static bool playedBlockSmashSound = false;
static bool playedParticleSmashSound = false;
void LevelController::Step(double interval)
{



    if (m_currentLevel == NULL) return;

    PanToSelectedObject(interval);

    PhysicsStep(interval);


    /*
     // Paused
     if (m_interactionState != e_interactionDisabled)
     {
     if (m_fixedPhysicsInterval)
     {
     // Try it twice...
     m_physicsWorld->Step(0.016f, //m_physicsFixedFrameDuration,
     m_physicsIterationsCount,
     m_physicsIterationsCount);

     m_physicsWorld->Step(0.016f, //m_physicsFixedFrameDuration,
     m_physicsIterationsCount,
     m_physicsIterationsCount);
     }
     else
     {
     proportion = (float)hdClamp(interval, 0.016, 0.04);
     m_physicsWorld->Step(proportion * m_variablePhysicsRateMultiplier,
     m_physicsIterationsCount,
     m_physicsIterationsCount);
     }
     }*/

    m_worldAABB = m_currentLevel->GetAABB();

    ++m_currBlockTicksSinceTapDown;


    m_panMomentumEnabled = ((m_interactionState != e_panAndZoom) || !m_tapDown); //(m_interactionState == e_waitingForInput);
    if (hdFastHypotenuse(m_lastPanDirectionVector.x, m_lastPanDirectionVector.y) > 0.0f)
    {
        m_lastPanDirectionVector = hdClamp(m_lastPanDirectionVector - (0.11f * m_lastPanDirectionVector),
                                           hdVec2(-70.0f, -70.0f), hdVec2(70.0f, 70.0f));

        if (m_panMomentumEnabled)
        {
            this->PanProjection(m_lastPanDirectionVector.x,
                                m_lastPanDirectionVector.y,
                                0.0f, 0.0f);
        }
    }



    playedLoudCollisionSound = false;
    playedSoftCollisionSound = false;
    playedHeavySplashSound = false;
    playedLightSplashSound = false;
    playedBlockSmashSound = false;
    playedParticleSmashSound = false;

    for (int i = 0; i < m_contactCount; ++i)
    {
        ContactPoint* point = m_contacts + i;
        this->HandleCollision(point);
    }

    if (m_interactionState != e_interactionDisabled)
    {
        if (m_fixedPhysicsInterval)
        {
            m_physicsParticleEmitter->Step(m_minFrameDuration);
        }
        else
        {
            m_physicsParticleEmitter->Step(interval);
        }
    }



    Game_Step(interval);

    m_currentLevel->Step();


}


// Check the collisions and divert to collision
// game event handler functions.
void LevelController::HandleCollision(const ContactPoint* point)
{
    b2Body* b1 = point->body1;
    b2Body* b2 = point->body2;

    if (point->state == e_contactAdded && b1->GetUserData() != NULL && b2->GetUserData() != NULL)
    {
        hdPolygon* obj1 = (hdPolygon *)(b1->GetUserData());
        hdPolygon* obj2 = (hdPolygon *)(b2->GetUserData());

        if (obj1->GetUserType() == (int)e_hdkTypeBlock && obj2->GetUserType() == (int)kPhysicsParticleUserType)
        {
            HandleParticleBlockCollision((hdPhysicsParticle *)obj2, (Block *)obj1, point);
        }
        else if (obj2->GetUserType() == (int)e_hdkTypeBlock && obj1->GetUserType() == (int)kPhysicsParticleUserType)
        {
            HandleParticleBlockCollision((hdPhysicsParticle *)obj1, (Block *)obj2, point);
        }
        else if (obj1->GetUserType() == (int)e_hdkTypeBlock && obj2->GetUserType() == (int)e_hdkTypeBlock)
        {
            HandleBlockBlockCollision((Block *)obj1, (Block *)obj2, point);

        }
        else if (point->state == e_contactAdded)
        {
            PlaySoftCollisionSound((Block *)obj1, b1);
        }
    }

    this->Game_HandleCollision(point);
}


void LevelController::HandleParticleBlockCollision(hdPhysicsParticle *particle, Block *block, const ContactPoint *point)
{
    if (block == m_floor)
    {
        HandleFragmentSplash(particle, point);
        return;
    }

    if (block->IsHazard() ||
        block->GetMaterial() == e_hdkMaterialGrinder ||
        block->GetMaterial() == e_hdkMaterialBomb)
    {
        HandleParticleTap(particle, hdVec3(point->position.x, point->position.y, 0.0f), true);
        return;
    }

    // - Wood block on plastic particle means a particle tap if velocity > 1 m/s; no hazard collision.
    if (particle->m_parentTag == (unsigned int)e_hdkMaterialPlastic &&
        block->GetMaterial() == e_hdkMaterialWood)
    {
        HandleParticleTap(particle, hdVec3(point->position.x, point->position.y, 0.0f), true);
    }
}


void LevelController::HandleFragmentSplash(hdPhysicsParticle *fragment, const ContactPoint *point)
{
    if (false == fragment->m_interacts) return;

    if (m_currentLevel->GetFloorInfo() != NULL && m_currentLevel->GetFloorInfo()->m_isWater)
    {
        // Splash initial size is the depth of the falling item
        m_waveParticleEmitter->SetParticlePointSize(fabs(fragment->m_depth * 3.0f));

        // Update level
        m_waveParticleEmitter->SetLevel(m_currentLevel);

        // Ensure the splash begins the the same z-distance as the fragment.
        m_waveParticleEmitter->Start(hdVec3(point->position.x, point->position.y, fragment->GetWorldCenter().z));

        m_splashEmitter->SetNextSplashIsSmall(true);

        m_splashEmitter->Start(hdVec3(point->position.x, point->position.y, fragment->GetWorldCenter().z));

        if (playedLightSplashSound == false)
        {
            playedLightSplashSound = true;
            hdSound *sound = m_lightSplashSounds->GetItems()[rand() % m_lightSplashSounds->GetItemCount()];
            sound->volume = hdClamp(100.0f * fabs(point->body2->GetLinearVelocity().y), 0.4f, 1.0f);
            sound->pitchOffset = hdRandom(0.9f, 1.05f);
            hdSoundManager::PlaySound(sound);
        }
        fragment->m_interacts = false; // so we don't get multiple splashes
    }
}


void LevelController::HandleBlockBlockCollision(Block *block1, Block *block2, const ContactPoint *point)
{
    Joint *joint;
    hdAABB combinedAABB;
    unsigned i;

    /*
     * Reject collisions between two blocks sharing the same revolute joint
     */
    if (block1->ContainsGameFlag(e_hdkGameFlagsBlockHasRevJoint) &&
        block2->ContainsGameFlag(e_hdkGameFlagsBlockHasRevJoint))
    {
        // If there is a joint connect these two, then reject collision.
        for (i = 0; i < m_currentLevel->GetJointCount(); ++i)
        {
            joint = (Joint *)m_currentLevel->GetJoints()[i];
            if (joint->GetJointType() == e_hdkJointTypeRevolute &&
                joint->GetPhysicsJoint() != NULL &&
                joint->GetFirstBody() == block1 &&
                joint->GetSecondBody() == block2)
            {
                return;
            }
        }
    }


    if (block1 == m_floor)
    {
        this->HandleBlockLoss(hdVec3(point->position.x, point->position.y, 0.0f), block2);
        return;
    }
    else if (block1 == m_bottomBoundary)
    {
        this->HandleBlockWorldExit(hdVec3(point->position.x, point->position.y, 0.0f), block2);
        return;
    }

    if (block2 == m_floor)
    {
        this->HandleBlockLoss(hdVec3(point->position.x, point->position.y, 0.0f), block1);
        return;
    }
    else if (block2 == m_bottomBoundary)
    {
        this->HandleBlockWorldExit(hdVec3(point->position.x, point->position.y, 0.0f), block1);
        return;
    }

    PlayLoudCollisionSound(block1, point->body1);
    PlayLoudCollisionSound(block2, point->body2);


    /* Collision with the boundary means the block disappears */
    if (block1->IsGoal())
    {
        if (block2->GetMaterial() == e_hdkMaterialJack)
        {
            // Handle block to goal.
            Game_HandleGoalCollision(block1, block2, hdVec3(point->position.x, point->position.y, 0.0f));
            return;
        }
    }

    if (block2->IsGoal())
    {
        if (block1->GetMaterial() == e_hdkMaterialJack)
        {
            // Handle block to goal
            Game_HandleGoalCollision(block2, block1, hdVec3(point->position.x, point->position.y, 0.0f));
            return;
        }
    }

    if (block1->IsHazard() || block1->GetMaterial() == e_hdkMaterialGrinder)
    {
        if (block2->GetMaterial() != e_hdkMaterialBomb &&
            (block2->IsDestroyable() || block2->IsSpecial() || block2->IsDraggable()))
        {
            this->HandleBlockTap(block2, hdVec3(point->position.x, point->position.y, 0.0f), true);
            return;
        }
    }

    if (block2->IsHazard() || block2->GetMaterial() == e_hdkMaterialGrinder)
    {
        if (block1->GetMaterial() != e_hdkMaterialBomb &&
            (block1->IsDestroyable() || block1->IsSpecial() || block1->IsDraggable()))
        {
            this->HandleBlockTap(block1, hdVec3(point->position.x, point->position.y, 0.0f), true);
            return;
        }
    }

    // Destroy both blocks as long as the target is destroyable, not draggable.
    // No mutual bomb destruction allowed.

    if (block1->GetMaterial() == e_hdkMaterialBomb)
    {
        if (block2->GetMaterial() != e_hdkMaterialBomb &&
            (block2->IsDestroyable() || block2->IsSpecial()))
        {
            combinedAABB.lower = hdMin(block1->GetAABB().lower, block2->GetAABB().lower);
            combinedAABB.upper = hdMax(block1->GetAABB().upper, block2->GetAABB().upper);

            m_explosionEmitter->Start(combinedAABB);
            m_explosionSmokeEmitter->Start(combinedAABB);

            m_shotty->pitchOffset = 0.75f;
            hdSoundManager::PlaySound(m_shotty);

            this->HandleBlockTap(block1, hdVec3(point->position.x, point->position.y, 0.0f), true);
            this->HandleBlockTap(block2, hdVec3(point->position.x, point->position.y, 0.0f), true);
            return;
        }
    }

    if (block2->GetMaterial() == e_hdkMaterialBomb)
    {
        if (block1->GetMaterial() != e_hdkMaterialBomb &&
            (block1->IsDestroyable() || block1->IsSpecial()))
        {
            combinedAABB.lower = hdMin(block1->GetAABB().lower, block2->GetAABB().lower);
            combinedAABB.upper = hdMax(block1->GetAABB().upper, block2->GetAABB().upper);

            m_explosionEmitter->Start(combinedAABB);
            m_explosionSmokeEmitter->Start(combinedAABB);

            m_shotty->pitchOffset = 0.75f;
            hdSoundManager::PlaySound(m_shotty);

            this->HandleBlockTap(block1, hdVec3(point->position.x, point->position.y, 0.0f), true);
            this->HandleBlockTap(block2, hdVec3(point->position.x, point->position.y, 0.0f), true);
            return;
        }
    }


    if (block1->GetMaterial() == e_hdkMaterialPlastic)
    {
        if (block2->GetMaterial() != e_hdkMaterialPlastic &&
            (block2->IsDestroyable() || block2->IsSpecial()))
        {
            // if impact force was toward ground and with good enough speed, destroy block 1
            // ADDED: The contact point must also be ABOVE the plastic blocks center
            if (block2->GetPhysicsBody() != NULL &&
                block2->GetPhysicsBody()->GetLinearVelocity().y < 0.0f &&
                point->position.y > block1->GetWorldCenter().y &&
                hdFastHypotenuse(block2->GetPhysicsBody()->GetLinearVelocity().x, block2->GetPhysicsBody()->GetLinearVelocity().y) > 2.0f)
            {
                this->HandleBlockTap(block1, hdVec3(point->position.x, point->position.y, 0.0f), true);
            }
            return;
        }
    }

    if (block2->GetMaterial() == e_hdkMaterialPlastic)
    {
        if (block1->GetMaterial() != e_hdkMaterialPlastic &&
            (block1->IsDestroyable() || block1->IsSpecial()))
        {
            // if impact force was toward ground and with good enough speed, destroy block 1
            if (block1->GetPhysicsBody() != NULL &&
                block1->GetPhysicsBody()->GetLinearVelocity().y < 0.0f &&
                point->position.y > block2->GetWorldCenter().y &&
                hdFastHypotenuse(block1->GetPhysicsBody()->GetLinearVelocity().x, block1->GetPhysicsBody()->GetLinearVelocity().y) > 2.0f)
            {
                this->HandleBlockTap(block2, hdVec3(point->position.x, point->position.y, 0.0f), true);
            }
            return;
        }
    }
}



void LevelController::PlayLoudCollisionSound(const Block* block, const b2Body* body)
{
    float vol;
    hdSound *sound;


    sound = NULL;

    if (playedLoudCollisionSound == false)
    {
        vol = 0.2f + (0.5f * fabs(body->GetLinearVelocity().y) + fabs(body->GetLinearVelocity().x));
        //hdClamp(100.0f * (0.5f * fabs(body->GetLinearVelocity().y) + fabs(body->GetLinearVelocity().x)), 0.2f, 1.0f);

        if (vol > 0.25f)
        {
            switch(((Block *)block)->GetMaterial())
            {

                case e_hdkMaterialWood:
                    sound = m_blockCollisionSoundsWood->GetItems()[rand() % m_blockCollisionSoundsWood->GetItemCount()];
                    break;
                case e_hdkMaterialMetal:
                    sound = m_blockCollisionSoundsMetal->GetItems()[rand() % m_blockCollisionSoundsMetal->GetItemCount()];
                    break;
                case e_hdkMaterialRock:
                case e_hdkMaterialCustomTexture:
                    //sound = m_blockCollisionSoundsStone->GetItems()[rand() % m_blockCollisionSoundsStone->GetItemCount()];
                    //break;
                case e_hdkMaterialEarth:
                    //sound = m_blockCollisionSoundsStone->GetItems()[rand() % m_blockCollisionSoundsStone->GetItemCount()];
                    //break;
                default:
                    sound = m_blockCollisionSoundsStone->GetItems()[rand() % m_blockCollisionSoundsStone->GetItemCount()];
                    break;
            }
            if (sound == NULL) return;

            sound->volume = vol;
            sound->pitchOffset = hdRandom(0.8f, 1.2f);
            hdSoundManager::PlaySound(sound);
            playedLoudCollisionSound = true;
        }
    }

    Game_PlayLoudCollisionSound(block, body);
}


void LevelController::PlaySoftCollisionSound(const Block* block, const b2Body* body)
{
    if (playedSoftCollisionSound == false)
    {
        float vol = 0.05f + (0.2f * fabs(body->GetLinearVelocity().y) + fabs(body->GetLinearVelocity().x));
        vol = hdClamp(vol, 0.05f, 0.3f);

        if (vol > 0.1f)
        {
            hdSound *sound = m_blockCollisionSoundsWood->GetItems()[rand() % m_blockCollisionSoundsWood->GetItemCount()];
            if (sound == NULL) return;

            sound->volume = hdClamp(100.0f * fabs(body->GetLinearVelocity().y), 0.10f, 0.3f);
            sound->pitchOffset = hdRandom(0.85f, 1.1f);
            hdSoundManager::PlaySound(sound);
            playedSoftCollisionSound = true;
        }
    }
}


const hdSound* LevelController::GetCollisionSoundForMaterial(const e_hdkMaterial material) const
{
    hdSound *sound = NULL;

    switch(material)
    {
        case e_hdkMaterialBomb:
            sound = m_bombSelectSound;
            break;
        case e_hdkMaterialWood:
            sound = m_blockCollisionSoundsWood->GetItems()[rand() % m_blockCollisionSoundsWood->GetItemCount()];
            break;
        case e_hdkMaterialMetal:
            sound = m_blockCollisionSoundsMetal->GetItems()[rand() % m_blockCollisionSoundsMetal->GetItemCount()];
            break;
        case e_hdkMaterialRock:
            //sound = m_blockCollisionSoundsStone->GetItems()[rand() % m_blockCollisionSoundsStone->GetItemCount()];
            //break;
        case e_hdkMaterialGrinder:
        case e_hdkMaterialPlastic:
        case e_hdkMaterialRawPolygon:
        case e_hdkMaterialText:
        case e_hdkMaterialJack:
        case e_hdkMaterialEarth:
            //sound = m_blockCollisionSoundsStone->GetItems()[rand() % m_blockCollisionSoundsStone->GetItemCount()];
            sound = m_blockCollisionSoundsStone->GetItems()[rand() % m_blockCollisionSoundsStone->GetItemCount()];
            break;
        case e_hdkMaterialCustomTexture:
        default:
            sound = m_dragSelectSound;

            break;
    }
    return sound;
}


bool LevelController::AddBlockToCurrentLevel(Block *block)
{
    if (m_currentLevel == NULL) return false;

    if (-1 == m_currentLevel->AddBlock(block))
    {
        return false;
    }

    if (block->IsDraggable())
    {
        if (m_buoyancyController != NULL && block->GetPhysicsBody() != NULL)
        {
            m_buoyancyController->AddBody((b2Body*)block->GetPhysicsBody());
        }
    }

    Game_AddBlockToLevel(block);
    return true;

}


#define ROTATION_EXPERIMENT 0
void LevelController::PushProjectionMatrix()
{
    hdAssert(!m_pushedProjectionMatrix);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glPushMatrix();

    hdMatrix mProjection;

#if (TARGET_OS_IPHONE == 1) || (TARGET_IPHONE_SIMULATOR == 1)
    MatrixPerspectiveFovRH(mProjection, 90.0f*(hd_pi/180.0f), 1.0f/1.5f, 1.0f, 10000.0f, false);
#else
    MatrixPerspectiveFovRH(mProjection, 67.38f*(hd_pi/180.0f), 1.0f/m_ScreenAspectRatio, 1.0f, 10000.0f, false);
#endif
    glMultMatrixf(mProjection.f);

#if ROTATION_EXPERIMENT == 1
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
    //glPushMatrix();
    glPushMatrix();
    glRotatef(-10.0f, 0.0f, 1.0f, 0.0f);
    //	MatrixRotationXYZAxis(mRot, 0.25f, hdVec3(0,1,0));
    //	glMultMatrixf(mRot.f);
    glPushMatrix();
#endif

#if (TARGET_OS_IPHONE == 1) || (TARGET_IPHONE_SIMULATOR == 1)
    if (IsLandscapeOrientation())
    {
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        glTranslatef(-m_projection->GetWorldCenter().x, -m_projection->GetWorldCenter().y,
                     -((m_projection->GetAABB().upper.x - m_projection->GetAABB().lower.x)/2.0f));
    }
    else
    {
        glTranslatef(-m_projection->GetWorldCenter().x, -m_projection->GetWorldCenter().y,
                     -((m_projection->GetAABB().upper.y - m_projection->GetAABB().lower.y)/2.0f));
    }
#else
    glTranslatef(-m_projection->GetWorldCenter().x, -m_projection->GetWorldCenter().y,
                 -((m_projection->GetAABB().upper.x - m_projection->GetAABB().lower.x)/2.0f));
#endif

    hdglError("frustum");
    m_pushedProjectionMatrix = true;
}


void LevelController::PopProjectionMatrix()
{
    hdAssert(m_pushedProjectionMatrix);
    glMatrixMode(GL_PROJECTION);
#if ROTATION_EXPERIMENT == 1
    glPopMatrix();
    glPopMatrix();
#endif

    glPopMatrix();
    m_pushedProjectionMatrix = false;
}



void LevelController::DrawInternal()
{
    hdAABB reflectionAABB;
    Block *block;
    hdVec3 lo, hi;
    float width;

    PushProjectionMatrix();

    hdglError("A");
    hdglBindTexture(NULL);
    hdglError("B");

    if (m_currentLevel != NULL)
        m_currentLevel->Draw();

    reflectionAABB = m_currentLevel->GetScissorAABB(false);

    /*
     * Reflect our physics particles.
     */
    if (m_currentLevel->GetFloorInfo() != NULL &&
        m_currentLevel->GetFloorInfo()->m_isReflective &&
        m_reflectParticles &&
        m_physicsParticleEmitter->GetParticles()->GetItemCount() < 16 &&
        (fabs(reflectionAABB.upper.y - reflectionAABB.lower.y) > 0.0f))
    {
        GLfloat equation1[] = {0, -1.0f, 0, m_currentLevel->GetFloorDrawingLevel()};
        hdglClipPlanef(GL_CLIP_PLANE0, equation1);
        glEnable( GL_CLIP_PLANE0 );
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();

        glTranslatef(0.0f, -2.0f*(0.0f-m_currentLevel->GetFloorDrawingLevel()), 0.0f);
        glRotatef(180.0f, 1.0f, 0.0f, 0.0f);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        m_physicsParticleEmitter->SetViewportAABB(m_currentLevel->GetScissorAABB(false));
        m_physicsParticleEmitter->Draw();
        glDisable(GL_BLEND);
        glDisable(GL_DEPTH_TEST);
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
        glDisable( GL_CLIP_PLANE0 );
    }

    m_currentLevel->DrawFloor();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    m_physicsParticleEmitter->SetViewportAABB(m_currentLevel->GetScissorAABB(true));
    m_physicsParticleEmitter->Draw();
    glDisable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);

    /*
     * Lights
     */
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    for (int i = 0; i < GetCurrentLevel()->GetBlockCount(); i++)
    {
        block = GetCurrentLevel()->GetBlocks()[i];
        if (block->GetBlockType() == e_hdkBlockTypeLight)
        {
            // Draw a quad on top of the block repping the light.
            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslatef(0.0f, 0.0f, fabs(block->GetDepth())+0.05f);
            width = 5.0f * (block->GetStartingAABB().upper.x - block->GetStartingAABB().lower.x);
            lo = block->GetWorldCenter() - hdVec3(width, width, width);
            hi = block->GetWorldCenter() + hdVec3(width, width, width);
            m_glow->SetAs2DBox(lo, hi);
            m_glow->Draw();
            glMatrixMode(GL_MODELVIEW);
            glPopMatrix();
        }
    }
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);


    glDepthMask(GL_FALSE);
    glEnable(GL_DEPTH_TEST);
    //EnablePointSprite();


    m_splashEmitter->Draw();
    m_splinterEmitter->Draw();
    m_smokeEmitter->Draw();
    m_explosionSmokeEmitter->Draw();
    m_explosionEmitter->Draw();

    //m_gunSmokeEmitter->Draw();
    //m_gunSparkEmitter->Draw();
    //m_questionEmitter->Draw();
    m_waveParticleEmitter->Draw();
    //DisablePointSprite();
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);


#ifdef DEBUG2
    // Draw contact points.
    hdglBindTexture(NULL);
    glPointSize(5.0f);
    hdglBegin(GL_POINTS);
    for (int32 i = 0; i < m_contactCount; ++i)
    {
        hdglColor4f(1.0f, 1.0f, 1.0f, 0.75f);
        ContactPoint* point = m_contacts + i;
        hdglVertex3f(point->position.x, point->position.y, 0.0f);
    }
    hdglEnd();
#endif
    //glDisable(GL_LIGHTING);
    //glDisable(GL_COLOR_MATERIAL);
    PopProjectionMatrix();
}


void LevelController::DrawInternalInterface()
{
    m_uiMessageQueueContainer->Draw();

    if (IsLandscapeOrientation())
    {
        m_uiLandscapeMessageContainer->Draw();
        m_uiLandscapeMessageButtonContainer->Draw();
    }
    else
    {
        m_uiPortraitMessageContainer->Draw();
        m_uiPortraitMessageButtonContainer->Draw();
    }
}


void LevelController::DrawSelectedBlockHighlight()
{
    float len;
    hdAABB draggableAABB;

    if (m_rayQueryCount > 0)
    {
        hdglBindTexture(NULL);

        PushProjectionMatrix();


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

#if AABB
        // DEBUG ray test lines and aabbs.
        hdglBegin(GL_LINES);
        hdVec3 center = m_projection->GetWorldCenter();
        hdglVertex3f(center.x, center.y, center.z + ((m_projection->GetAABB().upper.y - m_projection->GetAABB().lower.y)/2.0f));
        hdglVertex3f(m_lastRayScreenPoint.x, m_lastRayScreenPoint.y, m_lastRayScreenPoint.z);
        hdglEnd();

        hdglBegin(GL_POINTS);
        glPointSize(10.0f);
        hdglColor4f(1.0f, 1.0f, 0.0f, 1.0f);
        hdglVertex3f(m_testPoint.x, m_testPoint.y, m_testPoint.z + 0.01f);
        hdglEnd();

        hdAABB rayAABB;
        rayAABB.lower.Set(m_lastRayScreenPoint.x - 1.0f, m_lastRayScreenPoint.y - 1.0f, -100.0f); // = hdMin(ray[0], ray[1]) - hdVec3(0.1f, 0.1f, 0.1f);
        rayAABB.upper.Set(m_lastRayScreenPoint.x + 1.0f, m_lastRayScreenPoint.y + 1.0f, 100.0f);

        UtilDrawAABB(rayAABB);
#endif

        if (m_currBlock != NULL)
        {
            if (m_currBlock->IsDraggable())
            {
                draggableAABB = (hdAABB)(m_currBlock->GetStartingAABB());
                len = fabs(hdFastHypotenuse(draggableAABB.Width(),
                                            draggableAABB.Height()));

                m_glow->SetAs2DBox(m_currBlock->GetWorldCenter().x - (0.8f * len),
                                   m_currBlock->GetWorldCenter().y - (0.8f * len),
                                   1.6f * len,
                                   1.6f * len);

                glEnable(GL_TEXTURE_2D);
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                glTranslatef(0.0f, 0.0f, -fabs(m_currBlock->GetDepth()));
                m_glow->Draw();
                glPopMatrix();
                glDisable(GL_TEXTURE_2D);
            }


            glEnable(GL_DEPTH_TEST);

            if (m_currBlock->IsDestroyable() &&
                (GetBlockRiskiness(m_currBlock, m_contacts, m_contactCount)/2) > 3)
            {
                m_currBlock->DrawHighlight(m_projection->GetWorldCenter(), hdColor4(127, 127, 255, 127));
            }
            else
            {
                m_currBlock->DrawHighlight(m_projection->GetWorldCenter(), hdColor4(255, 255, 0, 127));
            }
            glDisable(GL_DEPTH_TEST);
        }

        glDisable(GL_BLEND);


        PopProjectionMatrix();
    }
}



/*
 * The cunting interaction model is fucked up beyond all hope.
 *
 * I've managed to write about some solutions. What I've built
 * works quite well, but it will fall apart at the meerest addition
 * to the code below.
 */
void LevelController::HandleTapUp(float x, float y, int tapCount)
{
    hdReceiver *hideClicked;
    hdReceiver *queueClicked;
    hdVec2 screen;
    totemMessage msg;

    m_tapDown = false;

    if (Game_HandleTapUp(x, y, tapCount)) return;

    hideClicked = NULL;
    queueClicked = NULL;
    m_rayQueryCount = 0;

    ConvertRawToScreen(screen, x, y);
    m_uiMessageButtonContainer->MouseUp(screen.x, screen.y);
    m_uiMessageQueueContainer->MouseUp(screen.x, screen.y);

    if (hideClicked == m_DoneButton ||
        hideClicked == m_OkButton ||
        hideClicked == m_NextButton)
    {
        hdSoundManager::PlaySound(m_btnMessageClickUpSound);
        HideMessage();
        //SetSelectedTotemBlock(NULL);
    }
    else if (m_SkipButton == hideClicked)
    {
        m_messageCount = 0;
        hdSoundManager::PlaySound(m_btnMessageClickUpSound);
        HideMessage();
    }
    else if (hideClicked == m_PrevButton)
    {
        if (m_currentMessageIndex >= 2)
        {
            hdSoundManager::PlaySound(m_btnMessageClickUpSound);
            m_currentMessageIndex = hdMax(0, m_currentMessageIndex-2); // because the index is incremented
            HideMessage();
        }
    }
    else if (queueClicked == m_uiMessageQueueButton)
    {
        hdSoundManager::PlaySound(m_btnMessageClickUpSound);

        hdAssert (m_messageTagQueue->GetItemCount() > 0);

        msg = m_messageTagQueue->GetItems()[0];
        if (msg.levelId == -1)
        {
            LoadScriptMessageForTag(msg.targetId, false);
        }
        else
        {
            LoadScriptMessageForTagAndLevel(msg.targetId, msg.levelId, false);
        }
        m_messageTagQueue->RemoveAtIndex(0);

        if (m_messageTagQueue->IsEmpty())
        {
            m_uiMessageQueueContainer->Hide();
        }
        else
        {
            m_uiMessageQueueCount->SetTextFormatted("%d", m_messageTagQueue->GetItemCount());
        }
    }
    else if (m_interactionState != e_interactionDisabled)
    {
        if (m_mouseJoint != NULL)
        {
            DestroyMouseJoint();
            return;
        }
        else if (m_interactionState == e_interactionStateSelecting)
        {
            Block *curr = m_currBlock;

            // If nothing was selected on this up event, we still want to
            // check if something was selected on the down event. If there
            // was something, we execute the tap up event on this older
            // object if only a short amount of time elapsed between the down
            // and up events.
            //
            // The motivation for this is to detect taps on blocks that are
            // moving quickly and have moved away from the player's finger before
            // the player raises it.
            //
            // While testing, players would feel cheated if they felt they accurately
            // tapped the block but didn't get any result. This little change
            // massively improves the playability of the game.
            if (m_currBlockTicksSinceTapDown < 10)
            {
                m_currBlock = curr;
                this->HandleBlockTap(m_currBlock, m_testPoint, false);
            }
            else
            {
                this->SelectBlockAtScreenPoint(screen.x, screen.y);
                if (curr!= NULL && curr == m_currBlock)
                {
                    this->HandleBlockTap(m_currBlock, m_testPoint, false);
                }
                else
                {
                    m_currBlock = NULL;
                }
            }
        }
        if ((m_currBlock == NULL ||
             Game_CanSelectBlock(m_currBlock)) &&
            !IsShowingMessage())
            m_interactionState = e_waitingForInput;

        m_currBlock = NULL;
    }
}



void LevelController::HandleTapDown(float x, float y, int tapCount)
{
    hdVec2 screen;
    bool selected;

    m_tapDown = true;

    if (Game_HandleTapDown(x, y, tapCount)) return;

    ConvertRawToScreen(screen, x, y);

    if (m_DoneButton->MouseDown(screen.x, screen.y) ||
        m_SkipButton->MouseDown(screen.x, screen.y) ||
        m_NextButton->MouseDown(screen.x, screen.y) ||
        m_PrevButton->MouseDown(screen.x, screen.y) ||
        m_OkButton->MouseDown(screen.x, screen.y))
    {
        hdSoundManager::PlaySound(m_btnMessageClickDownSound);
    }
    else if (m_uiMessageQueueContainer->MouseDown(screen.x, screen.y))
    {
        hdSoundManager::PlaySound(m_btnMessageClickDownSound);
    }
    else if (m_interactionState != e_interactionDisabled)
    {
        if (m_interactionState == e_waitingForInput)
        {
            selected = this->SelectBlockAtScreenPoint(e_hdInterfaceClickStateDown, screen.x, screen.y);

            if (selected)
            {
                m_interactionState = e_interactionStateSelecting;
            }
            else
            {
                return;
            }

            if (m_currBlock != NULL &&
                Game_CanSelectBlock(m_currBlock) &&
                m_currBlock->IsVisible())
            {
                // distance from screen center.
                m_distanceFromScreenCenter = m_currBlock->GetWorldCenter() -
                m_projection->GetWorldCenter();

                Game_hdkBlockWasSelected(m_currBlock);

                // This tick counter time stamp will be reset on tap up.
                m_currBlockTicksSinceTapDown = 0;

                m_blockDeselectThreshold.lower.Set(screen.x-40.0f, screen.y-40.0f, 0.0f);
                m_blockDeselectThreshold.upper.Set(screen.x+40.0f, screen.y+40.0f, 0.0f);

                m_interactionState = e_interactionStateSelecting;
#ifdef DEBUG
                hdPrintf("[LevelController::HandleTapDown] Block was selected.\n");
#endif
                // Fire the event if we tapped an event sender
                if (m_currBlock->IsEventSender())
                {
                    Event *ev = NULL;
                    for (int i = 0; i < m_currentLevel->GetEvents()->GetItemCount(); ++i)
                    {
                        ev = m_currentLevel->GetEvents()->GetItems()[i];
                        if (ev->GetFirstObject() == (hdGameObject *)m_currBlock)
                        {
                            HandleSenderBlockTapDownEvent(m_currBlock, ev);
                        }
                    }
                    return;
                }

            }
        }
    }
}


void LevelController::HandleSingleTap(float x, float y)
{
    Game_HandleSingleTap(x, y);
}


void LevelController::HandleDoubleTap(float x, float y)
{
    Game_HandleDoubleTap(x, y);
}


void LevelController::HandleTapMovedSingle(const float previousX, const float previousY,
                                                const float currentX, const float currentY)
{
    if (m_interactionState == e_interactionDisabled) return;

    if (Game_HandleTapMovedSingle(previousX, previousY, currentX, currentY)) return;

    hdVec2 screen;
    ConvertRawToScreen(screen, currentX, currentY);

    if (m_currBlock != NULL)
    {
        hdVec2 pp;
        this->ConvertScreenToInterface(pp, screen.x, screen.y);

        if (m_mouseJoint != NULL)
        {

            UpdateMouseJoint(pp.x, pp.y);
            return;
        }
        else
        {
            CreateMouseJoint();
        }

        /*
         If the player moves their finger outside the block deselect threshold then
         we deselect the current block - this allows players to pan around more
         easily without having to explicitly avoid putting their fingers on blocks
         and getting accidental explosions.
         */
        if (!m_currBlock->IsDraggable() &&
            m_mouseJoint == NULL &&
            m_interactionState == e_interactionStateSelecting)
        {
            if (!hdPointInsideAABB(hdVec2toVec3(screen), m_blockDeselectThreshold))
            {
                m_currBlock = NULL;
                m_interactionState = e_panAndZoom;
            }
        }
    }
    else
    {
        m_interactionState = e_panAndZoom;
        this->PanProjection(previousX, previousY, currentX, currentY);
        m_uiMessageQueueContainer->MouseOver(screen.x, screen.y);
    }

}



void LevelController::HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
                                                const float aCurrentX, const float aCurrentY,
                                                const float bPreviousX, const float bPreviousY,
                                                const float bCurrentX, const float bCurrentY)
{
    if (m_interactionState == e_interactionDisabled) return;
    m_currBlock = NULL;
    this->ZoomProjection(aPreviousX, aPreviousY, aCurrentX, aCurrentY,
                         bPreviousX, bPreviousY, bCurrentX, bCurrentY);
}



void LevelController::HandleOrientationChanged()
{
    this->RefreshLayout();
    Game_HandleOrientationChanged();
    PanProjection(0, 0, 0.1f, 0.1f);
}


bool LevelController::SelectBlockAtScreenPoint(float x, float y)
{
    return SelectBlockAtScreenPoint(e_hdInterfaceClickStateUp, x, y);
}


bool LevelController::SelectBlockAtScreenPoint(e_hdInterfaceClickState clickState, float x, float y)
{
    hdVec2 pp;
    bool particleFound;
    hdVec3 ray[2];
    hdVec3 center;
    const int k_maxCount = 10;
    hdGameObject* objs[k_maxCount];
    hdGameObject* top;

    this->ConvertScreenToInterface(pp, x, y);
    center = m_projection->GetWorldCenter();
    ray[0].Set(center.x, center.y, center.z + ((m_projection->GetAABB().upper.y - m_projection->GetAABB().lower.y)/2.0f));
    ray[1].Set(pp.x, pp.y, 0.0f);
    m_lastRayScreenPoint.Set(pp.x, pp.y, 0.0f);

    if (clickState == e_hdInterfaceClickStateDown)
    {
        m_currBlock = NULL;
        top = NULL;
        particleFound = false;

        m_rayQueryCount = m_gameWorld->RayQuery((unsigned int)e_hdkTypeBlock |
                                                (unsigned int)kPhysicsParticleUserType,
                                                ray, objs, k_maxCount, m_testPoint);

        // If first is a selectable block, then set curr_block = top and break.
        // If a particle, call HandleParticleTap until we get a block.
        if (m_rayQueryCount == 0) return false;

        for (int i = 0; i < hdMin(m_rayQueryCount, k_maxCount); ++i)
        {
            if (objs[i]->GetUserType() == (int)kPhysicsParticleUserType)
            {
                break;
            }
            else if (objs[i]->GetUserType() == (int)e_hdkTypeBlock &&
                     Game_CanSelectBlock((Block*)objs[i]))
            {
                top = objs[i];
                break;
            }
        }

        if (top != NULL)
        {
            m_currBlock = (Block*)top;
            return true;
        }

        for (int i = 0; i < hdMin(m_rayQueryCount, k_maxCount); ++i)
        {
            if (objs[i]->GetUserType() == (int)kPhysicsParticleUserType)
            {
                HandleParticleTap((hdPhysicsParticle *)objs[i], m_testPoint, false);
                particleFound = true;
            }
            else if (objs[i]->GetUserType() == (int)e_hdkTypeBlock &&
                     Game_CanSelectBlock((Block*)objs[i]))
            {
                break;
            }
        }
        return particleFound;
    }
    else
    {
        m_rayQueryCount = m_gameWorld->RayQuery((unsigned int)e_hdkTypeBlock, ray, objs, k_maxCount, m_testPoint);

        for (int i = 0; i < hdMin(m_rayQueryCount,k_maxCount); ++i)
        {
            if (objs[i]->GetUserType() == (int)e_hdkTypeBlock)
            {
                if (Game_CanSelectBlock((Block*)objs[i]))
                {
                    m_currBlock = (Block*)objs[i];
                    return true;
                }
            }
        }
        m_currBlock = NULL;
        return false;
    }
}


// TODO: A collision action depends entirely on what kind of game is loaded
void LevelController::HandleBlockLoss(const hdVec3& contactPoint, Block* block)
{
#ifdef DEBUG2
    hdPrintf("[I] LevelController::HandleBlockLoss contact point at (%3.5f, %3.5f, %3.5f)", contactPoint.x, contactPoint.y, contactPoint.z);
#endif

    // A block that is already "lost" cannot be lost again.
    if (block->ContainsGameFlag(e_hdkGameFlagsBlockLost)) return;
    if (block->GetPhysicsBody() == NULL) return;

    if (m_currentLevel->GetFloorInfo()->m_isWater)
    {
        m_waveParticleEmitter->SetParticlePointSize(hdMax(1.2f, fabs(block->GetDepth()*3.0f)));
        m_waveParticleEmitter->SetLevel(m_currentLevel);
        m_waveParticleEmitter->Start(contactPoint + hdVec3(0, 0.02f, 0));

        if (block->GetPhysicsBody()->GetLinearVelocity().y < -4.0f)
        {
            m_splashEmitter->SetNextSplashIsSmall(false);
            m_splashEmitter->Start(contactPoint.x, contactPoint.y);

            if (!playedHeavySplashSound)
            {
                hdSoundManager::PlaySound(m_blockLossSounds->GetItems()[rand() % m_blockLossSounds->GetItemCount()]);
                playedHeavySplashSound = true;
            }
        }
    }

    // Convert the block into a particle, and then destroy it.
    // The particle ensures that the block fades out and disappears on loss.
    if (Game_HandleBlockLoss(block, contactPoint))
    {
        block->Step();
    }
}


void LevelController::HandleBlockWorldExit(const hdVec3& contactPoint, Block* block)
{
    if (block->ContainsGameFlag(e_hdkGameFlagsBlockLost))
    {
        block->DestroyPhysicsBody();
        m_gameWorld->RemoveGameObject(block);
    }
}


void LevelController::CreateMouseJoint()
{
    if (m_mouseJoint != NULL) return;

    if (m_currBlock->IsDraggable() && (NULL != m_currBlock->GetPhysicsBody()))
    {
        if (m_HACK_lastJointedBlock != NULL &&
            m_HACK_lastJointedBlock->GetPhysicsBody() != NULL)
        {
            // Fixed rot not always reset; program crashes due to assertion
            // at line 75 on b2RevoluteJoint.cpp
            ((b2Body*)m_HACK_lastJointedBlock->GetPhysicsBody())->SetFixedRotation(false);
            m_HACK_lastJointedBlock = NULL;
        }

        b2MouseJointDef md;
        md.body1 = (b2Body*)m_topBlock->GetPhysicsBody();
        md.body2 = (b2Body*)m_currBlock->GetPhysicsBody();
        if (m_currBlock->GetTag() == 0 || m_currBlock->GetTag() == 1001)
        {
            md.body2->SetFixedRotation(true);
        }

        if (m_currBlock->GetMaterial() == e_hdkMaterialBomb)
        {
            md.target = b2Vec2(m_currBlock->GetWorldCenter().x, m_currBlock->GetWorldCenter().y-1.25f);
        }
        else
        {
            md.target = b2Vec2(m_currBlock->GetWorldCenter().x, m_currBlock->GetWorldCenter().y-0.25f);
        }

        md.maxForce = 80.0f * (m_currBlock->GetPhysicsBody())->GetMass();
        m_mouseJoint = (b2MouseJoint*)m_physicsWorld->CreateJoint(&md);
        md.body2->WakeUp();

        m_HACK_lastJointedBlock = m_currBlock;
    }
}


void LevelController::UpdateMouseJoint(float x, float y)
{
    if (m_mouseJoint == NULL) return; // callers responsible for checking first.
    if (m_currBlock == NULL) return;

    m_mouseJoint->SetTarget(b2Vec2(x, y));
}


void LevelController::DestroyMouseJoint()
{
    if (m_mouseJoint == NULL) return; // callers responsible for checking first.

    m_physicsWorld->DestroyJoint(m_mouseJoint);
    m_mouseJoint = NULL;
    m_interactionState = e_waitingForInput;

    if (m_currBlock &&
        m_currBlock->GetPhysicsBody() &&
        ((b2Body*)m_currBlock->GetPhysicsBody())->IsFixedRotation())
    {
        if (m_currBlock->GetTag() == 0 || m_currBlock->GetTag() == 1001)
        {
            ((b2Body*)m_currBlock->GetPhysicsBody())->SetFixedRotation(false);
        }
    }
}


void LevelController::HandleSenderBlockTapDownEvent(const Block* sender, Event* ev)
{
    Joint *receiver;
    switch(ev->GetEventType())
    {
        case e_hdkEventTypeFireGun:
            break;
        case e_hdkEventTypeShowMessage:
            break;
        case e_hdkEventTypeJointMotorStartPos:
            if (ev->GetSecondObject()->GetUserType() == (int)e_hdkTypeJoint)
            {
                receiver = (Joint *)ev->GetSecondObject();
                JointMotorStart(receiver, true);
            }
            break;
        case e_hdkEventTypeJointMotorStartNeg:
            if (ev->GetSecondObject()->GetUserType() == (int)e_hdkTypeJoint)
            {
                receiver = (Joint *)ev->GetSecondObject();
                JointMotorStart(receiver, false);
            }
            break;
        case e_hdkEventTypeJointMotorStop:

            break;
        default:
            break;
    }
}


void LevelController::HandleSenderBlockTapUpEvent(const Block* sender, Event* ev)
{
    switch(ev->GetEventType())
    {
        case e_hdkEventTypeFireGun:
        {
            // The second body emits a bullet from the world center
            // in the direction of the axis of rotation.

            Block *receiver = (Block *)ev->GetSecondObject();

            hdVec3 center = receiver->GetWorldCenter();
            hdVec3 line(0.0f, ((receiver->GetAABB().upper.y - receiver->GetAABB().lower.y)/2.0f), 0.0f);
            hdVec3 out;
            hdMatrix rotMatrix;

            MatrixRotationZ(rotMatrix, receiver->GetOBB().transform.rotation.z);
            MatrixVec3Multiply(out, line, rotMatrix);

            hdVec2 aa, bb;
            aa.Set(center.x + out.x - 0.05f, center.y + out.y - 0.05f);
            bb.Set(center.x + out.x + 0.05f, center.y + out.y + 0.05f);

            Block *block = new Block(m_gameWorld, m_physicsWorld, aa, bb, e_hdkMaterialMetal,
                                               e_hdkShapeTypeCylinder, e_hdkBlockTypeHazardDest);
            m_currentLevel->AddBlock(block);
            block->SetDepth(-0.05f);
            b2Body *body = (b2Body *)block->GetPhysicsBody();

            body->ApplyForce(b2Vec2(out.x * 250.0f, out.y * 250.0f), body->GetWorldCenter());
            body->SetBullet(true);

            m_gunSmokeEmitter->Start(block->GetWorldCenter().x, block->GetWorldCenter().y);
            m_gunSparkEmitter->SetDirection(out.x, out.y, out.z);
            m_gunSparkEmitter->Start(block->GetWorldCenter().x, block->GetWorldCenter().y);

            hdSoundManager::PlaySound(m_shotty);
        }
            break;
        case e_hdkEventTypeShowMessage:
            // Get the receiver, and its texture
            // Set the UIMessageboard texture to this, and then show
            // Game must be paused!
        {
            Block *receiver = (Block *)ev->GetSecondObject();

            if (receiver == NULL) return;

            m_interactionState = e_interactionDisabled;
            SetMessage(receiver->GetTexture());
            this->ShowMessage();
            m_questionEmitter->m_onStopResume = false;
        }
            break;
        case e_hdkEventTypeJointMotorStartPos:
        case e_hdkEventTypeJointMotorStartNeg:
        case e_hdkEventTypeJointMotorStop:
        {
            Joint *receiver = (Joint *)ev->GetSecondObject();
            JointMotorStop(receiver);
        }
            break;
        default:
            break;
    }
}



void LevelController::ShowMessage()
{
    float longSide;
    float shortSide;

    if (m_uiLandscapeMessageContainer->IsVisible() &&
        m_uiPortraitMessageContainer->IsVisible()) return;

    m_uiLandscapeMessageContainer->Show();
    m_uiPortraitMessageContainer->Show();

    longSide = hdMax(m_PixelScreenWidth, m_PixelScreenHeight);
    shortSide = hdMin(m_PixelScreenWidth, m_PixelScreenHeight);

    ShowMessageInterface(m_uiLandscapeMessageContainer, m_uiLandscapeMessageButtonContainer, longSide, shortSide);
    ShowMessageInterface(m_uiPortraitMessageContainer, m_uiPortraitMessageButtonContainer, shortSide, longSide);

    Game_ShowMessage();


}


void LevelController::HideMessage()
{
    float longSide;
    float shortSide;

    if (m_uiLandscapeMessageContainer->IsHidden() &&
        m_uiPortraitMessageContainer->IsHidden()) return;

    hdAnimation* anim = hdAnimationController::CreateAnimation(this);

    hdPauseAction *action = new hdPauseAction();
    action->SetDuration(0.3f);

    anim->AddGameObject(m_uiLandscapeMessageContainer);
    anim->AddAction(action);
    anim->SetFinishedCallback(this, LevelController::HideMessageCallback);
    anim->StartAnimation();

    longSide = hdMax(m_PixelScreenWidth, m_PixelScreenHeight);
    shortSide = hdMin(m_PixelScreenWidth, m_PixelScreenHeight);

    HideMessageInterface(m_uiLandscapeMessageContainer, m_uiLandscapeMessageButtonContainer, longSide, shortSide);
    HideMessageInterface(m_uiPortraitMessageContainer, m_uiPortraitMessageButtonContainer, shortSide, longSide);
}


void LevelController::ShowMessageInterface(hdUIContainer *container, hdUIContainer *buttonContainer,
                                                const float screenWidth, const float screenHeight)
{
    hdAnimation* anim1 = hdAnimationController::CreateAnimation(this);
    hdAnimation* anim2 = hdAnimationController::CreateAnimation(this);

    hdVectorAction *action1 = new hdVectorAction();
    action1->SetDuration(0.15f);
    action1->SetDestination(hdVec3(container->GetWorldCenter().x + screenWidth,
                                   container->GetWorldCenter().y,
                                   container->GetWorldCenter().z));

    anim1->AddGameObject(container);
    anim1->AddAction(action1);
    anim1->StartAnimation();

    hdVectorAction *action2 = new hdVectorAction();
    action2->SetDuration(0.25f);
    action2->SetDestination(hdVec3(buttonContainer->GetWorldCenter().x + screenWidth,
                                   buttonContainer->GetWorldCenter().y,
                                   buttonContainer->GetWorldCenter().z));

    anim2->AddGameObject(buttonContainer);
    anim2->AddAction(action2);
    anim2->StartAnimation();
}


void LevelController::HideMessageInterface(hdUIContainer *container, hdUIContainer *buttonContainer,
                                                const float screenWidth, const float screenHeight)
{
    hdAnimation* anim1 = hdAnimationController::CreateAnimation(this);
    hdAnimation* anim2 = hdAnimationController::CreateAnimation(this);

    hdVectorAction *action1 = new hdVectorAction();
    action1->SetDuration(0.25f);
    action1->SetDestination(hdVec3(container->GetWorldCenter().x - screenWidth,
                                   container->GetWorldCenter().y, 
                                   container->GetWorldCenter().z));
    
    anim1->AddGameObject(container);
    anim1->AddAction(action1);
    //anim1->SetFinishedCallback(this, LevelController::HideMessageCallback);
    anim1->StartAnimation();
    
    hdVectorAction *action2 = new hdVectorAction();
    action2->SetDuration(0.15f);
    action2->SetDestination(hdVec3(buttonContainer->GetWorldCenter().x - screenWidth, 
                                   buttonContainer->GetWorldCenter().y, 
                                   buttonContainer->GetWorldCenter().z));
    
    anim2->AddGameObject(buttonContainer);
    anim2->AddAction(action2);
    anim2->StartAnimation();
}


void LevelController::HideMessageCallback(void *handler, hdAnimation *anim)
{
    if (handler == NULL || anim == NULL) 
    {
        hdError(-1, "Fucking animation and handler have gone away in between anim starting and anim stopping.");
        return;
    }
    
    LevelController *self = (LevelController *)handler;
    
    self->Game_HideMessage();
    
    self->m_uiLandscapeMessageContainer->Hide();
    self->m_uiPortraitMessageContainer->Hide();
    self->ShowNextScriptMessage();
}


void LevelController::SetMessageTexture(const char *texturePrefix)
{
    //char landscapeTextureName[256];
    //char portraitTextureName[256];
    
    //snprintf(landscapeTextureName, 256, "%s_L.tga", texturePrefix);
    //snprintf(portraitTextureName, 256, "%s_P.tga", texturePrefix);
    
    m_uiLandscapeTextMessageBackground->SetTexture(hdTextureManager::Instance()->FindTexture(texturePrefix, TT_Wall));
    m_uiLandscapeTextMessageBackground->Show();
    
    m_uiPortraitTextMessageBackground->SetTexture(hdTextureManager::Instance()->FindTexture(texturePrefix, TT_Wall));
    m_uiPortraitTextMessageBackground->Show();
}


void LevelController::SetMessage(const hdTexture *texture)
{
    m_uiLandscapeTextMessageBackground->SetTexture(texture);
    m_uiLandscapeTextMessageBackground->Show();
    
    m_uiPortraitTextMessageBackground->SetTexture(texture);
    m_uiPortraitTextMessageBackground->Show();
    
    m_uiLandscapeTextMessage->Hide();
    m_uiPortraitTextMessage->Hide();
}


void LevelController::SetMessage(const totemMessage *msg)
{
    m_uiLandscapeAvatar->Hide();
    m_uiLandscapeAvatarChrome->Hide();
    m_uiPortraitAvatar->Hide();
    m_uiPortraitAvatarChrome->Hide();
    
    m_uiLandscapeTextMessage->Hide();
    m_uiLandscapeTextMessageBackground->Hide();
    
    m_uiPortraitTextMessage->Hide();
    m_uiPortraitTextMessageBackground->Hide();
    
    m_uiLandscapeAvatarMessageText->Hide();
    m_uiLandscapeAvatarMessageChrome->Hide();
    m_uiLandscapeAvatarMessageBackground->Hide();
    m_uiPortraitAvatarMessageText->Hide();
    m_uiPortraitAvatarMessageChrome->Hide();
    m_uiPortraitAvatarMessageBackground->Hide();
    
    m_uiLandscapeTitleMessageBackground->Hide();
    m_uiLandscapeTitleMessageFont->Hide();
    m_uiPortraitTitleMessageBackground->Hide();
    m_uiPortraitTitleMessageFont->Hide();
    
    if (msg->messageType == e_hdkMessageTypeImage)
    { 
        // Assume text is a texture
        SetMessageTexture(msg->texture);
        
        m_uiLandscapeTextMessage->Hide();
        
        m_uiPortraitTextMessage->Hide();
    }
    else if (msg->messageType == e_hdkMessageTypeTitle)
    {
        m_uiLandscapeTitleMessageFont->SetText(msg->message);
        m_uiLandscapeTitleMessageFont->Show();
        m_uiLandscapeTitleMessageBackground->Show();
        
        m_uiPortraitTitleMessageFont->SetText(msg->message);
        m_uiPortraitTitleMessageFont->Show();
        m_uiPortraitTitleMessageBackground->Show();
    }
    else if (msg->messageType == e_hdkMessageTypeText)
    {
        SetMessageTexture("Interface/imgScriptMessage");
        
        m_uiLandscapeTextMessage->SetText(msg->message);
        m_uiLandscapeTextMessage->Show();
        
        m_uiPortraitTextMessage->SetText(msg->message);
        m_uiPortraitTextMessage->Show();
    }
    else if (msg->messageType == e_hdkMessageTypeCustomImageText)
    {
        SetMessageTexture(msg->texture);
        
        m_uiLandscapeTextMessage->SetText(msg->message);
        m_uiLandscapeTextMessage->Show();
        
        m_uiPortraitTextMessage->SetText(msg->message);
        m_uiPortraitTextMessage->Show();
    }
    else if (msg->messageType == e_hdkMessageTypeAvatar)
    {
        m_uiLandscapeAvatar->SetTexture(hdTextureManager::Instance()->FindTexture(msg->texture, TT_Wall));
        m_uiLandscapeAvatar->Show();
        m_uiLandscapeAvatarChrome->Show();
        
        m_uiPortraitAvatar->SetTexture(hdTextureManager::Instance()->FindTexture(msg->texture, TT_Wall));
        m_uiPortraitAvatar->Show();
        m_uiPortraitAvatarChrome->Show();
        
        m_uiLandscapeAvatarMessageText->SetText(msg->message);
        m_uiLandscapeAvatarMessageText->Show();
        m_uiLandscapeAvatarMessageChrome->Show();
        m_uiLandscapeAvatarMessageBackground->Show();
        
        m_uiPortraitAvatarMessageText->SetText(msg->message);
        m_uiPortraitAvatarMessageText->Show();
        m_uiPortraitAvatarMessageChrome->Show();
        m_uiPortraitAvatarMessageBackground->Show();
        
    }
}


// User destroyed a block!
// - Decrement the block counter
// - play physics emitter   
// - play volumetric material particle emitter.
// - remove physics and game object from world.
// - Forward tap info to game handler.
void LevelController::HandleBlockTap(Block* block, const hdVec3& impactPoint, bool hazardCollision)
{
    polygon_info_t polygonInfo;
    
    Event *ev;
    uint32 i;
    
    hdSound *sound;
    
    hdAABB startAABB;
    hdVec3 center, orig, trans;
    
    
    if (block == NULL) return;
    
    // Event senders send special events; other blocks can send messages
    // if there's something in the scripts file for it.
    if (block->IsEventSender() && false == hazardCollision)
    {
        ev = NULL;
        for (i = 0; i < m_currentLevel->GetEvents()->GetItemCount(); ++i)
        {
            if (m_currentLevel->GetEvents()->GetItems()[i]->GetFirstObject() == block)
            {
                ev = m_currentLevel->GetEvents()->GetItems()[i];
                HandleSenderBlockTapUpEvent(block, ev);
            }
        }
        if (ev != NULL) return;
    }
    
    if (block->GetTag() > 0 && false == hazardCollision)
    {
        LoadScriptMessageForTag(block->GetTag(), false);
    }
    
    if (block->GetPhysicsBody() == NULL)
    {
        return;
    }
    
    
    if (!Game_HandleBlockTap(block, impactPoint, hazardCollision)) return;
    
    
    if (m_mouseJoint != NULL)
        DestroyMouseJoint();
    
    /* In case the block is off screen and has not been updated in a while */
    if (hazardCollision)
        block->Step(); 
    
    
    // Destroy block physics info so it no longer affects the world.
    block->DestroyPhysicsBody(); 
    m_gameWorld->RemoveGameObject(block);
    
    if (playedBlockSmashSound == false)
    {
        sound = m_blockExplosionSoundsStone->GetItems()[rand() % m_blockExplosionSoundsStone->GetItemCount()];
        sound->volume = 0.9f;
        sound->pitchOffset = hdRandom(0.85f, 1.15f);
        hdSoundManager::PlaySound(sound);
        playedBlockSmashSound = true;
    }
    
    /*
     * Bombs do their own thing
     */
    if (block->GetMaterial() == e_hdkMaterialBomb) return;
    
    /*
     * Smash the block...
     */
    {
        /*
         * Find the impact point as if it were on the original block.
         */
        startAABB = block->GetStartingAABB();
        center = startAABB.lower + (0.5f * (startAABB.upper - startAABB.lower));
        
        orig.Set(impactPoint.x, impactPoint.y, impactPoint.z);
        trans = block->GetWorldCenter() - center;
        
        hdTranslateVertices(&orig, 1, -(block->GetWorldCenter()));
        hdRotateVertices(&orig, 1, -block->GetOBB().transform.rotation);
        hdTranslateVertices(&orig, 1, block->GetWorldCenter());
        
        startAABB.lower = startAABB.lower + trans;
        startAABB.upper = startAABB.upper + trans;
        polygonInfo.impactTexCoord = block->GetTextureCoordForPoint(startAABB, orig);
        
        
        polygonInfo.texture = (hdTexture *)block->GetTexture();
        
        polygonInfo.tint[0] = block->GetTint()[0]; 
        polygonInfo.tint[1] = block->GetTint()[1];
        polygonInfo.tint[2] = block->GetTint()[2];
        polygonInfo.tint[3] = block->GetTint()[3];
        
        polygonInfo.vertices = block->GetVertices();
        polygonInfo.startingVertices = block->GetStartingVertices();
        polygonInfo.texCoords = block->GetTextureCoordinates();
        polygonInfo.vertexCount = block->GetVertexCount();
        
        polygonInfo.worldCenter = block->GetWorldCenter();
        polygonInfo.impactPoint = impactPoint;
        polygonInfo.depth = block->GetDepth();
        polygonInfo.zOffset = block->GetZOffset();
        polygonInfo.parentType = block->GetUserType();
        polygonInfo.parentTag = block->GetMaterial();
        
        m_physicsParticleEmitter->StartWithTexture(polygonInfo);
    }
    
    hdAABB blockAABB;
    blockAABB.lower = (block->GetDepthAABB().lower + hdVec3(0.1f, 0.1f, 0.1f));
    blockAABB.upper = (block->GetDepthAABB().upper - hdVec3(0.1f, 0.1f, 0.1f));
    
    m_smokeEmitter->Start(blockAABB);
    
    m_splinterEmitter->SetParticlePointSize(fabs(block->GetDepth()));
    m_splinterEmitter->SetParticleColor(hdColor4(block->GetTint()[0], block->GetTint()[1], block->GetTint()[2], 1.0f));
    m_splinterEmitter->Start(block->GetTexture(), blockAABB);
    
}


void LevelController::HandleParticleTap(hdPhysicsParticle* particle, const hdVec3& impactPoint, bool hazardCollision)
{
    polygon_info_t polygonInfo;
    
    /*
     * Game unit and texture unit calculations.
     */
    hdAABB texAABB;
    hdVec3 vDiff, diff;
    hdVec2 tDiff;
    
    float tpgx, tpgy;
    
    /*
     * Interaction can be turned off - don't allow taps.
     */
    if (false == particle->m_interacts) return;
    
    /*
     * Need a physics body
     */
    if (particle->m_physicsBody == NULL) return;
    
    /*
     * TODO: Fade out threshold so we don't get the somewhat distrubing effect
     * of resurrecting fading particles.
     */
    if (particle->life <= 0.0f) return;
    
    /*
     * Fading particles don't get destroyed by hazard collisions.
     */
    if (hazardCollision && (particle->m_duration - particle->life) < 0.25f) return;
    
    if (Game_HandleParticleTap(particle, impactPoint, hazardCollision))
    {
        //particle->Step();
        
        if (!hdPolygonContainsPoint(particle->GetVertices(), 
                                    particle->GetVertexCount(),
                                    impactPoint))
        {
            polygonInfo.impactPoint.Set(particle->GetWorldCenter().x, 
                                        particle->GetWorldCenter().y,
                                        particle->GetWorldCenter().z);
        }
        else
        {
            polygonInfo.impactPoint.Set(impactPoint.x, 
                                        impactPoint.y, 
                                        impactPoint.z);
        }
        
        /*
         * The original plan:
         * - Find texture units per game unit
         * - Diff between impact point and lowest vertex
         * - new tex coord = lowest tex coord + (Diff * tex units)
         *
         * This is all bullshit though. What we need
         * - Precalculated Delauny triangles, with precalculated texture coords
         * Even better:
         * - precalculated deformation polygons based on the properties of the material.
         */
        // Find texture aabb:
        texAABB.lower.Set(particle->GetTextureCoordinates()[0].x,
                          particle->GetTextureCoordinates()[0].y,
                          0.0f);
        texAABB.upper.Set(particle->GetTextureCoordinates()[0].x,
                          particle->GetTextureCoordinates()[0].y,
                          0.0f);
        for (int i = 0; i < particle->GetVertexCount(); ++i)
        {
            texAABB.lower = hdMin(texAABB.lower, hdVec2toVec3(particle->GetTextureCoordinates()[i]));
            texAABB.upper = hdMax(texAABB.upper, hdVec2toVec3(particle->GetTextureCoordinates()[i]));
        }
        
        hdVec2 tDiff = hdVec3toVec2(texAABB.upper - texAABB.lower);
        
        hdVec3 vDiff = (particle->GetAABB().upper - particle->GetAABB().lower);
        
        tpgx = tDiff.x / vDiff.x; 
        tpgy = tDiff.y / vDiff.y;
        
        diff = polygonInfo.impactPoint - particle->GetAABB().lower;
        
        polygonInfo.impactTexCoord = hdVec2(tpgx * diff.x, 1.0f - (tpgy * diff.y));
        
        polygonInfo.texture = particle->m_texture;
        polygonInfo.tint[0] = particle->m_tint[0];
        polygonInfo.tint[1] = particle->m_tint[1];
        polygonInfo.tint[2] = particle->m_tint[2];
        polygonInfo.tint[3] = particle->m_tint[3];
        polygonInfo.vertices = particle->GetVertices();
        polygonInfo.startingVertices = particle->GetStartingVertices();
        polygonInfo.texCoords = particle->GetTextureCoordinates();
        
        polygonInfo.vertexCount = particle->GetVertexCount();
        polygonInfo.worldCenter = particle->GetWorldCenter();
        
        polygonInfo.depth = particle->m_depth;
        polygonInfo.zOffset = 0.0f;
        polygonInfo.parentType = particle->GetUserType();
        polygonInfo.parentTag = particle->m_parentTag;
        
        m_physicsParticleEmitter->StartWithTexture(polygonInfo);
    }
    
    hdAABB blockAABB;
    blockAABB.lower = (particle->GetAABB().lower + hdVec3(0.01f, 0.01f, 0.01f));
    blockAABB.upper = (particle->GetAABB().upper - hdVec3(0.01f, 0.01f, 0.01f));
    
    //m_smokeEmitter->Start(blockAABB);
    m_splinterEmitter->SetParticlePointSize(fabs(particle->m_depth));
    m_splinterEmitter->SetParticleColor(hdColor4(particle->m_tint[0], 
                                                 particle->m_tint[1],
                                                 particle->m_tint[2], 
                                                 1.0f));
    m_splinterEmitter->Start(particle->m_texture, blockAABB);
    
    if (!playedParticleSmashSound)
    {
        hdSound *sound = m_blockExplosionSoundsStone->GetItems()[rand() % m_blockExplosionSoundsStone->GetItemCount()];
        sound->volume = 0.4f;
        sound->pitchOffset = hdRandom(1.25f, 1.5f);
        hdSoundManager::PlaySound(sound);
        playedParticleSmashSound = true;
    }
    
    particle->DestroyPhysicsBody(m_physicsWorld);
    particle->life = 0.0f;
} 


const bool LevelController::IsShowingMessage() const
{
    if (m_uiMessageContainer == NULL) return false;
    return m_uiLandscapeMessageContainer->IsVisible() && m_uiPortraitMessageContainer->IsVisible();
}


void LevelController::LoadScriptMessageForTag(const int tag)
{
    LoadScriptMessageForTag(tag, false);
}



void LevelController::LoadScriptMessageForTag(const int tag, const bool addToQueue)
{
    LoadScriptMessageForTagAndLevel(tag, m_currentLevelId, addToQueue);
}


void LevelController::LoadScriptMessageForTagAndLevel(const int tag, const int levelId,  const bool addToQueue)
{
    int msgCount;
    totemMessage t;
    
    if (addToQueue || IsShowingMessage())
    {
        t.levelId = levelId;
        t.targetId = tag;
        
        // Actually any messages???
        msgCount = Scripts_CountLevelMessagesForTag(levelId, tag);
        
        if (msgCount > 0)
        {
            if (-1 == m_messageTagQueue->Add(t))
            {
                hdPrintf("Message Queue is full!\n");
            }
            
            m_uiMessageQueueCount->SetTextFormatted("%d", m_messageTagQueue->GetItemCount());
            m_uiMessageQueueContainer->Show();
        }
    }
    else
    {
        m_currentMessageIndex = 0;
        m_messageCount = Scripts_GetLevelMessagesForTag(m_scriptMessages, 10, levelId, tag);
        ShowNextScriptMessage();
    }
}


void LevelController::LoadScriptMessageForTagAndLevel(const int tag, const int levelId, const bool addToQueue, const bool noShow)
{
    m_currentMessageIndex = 0;
    m_messageCount = Scripts_GetLevelMessagesForTag(m_scriptMessages, 10, levelId, tag);
}


const bool LevelController::ShowNextScriptMessage()
{
    m_OkButton->Hide();
    m_PrevButton->Enable();
    if (m_messageCount <= 1)
    {
        m_SkipButton->Hide();
        m_PrevButton->Hide();
        m_NextButton->Hide();
        m_DoneButton->Hide();
        m_OkButton->Show();
    }
    else
    {
        m_SkipButton->Show();
        if (m_currentMessageIndex == 0)
        {
            m_PrevButton->Disable();
            m_PrevButton->Show();
            m_NextButton->Show();
            m_DoneButton->Hide();
        }
        else if (m_currentMessageIndex == (m_messageCount - 1))
        {
            m_PrevButton->Show();
            m_NextButton->Hide();
            m_DoneButton->Show();
        }
        else
        {
            m_PrevButton->Show();
            m_NextButton->Show();
            m_DoneButton->Hide();
        }
    }
    
    if (m_messageCount > 0 &&
        m_currentMessageIndex < m_messageCount)
    {
        SetMessage(m_scriptMessages[m_currentMessageIndex]);
        m_interactionState = e_interactionDisabled;
        ShowMessage();
        ++m_currentMessageIndex;
        return true;
    }
    
    m_OkButton->Show();
    m_SkipButton->Hide();
    m_PrevButton->Hide();
    m_NextButton->Hide();
    m_DoneButton->Hide();
    
    return false;
}


const bool LevelController::Game_InitLevel(const Level *level, const int levelId)
{
    return true;
}


void LevelController::Game_Step(double interval) {}


void LevelController::Game_ResetCurrentLevel() {}


const bool LevelController::Game_CanSelectBlock(const Block *block)
{
    return (!block->IsStatic());
}


void LevelController::Game_hdkBlockWasSelected(const Block *block) const {}


const bool LevelController::Game_HandleTapUp(float x, float y, int tapCount)
{
    return false;
}


const bool LevelController::Game_HandleTapDown(float x, float y, int tapCount)
{
    return false;
}


const bool LevelController::Game_HandleSingleTap(float x, float y)
{
    return false;
}


const bool LevelController::Game_HandleDoubleTap(float x, float y)
{
    return false;
}


const bool LevelController::Game_HandleTapMovedSingle(const float previousX, const float previousY, 
                                                           const float currentX, const float currentY)
{
    return false;
}


const bool LevelController::Game_HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
                                                           const float aCurrentX, const float aCurrentY,
                                                           const float bPreviousX, const float bPreviousY,	
                                                           const float bCurrentX, const float bCurrentY)
{
    return false;
}


const bool LevelController::Game_HandleBlockTap(Block *block, const hdVec3& impactPoint, bool hazardCollision)
{
    return true;
}


const bool LevelController::Game_HandleParticleTap(hdPhysicsParticle* particle, const hdVec3& impactPoint, bool hazardCollision)
{
    return true;
}


const bool LevelController::Game_HandleBlockLoss(Block* block, const hdVec3& contactPoint) 
{
    return false;
}


void LevelController::Game_HandleCollision(const ContactPoint* point) {}


void LevelController::Game_ShowMessage() {}


void LevelController::Game_HideMessage() {}


void LevelController::Game_PlayLoudCollisionSound(const Block* block, const b2Body* body) {}


void LevelController::Game_PlaySoftCollisionSound(const Block* block, const b2Body* body) {}


void LevelController::Game_AddBlockToLevel(const Block* block) {}


void LevelController::Game_HandleOrientationChanged() {}


void LevelController::Game_HandleGoalCollision(Block *goalBlock, Block *block, const hdVec3& impactPoint) {}
