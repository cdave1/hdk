//
//  IBButtonPropertyWindow.mm
//  Tango
//
//  Created by David Petrie on 15/06/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "IBButtonPropertyWindow.h"
#import "InterfaceBuilder.h"
#import "NSImageView+Extensions.h"
#import "NSWindowController+Extensions.h"

@implementation IBButtonPropertyWindow

- (id) init
{
	if ((self = [super initWithWindowNibName:@"IBButtonPropertyWindow"]))
	{
		NSArray *draggedTextureFileTypes = [NSArray arrayWithObjects:
											NSCreateFileContentsPboardType(@"tga"),
											NSCreateFileContentsPboardType(@"png"),nil];
		
		[_normalTexture registerForDraggedTypes:draggedTextureFileTypes];
		[_overTexture registerForDraggedTypes:draggedTextureFileTypes];
		[_upTexture registerForDraggedTypes:draggedTextureFileTypes];
	}
	return self;
}


- (void)itemWasSelected
{
	hdIBProxy *selected;
	if (!(selected = (hdIBProxy *)[InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectSingle()))
	{
		hdPrintf("No game object was selected.");
		return;
	}
	
	if (selected->GetInterfaceBuilderProxyType() != hdInterfaceBuilderProxyTypeButton)
	{
		[self.window orderOut:self];
		return;
	}
	
	[self.window orderBack:self];
	[self setAllEnabled:YES];
	
	[_normalTexture setEnabled:YES];
	[_normalTexture setImageAtContentRepositoryPath:((hdIBButton *)selected)->GetNormalTexture()];
	
	[_overTexture setEnabled:YES];
	[_overTexture setImageAtContentRepositoryPath:((hdIBButton *)selected)->GetOverTexture()];
	
	[_upTexture setEnabled:YES];
	[_upTexture setImageAtContentRepositoryPath:((hdIBButton *)selected)->GetUpTexture()];
	
	/*
	 if (((hdIBImage *)selected)->GetTint())
	 {
	 NSColor *color = [NSColor colorWithDeviceRed:((hdIBImage *)selected)->GetTint()[0]
	 green:((hdIBImage *)selected)->GetTint()[1]
	 blue:((hdIBImage *)selected)->GetTint()[2]
	 alpha:((hdIBImage *)selected)->GetTint()[3]];
	 [_tint setColor:color];
	 } 
	 else 
	 {
	 [_tint setColor:[NSColor whiteColor]];
	 }*/
	
	[_proxyTag setStringValue:[NSString stringWithUTF8String:selected->GetTag()]];
	[_proxyGroup setStringValue:[NSString stringWithUTF8String:selected->GetGroup()]];
	[_zOffset setFloatValue:((hdIBImage *)selected)->GetZOffset()];
	
	[self updateTextInterface];
}


- (void)multipleItemsSelected
{
	[self.window orderOut:self];
}


- (void)itemWasUnselected
{
	[super itemWasUnselected];
}


- (NSDragOperation)draggingEntered:(id < NSDraggingInfo >)sender
{
	return NSDragOperationGeneric;
}


- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender
{
	return YES;
}


- (hdInterfaceBuilderProxyType_t)getInterfaceBuilderProxyType
{
	return hdInterfaceBuilderProxyTypeButton;
}


- (IBAction)UpdateSlidersFromText:(id)sender
{
	[_zOffset setFloatValue:[_zOffsetText floatValue]];
	[self UpdateZOffset:nil];
}


/**
 * Fired by drag and drop event onto imageview
 */
- (IBAction)UpdateNormalButtonTexture:(id)sender
{
	const char *textureName = [_normalTexture getResourcePathOfImage];
	if (!textureName) return;
	
	for (int i = 0; i < [InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectsCount(); ++i)
	{
		if ([InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i)->GetInterfaceBuilderProxyType() == [self getInterfaceBuilderProxyType])
		{
			hdIBButton * selected = (hdIBButton *)[InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i);
			if (selected)
			{
				selected->SetNormalTexture(textureName);
				selected->ResetTextureCoords();
			}
		}
	}
}


- (IBAction)UpdateOverButtonTexture:(id)sender
{
	const char *textureName = [_overTexture getResourcePathOfImage];
	if (!textureName) return;
	
	for (int i = 0; i < [InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectsCount(); ++i)
	{
		if ([InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i)->GetInterfaceBuilderProxyType() == [self getInterfaceBuilderProxyType])
		{
			hdIBButton * selected = (hdIBButton *)[InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i);
			if (selected)
			{
				selected->SetOverTexture(textureName);
				selected->ResetTextureCoords();
			}
		}
	}
}


- (IBAction)UpdateUpButtonTexture:(id)sender
{
	const char *textureName = [_upTexture getResourcePathOfImage];
	if (!textureName) return;
	
	for (int i = 0; i < [InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectsCount(); ++i)
	{
		if ([InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i)->GetInterfaceBuilderProxyType() == [self getInterfaceBuilderProxyType])
		{
			hdIBButton * selected = (hdIBButton *)[InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i);
			if (selected)
			{
				selected->SetUpTexture(textureName);
				selected->ResetTextureCoords();
			}
		}
	}
}


- (void)updateTextInterface
{
	[_zOffsetText setFloatValue:[_zOffset floatValue]];
}

@end
