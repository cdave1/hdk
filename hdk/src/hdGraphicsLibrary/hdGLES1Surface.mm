/*
 *  hdGraphicsLibrary
 */

#include "hdGLES1Surface.h"

@implementation hdGLES1Surface


@synthesize framebuffer = _framebuffer,
	pixelFormat = _format,
	depthFormat = _depthFormat;

- (id) init
{
	return [self initWithPixelFormat:GL_RGB565_OES 
						 depthFormat:GL_DEPTH_COMPONENT16_OES 
				  preserveBackbuffer:NO];
}


- (id) initWithPixelFormat:(GLuint)format 
{
	return [self initWithPixelFormat:format 
				   depthFormat:0 
			preserveBackbuffer:NO];
}


- (id) initWithPixelFormat:(GLuint)format depthFormat:(GLuint)depth preserveBackbuffer:(BOOL)retained
{
	if((self = [super init])) 
	{
		_format = format;
		_depthFormat = depth;
		
		_context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1];
		
		if(!_context || ![EAGLContext setCurrentContext:_context]) 
		{
			[self release];
			return nil;
		}
	}
	
	return self;
}


- (void) dealloc
{
	[self destroySurface];
	
	[_context release];
	_context = nil;
	
	[super dealloc];
}


- (BOOL)createSurface
{
	CAEAGLLayer *eaglLayer;
	CGSize newSize;
	GLuint oldRenderbuffer;
	GLuint oldFramebuffer;
	
	if(![EAGLContext setCurrentContext:_context]) 
	{
		return NO;
	}
	
	if (!_delegate)
	{
		return NO;
	}
	
	eaglLayer = [_delegate getLayer];
	
	newSize = [eaglLayer bounds].size;
	newSize.width = roundf(newSize.width);
	newSize.height = roundf(newSize.height);
	
	glGetIntegerv(GL_RENDERBUFFER_BINDING_OES, (GLint *) &oldRenderbuffer);
	glGetIntegerv(GL_FRAMEBUFFER_BINDING_OES, (GLint *) &oldFramebuffer);
	
	glGenRenderbuffersOES(1, &_renderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, _renderbuffer);
	
	if(![_context renderbufferStorage:GL_RENDERBUFFER_OES fromDrawable:(id<EAGLDrawable>)eaglLayer]) {
		glDeleteRenderbuffersOES(1, &_renderbuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_BINDING_OES, oldRenderbuffer);
		return NO;
	}
	
	glGenFramebuffersOES(1, &_framebuffer);
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, _framebuffer);
	glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_COLOR_ATTACHMENT0_OES, GL_RENDERBUFFER_OES, _renderbuffer);
	if (_depthFormat) {
		glGenRenderbuffersOES(1, &_depthBuffer);
		glBindRenderbufferOES(GL_RENDERBUFFER_OES, _depthBuffer);
		glRenderbufferStorageOES(GL_RENDERBUFFER_OES, _depthFormat, newSize.width, newSize.height);
		glFramebufferRenderbufferOES(GL_FRAMEBUFFER_OES, GL_DEPTH_ATTACHMENT_OES, GL_RENDERBUFFER_OES, _depthBuffer);
	}
	
	if(!_hasBeenCurrent) {
		glViewport(0, 0, newSize.width, newSize.height);
		glScissor(0, 0, newSize.width, newSize.height);
		_hasBeenCurrent = YES;
	}
	else {
		glBindFramebufferOES(GL_FRAMEBUFFER_OES, oldFramebuffer);
	}
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, oldRenderbuffer);
	
	return YES;
}


- (void) destroySurface
{
	EAGLContext *oldContext = [EAGLContext currentContext];
	
	if (oldContext != _context)
		[EAGLContext setCurrentContext:_context];
	
	if(_depthFormat) {
		glDeleteRenderbuffersOES(1, &_depthBuffer);
		_depthBuffer = 0;
	}
	
	glDeleteRenderbuffersOES(1, &_renderbuffer);
	_renderbuffer = 0;
	
	glDeleteFramebuffersOES(1, &_framebuffer);
	_framebuffer = 0;
	
	if (oldContext != _context)
		[EAGLContext setCurrentContext:oldContext];
}


- (void) bindFrameBuffer
{
	CGRect _bounds = [_delegate getBounds];
	glBindFramebufferOES(GL_FRAMEBUFFER_OES, _framebuffer);
	glViewport(0, 0, _bounds.size.width, _bounds.size.height);
}


- (void) bindRenderBufferAndPresent
{
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, _renderbuffer);
	[_context presentRenderbuffer:GL_RENDERBUFFER_OES];
}


- (void) swapBuffers
{
	EAGLContext *oldContext = [EAGLContext currentContext];
	GLuint oldRenderbuffer;
	
	if(oldContext != _context)
	{
		[EAGLContext setCurrentContext:_context];
	}
	
	// CHECK_GL_ERROR();
	
	glGetIntegerv(GL_RENDERBUFFER_BINDING_OES, (GLint *) &oldRenderbuffer);
	glBindRenderbufferOES(GL_RENDERBUFFER_OES, _renderbuffer);
	
	if(![_context presentRenderbuffer:GL_RENDERBUFFER_OES])
	{
		printf("Failed to swap renderbuffer in %s\n", __FUNCTION__);
	}
	
	if(oldContext != _context)
	{
		[EAGLContext setCurrentContext:oldContext];
	}
}



@end