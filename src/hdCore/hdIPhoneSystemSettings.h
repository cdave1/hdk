/*
 *  hdIPhoneSystemSettings.h
 *  TotemGame
 *
 *  Created by david on 12/09/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_IPHONE_SYSTEM_SETTINGS
#define HD_IPHONE_SYSTEM_SETTINGS

#ifdef __arm__
#include "mach-o/arch.h"
#endif

#import "hdSystemSettings.h"

#import <Foundation/Foundation.h>

#import <sys/utsname.h>
#import <sys/types.h>
#import <sys/sysctl.h>



#define FIRSTGEN_IPHONE_FRAME_DURATION 0.032f
#define SECONDGEN_IPHONE_FRAME_DURATION 0.025f
#define THIRDGEN_IPHONE_FRAME_DURATION 0.016f

@interface hdIPhoneSystemSettings : NSObject 
{
	
}


/*
 * Determine the system name
 */
//+ (NSString *)GetSystemName;


/*
 * FPS
 */
//+ (double)GetFrameDuration;


/*
 * Texture BPP detail
 */
//+ (int)GetMaxTextureBPP;



@end


#endif