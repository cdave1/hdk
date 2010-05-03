//
//  PrismaticJointPropertyWindow.h
//  Sierra
//
//  Created by David Petrie on 3/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface PrismaticJointPropertyWindow : NSWindowController 
{
	IBOutlet NSButton *_motorEnabledCheckbox;
	IBOutlet NSTextField *_motorSpeedTextField;
	IBOutlet NSTextField *_maxForceTextField;
	IBOutlet NSButton *_limitEnabledCheckbox;
	IBOutlet NSSlider *_lowerTranslationSlider;
	IBOutlet NSTextField *_lowerTranslationTextField;
	IBOutlet NSSlider *_upperTranslationSlider;
	IBOutlet NSTextField *_upperTranslationTextField;
}
- (IBAction)valueWasChanged:(id)sender;
- (void)itemWasSelected;
- (void)itemWasUnselected;
@end
