/*
 *  hdGraphicsLibrary
 */

#import "hdGLESSurface.h"
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>

@interface hdGLES1Surface : hdGLESSurface <hdGLESAbstractSurface>
{
@private
	GLuint _format;
	GLuint _depthFormat;
	GLuint _framebuffer;
	GLuint _renderbuffer;
	GLuint _depthBuffer;
}

- (id) init;
- (id) initWithPixelFormat:(GLuint)format;
- (id) initWithPixelFormat:(GLuint)format 
			   depthFormat:(GLuint)depth 
		preserveBackbuffer:(BOOL)retained;

@property(readonly) GLuint framebuffer;
@property(readonly) GLuint pixelFormat;
@property(readonly) GLuint depthFormat;

@end