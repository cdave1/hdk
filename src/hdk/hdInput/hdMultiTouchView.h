/*
 * Copyright (c) 2014 Hackdirt Ltd.
 * Author: David Petrie (david@davidpetrie.com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software. Permission is granted to anyone to use this software for
 * any purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 * that you wrote the original software. If you use this software in a product, an
 * acknowledgment in the product documentation would be appreciated but is not
 * required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>

const int kMultiTouchScreenValues = 10;

typedef struct Touch
{
    float LocationXTouchBegan;    // x and y coordinates where the touch began
    float LocationYTouchBegan;

    float LocationXTouchMovedPrevious;
    float LocationYTouchMovedPrevious;

    float LocationXTouchMoved; // x and y coordinates where the touch moved
    float LocationYTouchMoved;

    float LocationXTouchEnded;
    float LocationYTouchEnded;

    NSTimeInterval TimeStamp;

    int TouchID;
    int TapCount;

    bool TouchDown;
    bool TouchUp;
    bool TouchMoved;
}
TouchScreenValues;


@interface hdMultiTouchView : GLKView
{
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
