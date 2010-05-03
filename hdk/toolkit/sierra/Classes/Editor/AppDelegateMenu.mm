//
//  MainMenuDelegate.m
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "AppDelegateMenu.h"
#import "hdk.h"
#import "LevelEditor.h"

@implementation AppDelegate (MainMenuCategory)


- (void)fileloadError:(NSString *)reason withFileName:(NSString *)fileName
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
	
	NSString *worldExtension = [NSString stringWithUTF8String:TOTEM_WORLD_TEXTFILE_EXTENSION];
	
	hdPrintf("Opening folder: %s\n", [[fileURL absoluteString] UTF8String]);
	[open setDirectoryURL:fileURL];
	[open setAllowedFileTypes:[NSArray arrayWithObject:@"ttw"]];
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
				NSString *p = [[[urls objectAtIndex:0] path] 
							   stringByReplacingOccurrencesOfString:[NSString stringWithUTF8String:FileSystem_BaseDir()]
							   withString:@""];
				if (![LevelEditor sharedInstance]->LoadWorld([p UTF8String]))
				{
					[self fileloadError:@"Could not load the specified totem world file." withFileName:fileName];
					return;
				}
			} 
			else
			{
			
				[self fileloadError:@"The specifed file was not a totem world file." withFileName:fileName];
				return;
			}
			[[NSNotificationCenter defaultCenter] 
			 postNotificationName:kWorldWasLoadedNotification
			 object:nil];
		}
	}
}


- (IBAction)save:(id)sender
{
	[LevelEditor sharedInstance]->Save();
}


- (IBAction)physicsOn:(id)sender
{
	[LevelEditor sharedInstance]->PhysicsOn();
}


- (IBAction)physicsOff:(id)sender
{
	[LevelEditor sharedInstance]->PhysicsOff();
}


@end
