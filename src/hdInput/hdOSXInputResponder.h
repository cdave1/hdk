/*
 *  hdOSXMouseInput.h
 *  Smashed
 *
 *  Created by david on 25/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_OSX_MOUSE_INPUT
#define HD_OSX_MOUSE_INPUT

#import <Cocoa/Cocoa.h>
#include "hdMouseInput.h"

@interface hdOSXInputResponder : NSResponder
{
	
}


//mousevalues_t returnedMouseValues;
- (mousevalues_t)GetMouseValues;

@end


#endif