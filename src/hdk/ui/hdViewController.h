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

#ifndef _HD_VIEW_CONTROLLER_H_
#define _HD_VIEW_CONTROLLER_H_

#include <hdk/ui/hdInterfaceEnums.h>
#include <hdk/ui/hdUIContainer.h>
#include <hdk/ui/hdIBViewController.h>
#include <hdk/ui/hdIBLoader.h>

class hdViewController : public hdUIContainer
{
public:
	hdViewController(const char *hdIBViewControllerFilePath);
	
	~hdViewController();
	
	virtual void RefreshLayout(const e_hdInterfaceOrientation &orientation);
	
	/**
	 * For all items with tag, add the attached listener for mouse down
	 * events. If tag is NULL, attempt to add the listener to all items.
	 */
	void SetMouseDownCallbackForTaggedItems(const char *tag, void *obj, void (*func)(void *, void *)); 
	
	/**
	 * Same as SetMouseDownCallbackForTaggedItems, but for mouse up events.
	 */
	void SetMouseUpCallbackForTaggedItems(const char *tag, void *obj, void (*func)(void *, void *));
	
	const hdReceiver * GetInterfaceItemWithTag(const char *tag);

	void SetCloseListener(void *obj, void (*func)(void *, void *));
	
	void Draw();

	
protected:
	
	virtual void InitInterface();
	
	virtual void InitSounds();
	
	virtual void DoClosePrefsCallback();
	
private:
	void WillDraw() const;
	
	void DidDraw() const;
	
	void (*m_closePrefsCallback)(void *, void *);
	
	void *m_closePrefsCallbackObject;
	
	hdIBViewController *m_hdIBViewController;
	
	float m_width;
	
	float m_height;
	
};

#endif
