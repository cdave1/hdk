//
//  IBPropertyWindow.h
//  Tango
//
//  Created by David Petrie on 15/06/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>
#import "hdk.h"
#import "hdIB.h"

@interface IBPropertyWindow : NSWindowController 
{
@protected
	IBOutlet NSTextField *_proxyTag;
	IBOutlet NSTextField *_proxyGroup;
	IBOutlet NSSlider *_zOffset;
	IBOutlet NSTextField *_zOffsetText;
}

- (id) initWithWindowNibName:(NSString *)nibName;


- (void)itemWasSelected;


- (void)itemWillBeUnselected;


- (void)itemWasUnselected;


- (void)multipleItemsSelected;


- (void)updateTextInterface;


- (hdInterfaceBuilderProxyType_t)getInterfaceBuilderProxyType;


- (IBAction)UpdateTag:(id)sender;


- (IBAction)UpdateTag:(id)sender;


- (IBAction)UpdateGroup:(id)sender;


- (IBAction)UpdateZOffset:(id)sender;

@end
