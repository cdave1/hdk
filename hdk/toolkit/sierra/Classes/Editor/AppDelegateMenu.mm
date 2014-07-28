/*
 * Copyright (c) 2014 Hackdirt Ltd.
 * Author: David Petrie (david@davidpetrie.com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software. Permission is granted to anyone to use this software for
 * any purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 * that you wrote the original software. If you use this software in a product, an
 * acknowledgment in the product documentation would be appreciated but is not
 * required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#import "AppDelegateMenu.h"
#import "hdk.h"
#import "LevelEditor.h"

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
                NSString *p = [[urls objectAtIndex:0] path];
                if (![LevelEditor sharedInstance]->LoadWorld([p UTF8String]))
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
             postNotificationName:kWorldWasLoadedNotification
             object:nil];
        }
    }
}


- (IBAction)save:(id)sender
{
    [self showProgressPanel:@"Saving world..."];
    if (![LevelEditor sharedInstance]->SaveCurrentWorld())
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
    NSString *worldExtension = [NSString stringWithUTF8String:TOTEM_WORLD_TEXTFILE_EXTENSION];

    hdPrintf("Saving file\n");
    [savePanel setDirectoryURL:fileURL];
    [savePanel setAllowedFileTypes:[NSArray arrayWithObject:@"ttw"]];
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
                [self fileSystemError:@"The specifed file was not a ttw file." withFileName:fileName];
                return;
            } 
            else
            {
                NSString *p = [[savePanel URL] path];
                if ((![LevelEditor sharedInstance]->SaveCurrentWorldTo([p UTF8String])))
                {
                    [self fileSystemError:@"Could not save the specified totem world file." withFileName:fileName];
                    return;
                }
            }
        }		
    }
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
