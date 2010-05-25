//
//  PrismaticJointPropertyWindow.m
//  Sierra
//
//  Created by David Petrie on 3/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "PrismaticJointPropertyWindow.h"
#import "LevelEditor.h"
#import "NSImageView+Extensions.h"

@interface PrismaticJointPropertyWindow ()
- (void)updateInterfaceState:(totemJoint *)jnt;
@end


@implementation PrismaticJointPropertyWindow

- (id) init
{
	if ((self = [super initWithWindowNibName:@"PrismaticJointPropertyWindow"]))
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
	if (jnt->GetJointType() != e_totemJointTypePrismatic)
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
		
		[_maxForceTextField setEnabled:YES];
		[_maxForceTextField setFloatValue:jnt->GetPrismaticJointMaxMotorForce()];
	}
	else
	{
		[_motorSpeedTextField setEnabled:NO];
		[_maxForceTextField setEnabled:NO];
	}
	
	[_limitEnabledCheckbox setState:(jnt->GetJointLimitEnabled() ? NSOnState : NSOffState)];
	if (jnt->GetJointLimitEnabled())
	{
		[_lowerTranslationSlider setEnabled:YES];
		[_lowerTranslationSlider setFloatValue:jnt->GetPrismaticJointLowerTranslation()];
		
		[_lowerTranslationTextField setEnabled:YES];
		[_lowerTranslationTextField setFloatValue:jnt->GetPrismaticJointLowerTranslation()];
		
		[_upperTranslationSlider setEnabled:YES];
		[_upperTranslationSlider setFloatValue:jnt->GetPrismaticJointUpperTranslation()];
		
		[_upperTranslationTextField setEnabled:YES];
		[_upperTranslationTextField setFloatValue:jnt->GetPrismaticJointUpperTranslation()];
	}
	else
	{
		[_lowerTranslationSlider setEnabled:NO];
		[_lowerTranslationTextField setEnabled:NO];
		
		[_upperTranslationSlider setEnabled:NO];
		[_upperTranslationTextField setEnabled:NO];
	}
}


- (void)itemWasUnselected
{
	[self.window orderOut:self];
}


- (IBAction)valueWasChanged:(id)sender
{
	const hdGameObject *selected;
	if (!(selected = [LevelEditor sharedInstance]->GetSelectedGameObjectSingle())) return;
	
	totemJoint* jnt = (totemJoint *)selected;
	if (jnt->GetJointType() == e_totemJointTypePrismatic)
	{
		bool enableMotor, enableLimit;
		float motorSpeed, maxMotorForce, lowerTranslation, upperTranslation;
		
		enableMotor = [_motorEnabledCheckbox state] == NSOnState;
		enableLimit = [_limitEnabledCheckbox state] == NSOnState;
		
		motorSpeed		= (float) [_motorSpeedTextField floatValue];
		maxMotorForce	= (float) [_maxForceTextField floatValue];
		
		if (enableLimit)
		{
			lowerTranslation = (float) [_lowerTranslationSlider floatValue];
			upperTranslation = (float) [_upperTranslationSlider floatValue];
		}
		else
		{
			lowerTranslation = upperTranslation = 0.0f;
		}
		
		jnt->SetPrismaticJointProperties(enableMotor, enableLimit, motorSpeed, lowerTranslation, upperTranslation, maxMotorForce);
		
		[_lowerTranslationTextField setFloatValue:lowerTranslation];
		[_upperTranslationTextField setFloatValue:upperTranslation];
	}
	[self updateInterfaceState:jnt];
}

@end
