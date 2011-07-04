/*
 *  hdInterfaceContext.h
 *  Smashed
 *
 *  Created by david on 2/11/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_INTERFACE_CONTEXT_H
#define HD_INTERFACE_CONTEXT_H

#include "hdGame.h"
#include "hdGraphicsLibrary.h"
#include "hdMath.h"
#include "hdCollections.h"
#include "hdInterface/hdReceiver.h"
#include "hdInterface/hdInterfaceController.h"
 
/*
 * Retains a list of receivers. Receivers add themselves
 * to this context when they are created.
 *
 * The context dispatches events to each of the receivers.
 *
 * A context exists for the duration of an interface
 * controller, but is not tied to the interface controller.
 *
 * Events are sent via app controller; if there is no context
 * then just do nothing.
 */

class hdInterfaceController;


class hdInterfaceContext
{
public:
	static void Init(hdInterfaceController *context);
	
	static void TearDown(hdInterfaceController *context);
	
	static void EnableLogging();
	
	static void DisableLogging();
	
	static bool AddReceiver(const hdReceiver *receiver);
	
	static bool RemoveReceiver(const hdReceiver *receiver);
	
	static void HandleTapUp(float x, float y, int tapCount);
	
	static void HandleTapDown(float x, float y, int tapCount);
	
	static void HandleSingleTap(const float x, const float y);
	
	static void HandleDoubleTap(float x, float y);
	
	static void HandleTapMovedSingle(const float previousX, const float previousY, 
							  const float currentX, const float currentY);
	
	static void HandleTapMovedDouble(const float aPreviousX, const float aPreviousY,
							  const float aCurrentX, const float aCurrentY,
							  const float bPreviousX, const float bPreviousY,
							  const float bCurrentX, const float bCurrentY);
};


#endif