/*
 *  hdUIContainer.h
 *  TotemGame
 *
 *  Created by david on 23/05/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_INTERFACE_CONTAINER_H_
#define _HD_INTERFACE_CONTAINER_H_

#include "hdInterface/hdReceiver.h"
#include "hdCollections.h"


class hdUIContainer : public hdReceiver
{
public:
	hdUIContainer(hdGameWorld* gameWorld);
	
	hdUIContainer(hdGameWorld* gameWorld,  const hdVec3& lower, const hdVec3& upper);
	
	~hdUIContainer();
	
	int Add(hdReceiver *receiver);
	
	bool Remove(hdReceiver *receiver);
	
	void Translate(const hdVec3& translationVector, const hdVec3& translationAxis);
	
	void RotateAndTranslate(const hdVec3& rotVector, const hdVec3& transVector);
	
	// Check whether the touch event was in the bounds of any buttons.
	bool MouseDown(float x, float y);
	
	bool MouseOver(float x, float y);
	
	bool MouseUp(float x, float y);
	
	void Draw() const;
	
	void Hide();
	
	void Show();
	
	void Enable();
	
	void Disable();

	
	
private:
	void Init();
	//hdPointerList<hdUIImage, 64>* m_images;
	
	//hdPointerList<hdPolygon, 32>* m_polygons;
	
	hdTypedefList<hdReceiver*, 32>* m_receivers;
	
	bool m_isEnabled;
	
	void *m_tag;
};



#endif