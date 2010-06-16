//
//  InterfaceBuilder.h
//  Sierra
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "hdInterfaceBuilderController.h"

#define LOCK_GL_CONTEXT [[InterfaceBuilder editorContextLock] lock]
#define UNLOCK_GL_CONTEXT [[InterfaceBuilder editorContextLock] unlock]

#define kViewControllerWasLoadedNotification @"kViewControllerWasLoadedNotification"
#define kViewControllerWasUnloadedNotification @"kViewControllerWasUnloadedNotification"
#define kSelectedTextureImageChanged @"kSelectedTextureImageChanged"

#define kLoadedViewControllerWasChangedNotification @"kLoadedViewControllerWasChangedNotification"
#define kSingleItemSelectedNotification @"kSingleItemSelectedNotification"
#define kSingleItemWillBeUnselectedNotification @"kSingleItemWillBeUnselectedNotification"
#define kSingleItemWasUnselectedNotification @"kSingleItemWasUnselectedNotification"
#define kMultipleItemsSelectedNotification @"kMultipleItemsSelectedNotification"

@interface InterfaceBuilder : NSObject 

+ (hdInterfaceBuilderController *)sharedInstance;

+ (NSLock *)editorContextLock;

@end
