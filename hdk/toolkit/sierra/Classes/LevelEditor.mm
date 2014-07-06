//
//  LevelEditor.mm
//  Sierra
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "LevelEditor.h"

static hdLevelEditorController *_instance = nil;
static NSLock *_editorContextLock = nil;



@implementation LevelEditor

+ (hdLevelEditorController *)sharedInstance
{
	if (!_instance)
	{
		_instance = new hdLevelEditorController();
		FileSystem_Init("/Users/davidpetrie/Dropbox/Hackdirt/TotemGames/res");
	}
	return _instance;
}


+ (NSLock *)editorContextLock
{
	if (!_editorContextLock)
		_editorContextLock = [[NSLock alloc] init];
	return _editorContextLock;
}

@end
