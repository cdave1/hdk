//
//  hdAccelerometer.h
//  hdk
//
//  Created by David Petrie on 11/04/10.
//  Copyright 2010 Hackdirt Ltd. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface Accel : NSObject <UIAccelerometerDelegate>
{
	UIAccelerationValue		_accelerometer[3];
}

- (void) SetupAccelerometer: (float) AcclerometerFrequency;
- (void) accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)Acceleration;
- (void) GetAccelerometerMatrix:(float *) matrix;
- (void) GetAccelerometerVector:(double *) AccelValue;
@end
