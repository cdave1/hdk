/*
 *  hdGraphicsLibrary
 */


#import "hdGLESSurface.h"

@implementation hdGLESSurface

@synthesize delegate=_delegate,
	context = _context;


+ (Class) layerClass
{
	return [CAEAGLLayer class];
}

- (id) init
{
	if ((self = [super init]))
	{
		return self;
	}
	return nil;
}

- (void) setCurrentContext
{
	if(![EAGLContext setCurrentContext:_context]) {
		printf("Failed to set current context %p in %s\n", _context, __FUNCTION__);
	}
}

- (BOOL) isCurrentContext
{
	return ([EAGLContext currentContext] == _context ? YES : NO);
}

- (void) clearCurrentContext
{
	if(![EAGLContext setCurrentContext:nil])
		printf("Failed to clear current context in %s\n", __FUNCTION__);
}




@end