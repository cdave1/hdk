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

#ifndef APP_INTERFACE_H
#define APP_INTERFACE_H


#include "hdk.h"

#define LEVEL_CONTROLLER_TITLE_FONT "Fonts/bebas18.f"
#define LEVEL_CONTROLLER_MESSAGE_FONT "Fonts/collegiate16.f"
#define LEVEL_CONTROLLER_TINY_FONT "Fonts/bebas10.f"

class AppInterface : public hdInterfaceController
{
public:
	AppInterface(const hdInterfaceController* parentController);
	
	~AppInterface();
	
	
	
	void DrawAppInterface();

	void RefreshLayout();
	
	
	
protected:
	static void HideMessageClickCallback(void *handler, void *sender);
	
	
	static void SkipUpCallback(void *handler, void *sender);
	
	
	static void PrevUpCallback(void *handler, void *sender);
	
	
	static void MessageQueueUpClicked(void *handler, void *sender);
	
	
	static void GenericButtonDownCallback(void *handler, void *sender);
	
	virtual void Game_ShowMessage();
	
	virtual void Game_HideMessage();
	
	
	void FindAndShowQueueMessage();
	
	/**********************************************************************/
	/* Messaging Functions */
	/**********************************************************************/
	void LoadScriptMessageForTag(const int tag);
	
	void LoadScriptMessageForTag(const int tag, const bool addToQueue);
	
	void LoadScriptMessageForTagAndLevel(const int tag, const int levelId, const bool addToQueue);
	
	void LoadScriptMessageForTagAndLevel(const int tag, const int levelId, const bool addToQueue, const bool noShow);
	
	void SetMessage(const hdMessage *msg);
	
	void SetMessage(const hdTexture *texture);
	
	void ShowMessage();
	
	void HideMessage();
	
	const bool IsShowingMessage() const;
	
	
	/**********************************************************************/
	/* Messages */
	/*
	 * Note: two versions of each, one for portrait mode, the other
	 * for landscape mode.
	 *
	 * The final set of pointers are just pointers
	 */
	/**********************************************************************/
	hdMessage* m_scriptMessages[10];
	
	int m_messageCount;
	
	int m_currentMessageIndex;
	
	hdTypedefList<hdMessage, 16> *m_messageTagQueue;
	
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
	
	
	hdUIImage *m_glow;
	
	hdUIImage *m_uiPortraitAvatar;
	
	hdUIImage *m_uiPortraitAvatarChrome;
	
	hdFontPolygon *m_uiPortraitAvatarMessageText;
	
	hdUIImage *m_uiPortraitAvatarMessageBackground;
	
	hdUIImage *m_uiPortraitAvatarMessageChrome;
	
private:
	void InitAppInterface();
	
	hdSound *m_btnMessageClickDownSound;
	hdSound *m_btnMessageClickUpSound;
};


inline hdUIContainer * AppInterface::GetCurrentMessageContainer() const
{
	if (m_orientation == e_hdInterfaceOrientationLandscape)
	{
		return m_uiLandscapeMessageContainer;
	}
	else
	{
		return m_uiPortraitMessageContainer;
	}
}



#endif