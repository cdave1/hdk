/*
 *  hdGraphicsLibrary
 */


#import "hdGLView.h"
#import "hdGLES1Surface.h"
#import "hdGLES2Surface.h"

@implementation hdGLView

@synthesize autoresizesSurface = _autoresize,
					surfaceSize=_size;


+ (Class)layerClass
{
    return [CAEAGLLayer class];
}


- (id) initWithFrame:(CGRect)frame
{
	if((self = [super initWithFrame:frame])) 
	{
		CAEAGLLayer* eaglLayer = (CAEAGLLayer*)self.layer;
		
		eaglLayer.drawableProperties = [NSDictionary dictionaryWithObjectsAndKeys:
										[NSNumber numberWithBool:YES], 
										kEAGLDrawablePropertyRetainedBacking, 
										kEAGLColorFormatRGBA8, 
										kEAGLDrawablePropertyColorFormat, 
										nil];
		
		
		_surface = [[hdGLES1Surface alloc] init];
		if(!_surface) 
		{
			_surface = [[hdGLES2Surface alloc] init];
			if (!_surface)
			{
				[self release];
				return nil;
			}
		}
		
		_surface.delegate = self;
		
#ifdef __IPHONE_4_0
		if([[UIScreen mainScreen] respondsToSelector:@selector(scale)] &&
		   [self respondsToSelector:@selector(contentScaleFactor)]) 
		{
			self.contentScaleFactor = MAX(1.0f, [[UIScreen mainScreen] scale]);
		}
#endif
		if (![_surface createSurface])
		{
			[self release];
			return nil;
		}

		
		_size = frame.size;
	}

	return self;
}


- (void) dealloc
{
	[_surface release];
	[super dealloc];
}


- (void) layoutSubviews
{
	CGRect bounds = [self bounds];
	
	if(_autoresize && ((roundf(bounds.size.width) != _size.width) || (roundf(bounds.size.height) != _size.height))) 
	{
		[_surface destroySurface];
		[_surface createSurface];
		_size = bounds.size;
	}
}


- (void) setAutoresizesEAGLSurface:(BOOL)autoresizesEAGLSurface;
{
	_autoresize = autoresizesEAGLSurface;
	if(_autoresize)
		[self layoutSubviews];
}


- (void) swapBuffers
{
	[_surface swapBuffers];
}


- (void)setCurrentContext
{
	[_surface setCurrentContext];
}


- (CAEAGLLayer *) getLayer
{
	return (CAEAGLLayer*)[self layer];
}


- (CGRect) getBounds
{
	return [self bounds];
}



@end
