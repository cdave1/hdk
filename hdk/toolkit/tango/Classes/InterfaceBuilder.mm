//
//  InterfaceBuilder.mm
//  Sierra
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "InterfaceBuilder.h"

static hdInterfaceBuilderController *_instance = nil;
static NSLock *_editorContextLock = nil;



@implementation InterfaceBuilder

+ (hdInterfaceBuilderController *)sharedInstance
{
	if (!_instance)
	{
		_instance = new hdInterfaceBuilderController();
		FileSystem_Init("/Users/david/Projects/Hackdirt/TotemGames/res");
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
