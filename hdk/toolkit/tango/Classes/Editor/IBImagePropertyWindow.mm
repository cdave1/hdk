//
//  IBImagePropertyWindow.mm
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "IBImagePropertyWindow.h"
#import "InterfaceBuilder.h"
#import "NSImageView+Extensions.h"
#import "NSWindowController+Extensions.h"

@implementation IBImagePropertyWindow

- (id) init
{
	if ((self = [super initWithWindowNibName:@"IBImagePropertyWindow"]))
	{		
		[_texture registerForDraggedTypes:[NSArray arrayWithObjects:NSCreateFileContentsPboardType(@"tga"),NSCreateFileContentsPboardType(@"png"),nil]];
		//[self.window setDraggingDestinationDelegate:self];
		
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
	
	if (selected->GetInterfaceBuilderProxyType() != hdInterfaceBuilderProxyTypeImage)
	{
		[self.window orderOut:self];
		return;
	}
	[self.window orderBack:self];
	[self setAllEnabled:YES];
	
	[_texture setEnabled:YES];
	[_texture setImageAtContentRepositoryPath:((hdIBImage *)selected)->GetTexture()];
	[_textureName setStringValue:[NSString stringWithUTF8String:((hdIBImage *)selected)->GetTexture()]];
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
	//[_zOffset setFloatValue:((hdIBImage *)selected)->GetZOffset()];
	[_tiling setFloatValue:((hdIBImage *)selected)->GetTextureRepeatX()];
	
	[self updateTextInterface];
}


- (void)multipleItemsSelected
{
	if (![InterfaceBuilder sharedInstance]->SelectedItemsContainsType(hdInterfaceBuilderProxyTypeImage)) return;
	
	[self.window orderBack:self];
	[self setAllEnabled:NO];
	
	[_texture setEnabled:YES];
	[_textureName setEnabled:YES];
	[_tint setEnabled:YES];
	[_zOffset setEnabled:YES];
	[_proxyTag setEnabled:YES];
	[self updateTextInterface];
}


- (void)itemWasUnselected
{
	[super itemWasUnselected];
}


- (IBAction)valueWasChanged:(id)sender
{
	
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
	return hdInterfaceBuilderProxyTypeImage;
}


- (IBAction)UpdateSlidersFromText:(id)sender
{
	[_zOffset setFloatValue:[_zOffsetText floatValue]];
	[self UpdateZOffset:nil];
	[_tiling setFloatValue:[_tilingText floatValue]];
	[self UpdateImageTiling:nil];
}


/**
 * Fired by drag and drop event onto imageview
 */
- (IBAction)UpdateImageTexture:(id)sender
{
	hdIBImage * selected = NULL;
	const char *textureName = [_texture getResourcePathOfImage];
	if (!textureName) return;
	
	for (int i = 0; i < [InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectsCount(); ++i)
	{
		if ([InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i)->GetInterfaceBuilderProxyType() == [self getInterfaceBuilderProxyType])
		{
			selected = (hdIBImage *)[InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i);
			if (selected)
			{
				selected->SetTexture(textureName);
				selected->ResetTextureCoords();
			}
		}
	}
	[_textureName setStringValue:[NSString stringWithUTF8String:textureName]];
}


- (IBAction)UpdateImageTiling:(id)sender
{
	hdVec3 dim;
	float hwRatio, xTiling;
	hdIBImage * selected = NULL;
	
	for (int i = 0; i < [InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectsCount(); ++i)
	{
		if ([InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i)->GetInterfaceBuilderProxyType() == [self getInterfaceBuilderProxyType])
		{
			selected = (hdIBImage *)[InterfaceBuilder sharedInstance]->GetSelectedInterfaceObjectAtIndex(i);
			dim = (selected->GetAABB().upper - selected->GetAABB().lower);
			
			if (dim.x <= 0.0f) return;
			
			hwRatio = (dim.y / dim.x);
			xTiling		= [_tiling floatValue];
			
			if (xTiling < 0.01f) xTiling = 1.0f;
			
			selected->SetTextureRepeatX(xTiling);
			selected->SetTextureRepeatY(xTiling);
			
			selected->ResetTextureCoords();
		}
	}
	[self updateTextInterface];
}



- (void)updateTextInterface
{
	[_zOffsetText setFloatValue:[_zOffset floatValue]];
	[_tilingText setFloatValue:[_tiling floatValue]];
}


#pragma mark -
#pragma mark NSComboBoxDelegate functions

- (void)comboBoxSelectionDidChange:(NSNotification *)notification
{
	NSLog(@"%@", [notification name]);
}


- (void)comboBoxWillDismiss:(NSNotification *)notification
{
	NSLog(@"%@", [notification name]);
}


@end
