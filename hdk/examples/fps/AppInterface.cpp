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

#include "AppInterface.h"


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


AppInterface::AppInterface(const hdInterfaceController* parentController) : hdInterfaceController(e_hdInterfaceOrientationLandscape, parentController)
{
	hdInterfaceContext::Init(this);
	
	InitAppInterface();
}


AppInterface::~AppInterface()
{
	hdInterfaceContext::TearDown(this);
	
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
	
	delete m_uiLandscapeTitleMessageBackground;
	
	delete m_uiLandscapeTitleMessageFont;
	
	delete m_uiPortraitTitleMessageBackground;
	
	delete m_uiPortraitTitleMessageFont;
}






void AppInterface::InitAppInterface()
{
	float longSide;
	float shortSide;
	
	
	
	longSide = hdMax(m_PixelScreenWidth, m_PixelScreenHeight);
	shortSide = hdMin(m_PixelScreenWidth, m_PixelScreenHeight);
	
	m_btnMessageClickDownSound = hdSoundManager::FindSound("Sounds/btnClickDown.caf", e_soundTypeNormal);
	m_btnMessageClickUpSound = hdSoundManager::FindSound("Sounds/btnClick01.caf", e_soundTypeNormal);
	
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
	
	/* Landscape */
	m_uiLandscapeMessageOKButton->SetMouseDownListener(this, AppInterface::GenericButtonDownCallback);
	m_uiLandscapeMessageOKButton->SetMouseUpListener(this, AppInterface::HideMessageClickCallback);
	
	m_uiLandscapeMessageDoneButton->SetMouseDownListener(this, AppInterface::GenericButtonDownCallback);
	m_uiLandscapeMessageDoneButton->SetMouseUpListener(this, AppInterface::HideMessageClickCallback);
	
	m_uiLandscapeMessageSkipButton->SetMouseDownListener(this, AppInterface::GenericButtonDownCallback);
	m_uiLandscapeMessageSkipButton->SetMouseUpListener(this, AppInterface::SkipUpCallback);
	
	m_uiLandscapeMessagePrevButton->SetMouseDownListener(this, AppInterface::GenericButtonDownCallback);
	m_uiLandscapeMessagePrevButton->SetMouseUpListener(this, AppInterface::PrevUpCallback);
	
	m_uiLandscapeMessageNextButton->SetMouseDownListener(this, AppInterface::GenericButtonDownCallback);
	m_uiLandscapeMessageNextButton->SetMouseUpListener(this, AppInterface::HideMessageClickCallback);
	
	/* Portrait */
	m_uiPortraitMessageOKButton->SetMouseDownListener(this, AppInterface::GenericButtonDownCallback);
	m_uiLandscapeMessageOKButton->SetMouseUpListener(this, AppInterface::HideMessageClickCallback);
	
	m_uiPortraitMessageDoneButton->SetMouseDownListener(this, AppInterface::GenericButtonDownCallback);
	m_uiLandscapeMessageDoneButton->SetMouseUpListener(this, AppInterface::HideMessageClickCallback);
	
	m_uiPortraitMessageSkipButton->SetMouseDownListener(this, AppInterface::GenericButtonDownCallback);
	m_uiLandscapeMessageSkipButton->SetMouseUpListener(this, AppInterface::SkipUpCallback);
	
	m_uiPortraitMessagePrevButton->SetMouseDownListener(this, AppInterface::GenericButtonDownCallback);
	m_uiLandscapeMessagePrevButton->SetMouseUpListener(this, AppInterface::PrevUpCallback);
	
	m_uiPortraitMessageNextButton->SetMouseDownListener(this, AppInterface::GenericButtonDownCallback);
	m_uiLandscapeMessageNextButton->SetMouseUpListener(this, AppInterface::HideMessageClickCallback);
	
	/* Message Queue Handlers */
	m_uiMessageQueueButton->SetMouseDownListener(this, AppInterface::GenericButtonDownCallback);
	
	m_uiMessageQueueButton->SetMouseUpListener(this, AppInterface::MessageQueueUpClicked);
	
	
}


