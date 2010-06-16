//
//  MainMenuDelegate.m
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "AppDelegateMenu.h"
#import "hdk.h"
#import "InterfaceBuilder.h"

@implementation AppDelegate (MainMenuCategory)


- (void)fileSystemError:(NSString *)reason withFileName:(NSString *)fileName
{
	NSWindow *infoWindow;
	
	infoWindow = NSGetCriticalAlertPanel(reason,
										 [NSString stringWithFormat:@"File: %@", fileName],
										 @"OK", nil, nil );
	[NSApp runModalForWindow:infoWindow];
	[infoWindow close];
}


- (IBAction)openNewWorld:(id)sender
{
	NSOpenPanel *open = [NSOpenPanel openPanel];
	NSURL *fileURL = 
		[NSURL fileURLWithPath:[NSString stringWithUTF8String:FileSystem_BaseDir()]];
	
	NSString *hibFileExtension = [NSString stringWithUTF8String:HD_VIEWCONTROLLER_FILE_EXTENSION];
	NSString *worldExtension = [NSString stringWithFormat:@".%@", hibFileExtension];
	
	hdPrintf("Opening folder: %s\n", [[fileURL absoluteString] UTF8String]);
	[open setDirectoryURL:fileURL];
	[open setAllowedFileTypes:[NSArray arrayWithObject:hibFileExtension]];
	[open setCanChooseDirectories:NO]; 
	[open setCanChooseFiles:YES]; 
	[open setCanCreateDirectories:NO]; 
	[open setAllowsMultipleSelection:NO];
	[open setAlphaValue:0.95]; 
	[open setTitle:@"Select a world file"];
	
	//if ([open runModalForDirectory:[NSString stringWithUTF8String:FileSystem_BaseDir()] file:nil])
	if ([open runModal])
	{
		NSArray *urls = [open URLs];
		if (urls.count == 1)
		{
			NSString *fileName = [[urls objectAtIndex:0] lastPathComponent];
			if ([fileName hasSuffix:worldExtension])
			{
				NSString *p = [[urls objectAtIndex:0] path];
				if (![InterfaceBuilder sharedInstance]->LoadViewController([p UTF8String]))
				{
					[self fileSystemError:@"Could not load the specified totem world file." withFileName:fileName];
					return;
				}
			} 
			else
			{
				[self fileSystemError:@"The specifed file was not a totem world file." withFileName:fileName];
				return;
			}
			
			[[NSNotificationCenter defaultCenter] 
			 postNotificationName:kViewControllerWasLoadedNotification
			 object:nil];
		}
	}
}


- (IBAction)save:(id)sender
{
	[self showProgressPanel:@"Saving world..."];
	if (![InterfaceBuilder sharedInstance]->SaveCurrentViewController())
	{
		[self fileSystemError:@"Could not save the current totem world file" withFileName:@"Current file"];
	}
	[self hideProgressPanel];
}


- (IBAction)saveAs:(id)sender
{
	// Need to open a file picker
	NSSavePanel *savePanel = [NSSavePanel savePanel];
	NSURL *fileURL = 
		[NSURL fileURLWithPath:[NSString stringWithUTF8String:FileSystem_BaseDir()]];
	NSString *hibFileExtension = [NSString stringWithUTF8String:HD_VIEWCONTROLLER_FILE_EXTENSION];
	NSString *worldExtension = [NSString stringWithFormat:@".%@", hibFileExtension];
	
	hdPrintf("Saving file\n");
	[savePanel setDirectoryURL:fileURL];
	[savePanel setAllowedFileTypes:[NSArray arrayWithObject:hibFileExtension]];
	[savePanel setCanCreateDirectories:YES];
	[savePanel setTitle:@"Save current world file"];
	
	if ([savePanel runModal])
	{
		if ([savePanel URL])
		{
			NSString *fileName = [[savePanel URL] lastPathComponent];
			// can't save a file that already exists
			if (FileSystem_FileExists([[[savePanel URL] path] UTF8String]))
			{
				[self fileSystemError:@"Can't save over file that already exists." withFileName:fileName];
				return;
			}
			else if (![fileName hasSuffix:worldExtension])
			{
				[self fileSystemError:@"The specifed file was not a hib file." withFileName:fileName];
				return;
			} 
			else
			{
				NSString *p = [[savePanel URL] path];
				if ((![InterfaceBuilder sharedInstance]->SaveCurrentViewControllerTo([p UTF8String])))
				{
					[self fileSystemError:@"Could not save the specified hib file." withFileName:fileName];
					return;
				}
			}
		}		
	}
}



@end
