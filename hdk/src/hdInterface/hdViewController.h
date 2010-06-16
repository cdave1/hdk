/*
 *  hdIBController.h
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 * A collection of interface entities.
 */

#ifndef _HD_VIEW_CONTROLLER_H_
#define _HD_VIEW_CONTROLLER_H_

#include "hdk.h"
#include "hdInterface/hdIBViewController.h"
#include "hdInterface/hdIBLoader.h"

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