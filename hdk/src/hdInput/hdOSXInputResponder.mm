/*
 *  hdOSXMouseInput.cpp
 *  Smashed
 *
 *  Created by david on 25/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdOSXInputResponder.h"

id idResponder;


mousevalues_t GetMouseValues()
{
	
	
	
	return [idResponder GetMouseValues];
}

@interface hdOSXInputResponder (Private)

mousevalues_t mouseValues;

@end

@implementation hdOSXInputResponder

- (BOOL)acceptsFirstResponder
{
	idResponder = self;
	return YES;
}

- (mousevalues_t)GetMouseValues
{
	return mouseValues;
}

- (void)mouseDown:(NSEvent *)theEvent
{
	if (theEvent.type == NSLeftMouseDown)
	{
		assert(mouseValues.leftButton.clickState == e_clickstate_null);
		mouseValues.leftButton.clickState = e_clickstate_down;
		mouseValues.leftButton.xClickDown = theEvent.locationInWindow.x; 
		mouseValues.leftButton.yClickDown = theEvent.locationInWindow.y;
		mouseValues.leftButton.clickDownTimeStamp = theEvent.timestamp;
	}
	else if (theEvent.type == NSRightMouseDown)
	{
		assert(mouseValues.rightButton.clickState == e_clickstate_null);
		mouseValues.rightButton.clickState = e_clickstate_down;
		mouseValues.rightButton.xClickDown = theEvent.locationInWindow.x; 
		mouseValues.rightButton.yClickDown = theEvent.locationInWindow.y;
		mouseValues.rightButton.clickDownTimeStamp = theEvent.timestamp;		
	}
}


- (void)mouseUp:(NSEvent *)theEvent
{
	if (theEvent.type == NSLeftMouseUp)
	{
		assert(mouseValues.leftButton.clickState == e_clickstate_down ||
			   mouseValues.leftButton.clickState == e_clickstate_moving);
		mouseValues.leftButton.clickState = e_clickstate_up;
		mouseValues.leftButton.xClickUp = theEvent.locationInWindow.x; 
		mouseValues.leftButton.yClickUp = theEvent.locationInWindow.y;
		mouseValues.leftButton.clickUpTimeStamp = theEvent.timestamp;	
	}
	else if (theEvent.type == NSRightMouseDown)
	{
		assert(mouseValues.rightButton.clickState == e_clickstate_down ||
			   mouseValues.rightButton.clickState == e_clickstate_moving);
		mouseValues.rightButton.clickState = e_clickstate_up;
		mouseValues.rightButton.xClickDown = theEvent.locationInWindow.x; 
		mouseValues.rightButton.yClickDown = theEvent.locationInWindow.y;
		mouseValues.rightButton.clickDownTimeStamp = theEvent.timestamp;		
	}
}


- (void)mouseMoved:(NSEvent *)theEvent
{
	if (theEvent.type == NSMouseMoved)
	{
		mouseValues.xPrevious = mouseValues.xCurrent;
		mouseValues.yPrevious = mouseValues.yCurrent;
		
		mouseValues.xCurrent = theEvent.locationInWindow.x;
		mouseValues.yCurrent = theEvent.locationInWindow.y;
		
		if (mouseValues.leftButton.clickState == e_clickstate_down)
		{
			mouseValues.leftButton.clickState = e_clickstate_moving;
		}
		
		if (mouseValues.rightButton.clickState == e_clickstate_down)
		{
			mouseValues.rightButton.clickState = e_clickstate_moving;
		}
	}
}


- (void)mouseDragged:(NSEvent *)theEvent
{
}

- (void)scrollWheel:(NSEvent *)theEvent
{
}

@end