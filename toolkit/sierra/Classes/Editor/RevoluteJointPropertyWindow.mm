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

#import "RevoluteJointPropertyWindow.h"
#import "LevelEditor.h"
#import "NSImageView+Extensions.h"

@interface RevoluteJointPropertyWindow ()
- (void)updateInterfaceState:(totemJoint *)jnt;
@end

@implementation RevoluteJointPropertyWindow

- (id) init
{
    if ((self = [super initWithWindowNibName:@"RevoluteJointPropertyWindow"]))
    {
        [[NSNotificationCenter defaultCenter]
         addObserver:self
         selector:@selector(itemWasSelected)
         name:kSingleItemSelectedNotification
         object:nil];

        [[NSNotificationCenter defaultCenter]
         addObserver:self
         selector:@selector(itemWasUnselected)
         name:kSingleItemUnselectedNotification
         object:nil];

        [self.window orderOut:self];
    }
    return self;
}


- (void)itemWasSelected
{
    const hdGameObject *selected;
    if (!(selected = [LevelEditor sharedInstance]->GetSelectedGameObjectSingle()))
    {
        hdPrintf("No game object was selected.");
        return;
    }

    if (selected->GetUserType() != e_totemTypeJoint)
    {
        [self.window orderOut:self];
        return;
    }

    [self.window orderBack:self];
    [self updateInterfaceState:(totemJoint *)selected];
}


- (void)updateInterfaceState:(totemJoint *)jnt
{
    if (jnt->GetJointType() != e_totemJointTypeRevolute)
    {
        [self.window orderOut:self];
        return;
    }

    [_motorEnabledCheckbox setState:(jnt->GetJointMotorEnabled() ? NSOnState : NSOffState)];
    if (jnt->GetJointMotorEnabled())
    {
        [_motorSpeedTextField setEnabled:YES];
        [_motorSpeedTextField setFloatValue:jnt->GetJointMotorSpeed()];

        [_maxTorqueTextField setEnabled:YES];
        [_maxTorqueTextField setFloatValue:jnt->GetRevoluteJointMaxTorque()];
    }
    else
    {
        [_motorSpeedTextField setEnabled:NO];
        [_maxTorqueTextField setEnabled:NO];
    }

    [_limitEnabledCheckbox setState:(jnt->GetJointLimitEnabled() ? NSOnState : NSOffState)];
    if (jnt->GetJointLimitEnabled())
    {
        [_lowerAngleSlider setEnabled:YES];
        [_lowerAngleSlider setFloatValue:jnt->GetRevoluteJointLowerAngle()];

        [_lowerAngleTextField setEnabled:YES];
        [_lowerAngleTextField setFloatValue:jnt->GetRevoluteJointLowerAngle()];

        [_upperAngleSlider setEnabled:YES];
        [_upperAngleSlider setFloatValue:jnt->GetRevoluteJointUpperAngle()];

        [_upperAngleTextField setEnabled:YES];
        [_upperAngleTextField setFloatValue:jnt->GetRevoluteJointUpperAngle()];
    }
    else
    {
        [_lowerAngleSlider setEnabled:NO];
        [_lowerAngleTextField setEnabled:NO];

        [_upperAngleSlider setEnabled:NO];
        [_upperAngleTextField setEnabled:NO];
    }

    [_lowerAngleSlider setMinValue:-(double)hd_pi];
    [_lowerAngleSlider setMaxValue:(double)hd_pi];

    [_upperAngleSlider setMinValue:-(double)hd_pi];
    [_upperAngleSlider setMaxValue:(double)hd_pi];
}


- (IBAction)valueWasChanged:(id)sender
{
    const hdGameObject *selected;
    if (!(selected = [LevelEditor sharedInstance]->GetSelectedGameObjectSingle())) return;

    totemJoint* jnt = (totemJoint *)selected;
    if (jnt->GetJointType() == e_totemJointTypeRevolute)
    {
        bool enableMotor, enableLimit;
        float motorSpeed, maxMotorTorque, lowerAngle, upperAngle;

        enableMotor = [_motorEnabledCheckbox state] == NSOnState;
        enableLimit = [_limitEnabledCheckbox state] == NSOnState;

        motorSpeed		= (float) [_motorSpeedTextField floatValue];
        maxMotorTorque	= (float) [_maxTorqueTextField floatValue];
        
        if (enableLimit)
        {
            lowerAngle = [_lowerAngleSlider floatValue];
            upperAngle = [_upperAngleSlider floatValue];
        }
        else
        {
            lowerAngle = upperAngle = 0.0f;
        }
        
        [_lowerAngleTextField setFloatValue:[_lowerAngleSlider floatValue]];
        [_upperAngleTextField setFloatValue:[_upperAngleSlider floatValue]];
        
        jnt->SetRevoluteJointProperties(enableMotor, enableLimit, lowerAngle, upperAngle, maxMotorTorque, motorSpeed);
    }
    [self updateInterfaceState:jnt];
}


- (void)itemWasUnselected
{
    [self.window orderOut:self];
}

@end
