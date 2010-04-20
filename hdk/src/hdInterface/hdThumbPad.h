/*
 *  hdThumbPad.h
 *  fpsTest
 *
 *  Created by david on 16/11/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

/*
 
 Mimics a thumb pad.
 
 Used to return a normalized vector indicating the difference
 between thumb's current position, and center of bounding
 polygon.
 
 Use an octagon to indicate the bounds of the pad.
 
 pad icon is always at thumb's position; on mouse up
 it snaps back to the origin.
 
 */

#ifndef HD_THUMB_PAD
#define HD_THUMB_PAD

#include "hdGameWorld.h"
#include "hdReceiver.h"
#include "hdDrawable.h"
#include "hdMath.h"
#include "hdGeometry.h"
#include "hdTypedefList.h"
#include "hdTextureManager.h"
#include "hdTexture.h"
#include "hdDrawingUtilities.h"
#include "hdUIImage.h"

typedef enum
	{
		e_padWaiting = 0,
		e_padMoving,
		e_padDisabled,
	} e_padState;

class hdThumbPad : public hdReceiver
{
public:
	hdThumbPad() {}
	
	hdThumbPad(const char *bgTexture,
			   const char *padTexture,
			   const float x, const float y, 
			   const float w, const float h);
	
	~hdThumbPad();
	
	void Draw() const;
	
	bool MouseDown(float x, float y);
	
	bool MouseOver(float x, float y);
	
	bool MouseUp(float x, float y);
	
	bool IsMoving();
	
	const hdVec2 GetPadDirection();
	
	
private:
	void Init();
	
	void DrawImpl();
	
	void MoveThumbPadTo(float x, float y);
	
	hdUIImage *m_pad;
	
	hdTexture *m_bgTexture;
	
	e_padState m_state;
	
	float m_radius;
};


#endif