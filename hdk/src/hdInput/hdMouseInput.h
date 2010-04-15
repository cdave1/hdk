/*
 *  hdMouseInput.h
 *  Smashed
 *
 *  Created by david on 25/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_MOUSE_INPUT
#define HD_MOUSE_INPUT

typedef enum clickstate
{
	e_clickstate_null = 0,
	e_clickstate_down = 1,
	e_clickstate_moving = 2,
	e_clickstate_up = 3,
	e_clickstate_scrollup = 4,
	e_clickstate_scrolldown = 5,
} 
e_clickstate_t;


typedef struct clickinfo
{
	clickinfo() :
		xClickDown(0.0f),
		yClickDown(0.0f),
		xClickUp(0.0f),
		yClickUp(0.0f),
		clickDownTimeStamp(0),
		clickUpTimeStamp(0),
		isDown(false),
		isMoving(false),
		isUp(false),
		clickCount(0)
	{}
	
	float xClickDown;
	float yClickDown;
	float xClickUp;
	float yClickUp;
	double clickDownTimeStamp;
	double clickUpTimeStamp;
	bool isDown;
	bool isMoving;
	bool isUp;
	uint16 clickCount;
}
clickinfo_t;


typedef struct mousevalues
{ 
	mousevalues() :
		xCurrent(0.0f), 
		yCurrent(0.0f),
		xPrevious(0.0f),
		yPrevious(0.0f) {}
	
	float xCurrent;    // x and y coordinates where the touch began
	float yCurrent;
	
	float xPrevious;
	float yPrevious;

	clickinfo_t leftButton;
	
	clickinfo_t rightButton;
	
	clickinfo_t scrollWheel;
} 
mousevalues_t;


extern mousevalues_t GetMouseValues();



#endif