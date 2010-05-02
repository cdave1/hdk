//
//  MainMenuDelegate.h
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "AppDelegate.h"

/**
 * Category for main menu functions, purely so we don't clutter AppDelegate.mm with thousands of
 * lines of crap.
 */
@interface AppDelegate (MainMenuCategory)

- (void)fileloadError:(NSString *)reason withFileName:(NSString *)fileName;

- (IBAction)openNewWorld:(id)sender;

- (IBAction)save:(id)sender;


/**
 * Physics menu items
 */
- (IBAction)physicsOn:(id)sender;

- (IBAction)physicsOff:(id)sender;

@end
