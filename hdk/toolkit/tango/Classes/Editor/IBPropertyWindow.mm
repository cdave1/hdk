//
//  IBPropertyWindow.mm
//  Tango
//
//  Created by David Petrie on 15/06/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "IBPropertyWindow.h"
#import "InterfaceBuilder.h"
#import "NSImageView+Extensions.h"
#import "NSWindowController+Extensions.h"

@implementation IBPropertyWindow

- (id) initWithWindowNibName:(NSString *)nibName
{
	if ((self = [super initWithWindowNibName:nibName]))
	{
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(itemWasSelected)
		 name:kSingleItemSelectedNotification
		 object:nil];
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(itemWillBeUnselected)
		 name:kSingleItemWillBeUnselectedNotification
		 object:nil];
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(itemWasUnselected)
		 name:kSingleItemWasUnselectedNotification
		 object:nil];
		
		[[NSNotificationCenter defaultCenter]
		 addObserver:self
		 selector:@selector(multipleItemsSelected)
		 name:kMultipleItemsSelectedNotification
		 object:nil];
		
		// initially hidden
		[self.window orderOut:self];
		
		if ([self.window respondsToSelector:@selector(setFloatingPanel:)])
		{
			[((NSPanel *)self.window) setFloatingPanel:YES];
		}
	}
	return self;
}


- (void)itemWasSelected
{
}


- (void)itemWillBeUnselected
{
	[self UpdateTag:nil];
	[self UpdateGroup:nil];
}


- (void)itemWasUnselected
{
	
	[self.window orderOut:self];
}


- (void)multipleItemsSelected
{
}


- (void)updateTextInterface {}


- (hdInterfaceBuilderProxyType_t)getInterfaceBuilderProxyType
{
	return hdInterfaceBuilderProxyTypeNone;
}


- (IBAction)UpdateZOffset:(id)sender
{
	float zOffset;
	hdIBProxy * selected = NULL;
	for (int i = 0; i < [InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectsCount(); ++i)
	{
		if ([InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i)->GetInterfaceBuilderProxyType() == [self getInterfaceBuilderProxyType])
		{
			selected = [InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i);
			zOffset = [_zOffset floatValue];
			zOffset = hdClamp(zOffset, -10.0f, 10.0f);
			selected->SetZOffset(zOffset);
		}
	}
	[self updateTextInterface];
}


- (IBAction)UpdateTag:(id)sender
{
	hdIBProxy * selected = NULL;
	for (int i = 0; i < [InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectsCount(); ++i)
	{
		if ([InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i)->GetInterfaceBuilderProxyType() == [self getInterfaceBuilderProxyType])
		{
			selected = [InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i);
			selected->SetTag([[_proxyTag stringValue] UTF8String]);
		}
	}
}


- (IBAction)UpdateGroup:(id)sender
{
	hdIBProxy * selected = NULL;
	for (int i = 0; i < [InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectsCount(); ++i)
	{
		if ([InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i)->GetInterfaceBuilderProxyType() == [self getInterfaceBuilderProxyType])
		{
			selected = [InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i);
			selected->SetGroup([[_proxyGroup stringValue] UTF8String]);
		}
	}
}

@end
