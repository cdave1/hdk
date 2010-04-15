/*
 *  hdInterfaceEnums.h
 *  TotemGame
 *
 *  Created by david on 21/09/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_INTERFACE_ENUMS_H
#define HD_INTERFACE_ENUMS_H

typedef enum
{
	e_hdInterfaceClickStateDown = 0,
	e_hdInterfaceClickStateOver,
	e_hdInterfaceClickStateUp,
} e_hdInterfaceClickState;


typedef enum
{
	e_hdInterfaceOrientationPortrait = 0,
	e_hdInterfaceOrientationLandscape = 1,
} e_hdInterfaceOrientation;


typedef enum
{
	e_screenEdgeLeft = 0,
	e_screenEdgeRight,
	e_screenEdgeBottom,
	e_screenEdgeTop,
} e_screenEdge;


typedef enum 
{
	e_interactionDisabled = 0,
	e_waitingForInput,
	e_panAndZoom,
	e_interactionStateSelecting,
} e_hdInteractionState;


#endif