/*
 *  hdGraphicsLibrary
 */

#import <QuartzCore/QuartzCore.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/EAGLDrawable.h>

@protocol hdGLESSurfaceDelegate <NSObject>
@required
- (CAEAGLLayer *) getLayer;
- (CGRect) getBounds;
@end


@protocol hdGLESAbstractSurface <NSObject>
@required
- (BOOL) createSurface;
- (void) destroySurface;
- (void) bindFrameBuffer;
- (void) bindRenderBufferAndPresent;
- (void) swapBuffers;
@end


@interface hdGLESSurface : NSObject
{
@protected
	EAGLContext *_context;
	BOOL _hasBeenCurrent;
	id<hdGLESSurfaceDelegate> _delegate;
}

- (void) setCurrentContext;
- (BOOL) isCurrentContext;
- (void) clearCurrentContext;

@property(readonly) EAGLContext *context;
@property(assign) id<hdGLESSurfaceDelegate> delegate;

@end