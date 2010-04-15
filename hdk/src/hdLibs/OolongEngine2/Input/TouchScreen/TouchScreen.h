/*
Oolong Engine for the iPhone / iPod touch
Copyright (c) 2007-2008 Wolfgang Engel  http://code.google.com/p/oolongengine/

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#import "Renderer/Core/GraphicsDevice/OS2x/EAGLView.h"

@class EAGLCameraView;

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


@interface EAGLCameraView : EAGLView
{
	//TouchScreenValues TouchScreen;	
	
	TouchScreenValues CPY_TouchScreenValues[kMultiTouchScreenValues];
	
	int CountTouchesBegan;
	int CountTouchesMoved;
	int TouchCount;
	NSObject *renderLock;
	NSObject *touchScreenLock;
}

@property (readwrite, assign) NSObject *renderLock;

//- (TouchScreenValues*) GetValuesTouchScreen;
- (TouchScreenValues*)GetValuesMultiTouchScreen;
- (int) GetCountTouchesBegan;
- (int) GetCountTouchesMoved;
- (int) GetTouchCount;

@end


// C wrapper
//TouchScreenValues* GetValuesTouchScreen();
TouchScreenValues* GetValuesMultiTouchScreen();
int GetCountTouchesBegan();
int GetCountTouchesMoved();
int GetTouchCount();

