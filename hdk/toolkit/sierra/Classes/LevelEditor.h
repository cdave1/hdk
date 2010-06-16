//
//  LevelEditor.h
//  Sierra
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#include "hdLevelEditorController.h"

#define LOCK_GL_CONTEXT [[LevelEditor editorContextLock] lock]
#define UNLOCK_GL_CONTEXT [[LevelEditor editorContextLock] unlock]

#define kWorldWasLoadedNotification @"kWorldWasLoadedNotification"
#define kWorldWasUnloadedNotification @"kWorldWasUnloadedNotification"


#define kLoadedLevelWasChangedNotification @"kLoadedLevelWasChangedNotification"
#define kSingleItemSelectedNotification @"kSingleItemSelectedNotification"
#define kSingleItemUnselectedNotification @"kSingleItemUnselectedNotification"
#define kMultipleItemsSelectedNotification @"kMultipleItemsSelectedNotification"
#define kLevelWasDeletedNotification @"kLevelWasDeletedNotification"

@interface LevelEditor : NSObject 

+ (hdLevelEditorController *)sharedInstance;

+ (NSLock *)editorContextLock;

@end
