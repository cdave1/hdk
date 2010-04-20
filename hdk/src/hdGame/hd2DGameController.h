/*
 *  hd2DGameController.h
 *  hdGameEngine
 *
 *  Created by david on 15/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_2D_GAME_CONTROLLER_
#define _HD_2D_GAME_CONTROLLER_

#include "hdMath.h"
#include "hdGame/hdGameWorld.h"


class hd2DGameController
{
private:
	hdVec2 m_targetScreenArea; // we are drawing for the iPhone.
	
	hdVec2 m_targetScreenPosition;
	
	float32 m_timeStep;
	
	int32 m_iterations;
	
	// gameworld stuff
	hdGameWorld *m_gameWorld;
	
	int32 m_pointCount;
	
public:	
	hd2DGameController(const char* path);
	
	~hd2DGameController();
	
	void Step(double interval);
	
	void Draw();
	
	void HandleDoubleTap(float x, float y);
	
	void PanProjection(const float aPreviousX, const float aPreviousY,
					   const float aCurrentX, const float aCurrentY);
	
	void ZoomProjection(const float aPreviousX, const float aPreviousY,
						const float aCurrentX, const float aCurrentY,
						const float bPreviousX, const float bPreviousY,	
						const float bCurrentX, const float bCurrentY);
		
};

#endif