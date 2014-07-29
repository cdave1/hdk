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

#import "AppDelegate.h"

/**
 * Category for main menu functions, purely so we don't clutter AppDelegate.mm with thousands of
 * lines of crap.
 */
@interface AppDelegate (MainMenuCategory)

- (void)fileSystemError:(NSString *)reason withFileName:(NSString *)fileName;

- (IBAction)openNewWorld:(id)sender;

- (IBAction)save:(id)sender;

- (IBAction)saveAs:(id)sender;

/**
 * Physics menu items
 */
- (IBAction)physicsOn:(id)sender;

- (IBAction)physicsOff:(id)sender;

@end
