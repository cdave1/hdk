//
//  JointPropertyWindow.h
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface RevoluteJointPropertyWindow : NSWindowController 
{
	IBOutlet NSButton *_motorEnabledCheckbox;
	IBOutlet NSTextField *_motorSpeedTextField;
	IBOutlet NSTextField *_maxTorqueTextField;
	IBOutlet NSButton *_limitEnabledCheckbox;
	IBOutlet NSSlider *_lowerAngleSlider;
	IBOutlet NSTextField *_lowerAngleTextField;
	IBOutlet NSSlider *_upperAngleSlider;
	IBOutlet NSTextField *_upperAngleTextField;
}
- (IBAction)valueWasChanged:(id)sender;
- (void)itemWasSelected;
- (void)itemWasUnselected;
@end
