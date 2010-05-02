/*
 *  hdMultiTouch.h
 *  hdk
 *
 *  Created by David Petrie on 11/04/10.
 *  Copyright 2010 Hackdirt Ltd. All rights reserved.
 *
 */

#ifndef HD_MULTITOUCH_H
#define HD_MULTITOUCH_H

#if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
#import "hdInput/hdMultiTouchView.h"
#import "hdInput/hdAccelerometer.h"
#else
#include "hdInput/hdMouseInput.h"
#endif

#endif