void AppInterface::RefreshLayout()
{
	m_uiMessageQueueButton->SetAs2DBox(5.0f, 5.0f, 45.0f, 45.0f);
	m_uiMessageQueueCount->SetAs2DBox(38.0f, 17.0f, 14.0f, 14.0f);
	
	
	
	if (GetOrientation() == e_hdInterfaceOrientationLandscape)
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




void AppInterface::HideMessageClickCallback(void *handler, void *sender)
{
	AppInterface *self = (AppInterface *)handler;
	if (!self) return;
	
	hdSoundManager::PlaySound(self->m_btnMessageClickUpSound);
	self->HideMessage();
}


void AppInterface::SkipUpCallback(void *handler, void *sender)
{
	AppInterface *self = (AppInterface *)handler;
	if (!self) return;
	
	self->m_messageCount = 0;
	hdSoundManager::PlaySound(self->m_btnMessageClickUpSound);
	self->HideMessage();
}


void AppInterface::PrevUpCallback(void *handler, void *sender)
{
	AppInterface *self = (AppInterface *)handler;
	if (!self) return;
	
	if (self->m_currentMessageIndex >= 2)
	{
		hdSoundManager::PlaySound(self->m_btnMessageClickUpSound);
		self->m_currentMessageIndex = hdMax(0, self->m_currentMessageIndex-2); // because the index is incremented
		self->HideMessage();
	}
}


void AppInterface::MessageQueueUpClicked(void *handler, void *sender)
{
	AppInterface *self = (AppInterface *)handler;
	if (!self) return;
	
	self->FindAndShowQueueMessage();
	
}


void AppInterface::GenericButtonDownCallback(void *handler, void *sender)
{
	AppInterface *self = (AppInterface *)handler;
	if (!self) return;
	
	hdSoundManager::PlaySound(self->m_btnMessageClickDownSound);
}


void AppInterface::FindAndShowQueueMessage()
{
	hdMessage msg;
	
	hdSoundManager::PlaySound(m_btnMessageClickUpSound);
	
	hdAssert (m_messageTagQueue->GetItemCount() > 0);
	
	msg = m_messageTagQueue->GetItems()[0];
	if (msg.contextId == -1)
	{
		LoadScriptMessageForTag(msg.targetId, false);
	}
	else
	{
		LoadScriptMessageForTagAndLevel(msg.targetId, msg.contextId, false);
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



void AppInterface::ShowMessage()
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


void AppInterface::HideMessage()
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
	anim->SetFinishedCallback(this, AppInterface::HideMessageCallback);
	anim->StartAnimation();
	
	longSide = hdMax(m_PixelScreenWidth, m_PixelScreenHeight);
	shortSide = hdMin(m_PixelScreenWidth, m_PixelScreenHeight);
	
	HideMessageInterface(m_uiLandscapeMessageContainer, m_uiLandscapeMessageButtonContainer, longSide, shortSide);
	HideMessageInterface(m_uiPortraitMessageContainer, m_uiPortraitMessageButtonContainer, shortSide, longSide);	
}


void AppInterface::ShowMessageInterface(hdUIContainer *container, hdUIContainer *buttonContainer,
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


void AppInterface::HideMessageInterface(hdUIContainer *container, hdUIContainer *buttonContainer,
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
	//anim1->SetFinishedCallback(this, AppLevelController::HideMessageCallback);
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


void AppInterface::HideMessageCallback(void *handler, hdAnimation *anim)
{
	if (handler == NULL || anim == NULL) 
	{
		hdError(-1, "Fucking animation and handler have gone away in between anim starting and anim stopping.");
		return;
	}
	
	AppInterface *self = (AppInterface *)handler;
	
	self->Game_HideMessage();
	
	self->m_uiLandscapeMessageContainer->Hide();
	self->m_uiPortraitMessageContainer->Hide();
	self->ShowNextScriptMessage();
}


void AppInterface::SetMessageTexture(const char *texturePrefix)
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


void AppInterface::SetMessage(const hdTexture *texture)
{
	m_uiLandscapeTextMessageBackground->SetTexture(texture);
	m_uiLandscapeTextMessageBackground->Show();
	
	m_uiPortraitTextMessageBackground->SetTexture(texture);
	m_uiPortraitTextMessageBackground->Show();
	
	m_uiLandscapeTextMessage->Hide();
	m_uiPortraitTextMessage->Hide();
}


void AppInterface::SetMessage(const hdMessage *msg)
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
	
	if (msg->messageType == e_hdMessageTypeImage)
	{ 
		// Assume text is a texture
		SetMessageTexture(msg->texture);
		
		m_uiLandscapeTextMessage->Hide();
		
		m_uiPortraitTextMessage->Hide();
	}
	else if (msg->messageType == e_hdMessageTypeTitle)
	{
		m_uiLandscapeTitleMessageFont->SetText(msg->message);
		m_uiLandscapeTitleMessageFont->Show();
		m_uiLandscapeTitleMessageBackground->Show();
		
		m_uiPortraitTitleMessageFont->SetText(msg->message);
		m_uiPortraitTitleMessageFont->Show();
		m_uiPortraitTitleMessageBackground->Show();
	}
	else if (msg->messageType == e_hdMessageTypeText)
	{
		SetMessageTexture("Interface/imgScriptMessage");
		
		m_uiLandscapeTextMessage->SetText(msg->message);
		m_uiLandscapeTextMessage->Show();
		
		m_uiPortraitTextMessage->SetText(msg->message);
		m_uiPortraitTextMessage->Show();
	}
	else if (msg->messageType == e_hdMessageTypeCustomImageText)
	{
		SetMessageTexture(msg->texture);
		
		m_uiLandscapeTextMessage->SetText(msg->message);
		m_uiLandscapeTextMessage->Show();
		
		m_uiPortraitTextMessage->SetText(msg->message);
		m_uiPortraitTextMessage->Show();
	}
	else if (msg->messageType == e_hdMessageTypeAvatar)
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


const bool AppInterface::IsShowingMessage() const
{
	if (m_uiMessageContainer == NULL) return false;
	return m_uiLandscapeMessageContainer->IsVisible() && m_uiPortraitMessageContainer->IsVisible();
}


void AppInterface::LoadScriptMessageForTag(const int tag)
{
	LoadScriptMessageForTag(tag, false);
}



void AppInterface::LoadScriptMessageForTag(const int tag, const bool addToQueue)
{
	LoadScriptMessageForTagAndLevel(tag, -1, addToQueue);
}


void AppInterface::LoadScriptMessageForTagAndLevel(const int tag, const int contextId,  const bool addToQueue)
{
	int msgCount;
	hdMessage t;
	
	if (addToQueue || IsShowingMessage())
	{
		t.contextId = contextId;
		t.targetId = tag;
		
		// Actually any messages???
		msgCount = Scripts_CountContextMessagesForTag(contextId, tag);
		
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
		m_messageCount = Scripts_GetLevelMessagesForTag(m_scriptMessages, 10, contextId, tag);
		ShowNextScriptMessage();
	}
}


void AppInterface::LoadScriptMessageForTagAndLevel(const int tag, const int contextId, const bool addToQueue, const bool noShow)
{
	m_currentMessageIndex = 0;
	m_messageCount = Scripts_GetLevelMessagesForTag(m_scriptMessages, 10, contextId, tag);
}


const bool AppInterface::ShowNextScriptMessage()
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




void AppInterface::Game_ShowMessage() {}


void AppInterface::Game_HideMessage() {}