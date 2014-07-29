/*
 * Copyright (c) 2014 Hackdirt Ltd.
 * Author: David Petrie (david@davidpetrie.com)
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the
 * use of this software. Permission is granted to anyone to use this software for
 * any purpose, including commercial applications, and to alter it and
 * redistribute it freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not claim
 * that you wrote the original software. If you use this software in a product, an
 * acknowledgment in the product documentation would be appreciated but is not
 * required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 * misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#import "hdMultiTouchViewController.h"

@interface hdMultiTouchViewController () {}

@property (strong, nonatomic) EAGLContext *context;
@property (strong, nonatomic) GLKBaseEffect *effect;

- (void)setupGL;
- (void)tearDownGL;
@end


@implementation hdMultiTouchViewController

@synthesize touchView;

- (void)dealloc {
    [_context release];
    [_effect release];
    [super dealloc];
}


- (void)viewDidLoad {
    [super viewDidLoad];

    self.context = [[[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1] autorelease];

    if (!self.context) {
        NSLog(@"Failed to create ES context");
    }

    GLKView *view = (GLKView *)self.view;
    view.context = self.context;
    view.drawableDepthFormat = GLKViewDrawableDepthFormat24;
    [view setMultipleTouchEnabled:YES];

    self.preferredFramesPerSecond = 60;
    [self setupGL];
}


- (void)viewDidUnload {
    [super viewDidUnload];

    [self tearDownGL];

    if ([EAGLContext currentContext] == self.context) {
        [EAGLContext setCurrentContext:nil];
    }
    self.context = nil;
}


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft ||
            interfaceOrientation == UIInterfaceOrientationLandscapeRight);
}


- (void)setupGL {
    [EAGLContext setCurrentContext:self.context];
}


- (void)tearDownGL {
    [EAGLContext setCurrentContext:self.context];
}


#pragma mark - GLKView and GLKViewController delegate methods

- (void)update {
    if (self.delegate) {
        [self.delegate performSelector:@selector(Update)];
    }
/*
    ++frames;
	CurrentTime = CACurrentMediaTime();

	if ((CurrentTime - LastFPSUpdate) > 1.0f) {
        printf("fps: %d\n", frames);
		frames = 0;
		LastFPSUpdate = CurrentTime;
	}*/
}


- (void)glkView:(GLKView *)view drawInRect:(CGRect)rect {
    if (self.delegate) {
        [self.delegate performSelector:@selector(Render)];
    }
}

@end
