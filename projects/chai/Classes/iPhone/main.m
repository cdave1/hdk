#import <UIKit/UIKit.h>

int main(int argc, char *argv[])
{
	NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	UIApplicationMain(argc, argv, nil, @"AppController");
	[pool drain];
	
	return 0;
}
