//
//  JointPropertyWindow.mm
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "RevoluteJointPropertyWindow.h"
#import "LevelEditor.h"
#import "NSImageView+Extensions.h"

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
	
	totemJoint* jnt = (totemJoint *)selected;
	if (jnt->GetJointType() != e_totemJointTypeRevolute)
	{
		[self.window orderOut:self];
		return;
	}
	[self.window orderBack:self];
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
		enableLimit = [_limitEnabledCheckbox state] == NSOffState;
		
		motorSpeed		= (float) [_motorSpeedTextField floatValue];
		maxMotorTorque	= (float) [_maxTorqueTextField floatValue];
		
		if (enableLimit)
		{
			lowerAngle		= (float) [_lowerAngleSlider floatValue];
			upperAngle		= (float) [_upperAngleSlider floatValue];
		}
		else
		{
			lowerAngle = upperAngle = 0.0f;
		}
		
		[_lowerAngleTextField setFloatValue:lowerAngle];
		[_upperAngleTextField setFloatValue:upperAngle];
		
		jnt->SetRevoluteJointProperties(enableMotor, enableLimit, lowerAngle, upperAngle, maxMotorTorque, motorSpeed);
	}
}


- (void)itemWasUnselected
{
	[self.window orderOut:self];
}

@end
