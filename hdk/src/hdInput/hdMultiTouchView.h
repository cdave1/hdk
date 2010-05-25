//
//  hdMultiTouchScreen.h
//  hdk
//
//  Created by David Petrie on 11/04/10.
//  Copyright 2010 Hackdirt Ltd. All rights reserved.
//

#import "hdGraphicsLibrary/hdGLView.h"

@class hdGLView;

const int kMultiTouchScreenValues = 10;

typedef struct Touch
{ 
	float LocationXTouchBegan;    // x and y coordinates where the touch began
	float LocationYTouchBegan;
	//int TapCountBegan;
	//float CountTouchesBegan;		// how many fingers are pressed
	//float TapCountTouchesBegan;		// tap with one, two or three fingers or more at once
	
	float LocationXTouchMovedPrevious;
	float LocationYTouchMovedPrevious;
	
	float LocationXTouchMoved;	// x and y coordinates where the touch moved
	float LocationYTouchMoved;
	
	float LocationXTouchEnded;
	float LocationYTouchEnded;
	//int TapCountMoved;
	
	NSTimeInterval TimeStamp;
	
	//int TouchID; // iPhone touch id as int
	int TouchID;
	int TapCount;
	
	bool TouchDown;
	bool TouchUp;
	bool TouchMoved;
	
	
	
	//bool TouchesEnd;				// nothing touches the screen anymore
} 
TouchScreenValues; 


@interface hdMultiTouchView : hdGLView
{
	//TouchScreenValues TouchScreen;	
	
	TouchScreenValues CPY_TouchScreenValues[kMultiTouchScreenValues];
	
	int CountTouchesBegan;
	int CountTouchesMoved;
	int TouchCount;
	NSObject *renderLock;
	NSObject *touchScreenLock;
@private
	UIEvent* lastTouchBeganEvent;
	UIEvent* lastTouchMovedEvent;
	UIEvent* lastTouchEndedEvent;
	
	TouchScreenValues MultiTouchScreen[kMultiTouchScreenValues];
	
	BOOL _logOutput;
}

@property (readwrite, assign) NSObject *renderLock;

//- (TouchScreenValues*) GetValuesTouchScreen;
- (TouchScreenValues*)GetValuesMultiTouchScreen;
- (int) GetCountTouchesBegan;
- (int) GetCountTouchesMoved;
- (int) GetTouchCount;

@property BOOL logOutput;

@end


// C wrapper
//TouchScreenValues* GetValuesTouchScreen();
TouchScreenValues* GetValuesMultiTouchScreen();
int GetCountTouchesBegan();
int GetCountTouchesMoved();
int GetTouchCount();