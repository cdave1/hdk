/*
 *  hdGraphicsLibrary
 */

#import "hdGLESSurface.h"
#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>



@class hdGLView;

@interface hdGLView : UIView <hdGLESSurfaceDelegate>
{
@private
	BOOL _autoresize;
	CGSize _size;
	hdGLESSurface<hdGLESAbstractSurface> *_surface;
}

@property BOOL autoresizesSurface;
@property(readonly, nonatomic) CGSize surfaceSize;

- (id) initWithFrame:(CGRect)frame;
- (void) swapBuffers;
- (void) setCurrentContext;

@end
