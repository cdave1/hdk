/*
 *  hdOSXSystemSettings.h
 *  TotemGameOSX
 *
 *  Created by david on 25/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_OSX_SYSTEM_SETTINGS
#define HD_OSX_SYSTEM_SETTINGS

#include "mach-o/arch.h"

#import "hdSystemSettings.h"

#import <Foundation/Foundation.h>

#import <sys/utsname.h>
#import <sys/types.h>
#import <sys/sysctl.h>



@interface hdOSXSystemSettings : NSObject 
{}


@end


#endif