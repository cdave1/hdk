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

#define kLevelWasLoadedNotification @"kLevelWasLoadedNotification"
#define kSingleItemSelectedNotification @"kSingleItemSelectedNotification"
#define kSingleItemUnselectedNotification @"kSingleItemUnselectedNotification"
#define kMultipleItemsSelectedNotification @"kMultipleItemsSelectedNotification"


@interface LevelEditor : NSObject 

+ (hdLevelEditorController *)sharedInstance;

+ (NSLock *)editorContextLock;

@end
