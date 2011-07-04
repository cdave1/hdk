//
//  hdMultiTouchViewController.mm
//  hdk
//
//  Created by David Petrie on 28/03/11.
//  Copyright 2011 n/a. All rights reserved.
//

#import "hdMultiTouchViewController.h"


@implementation hdMultiTouchViewController

@synthesize touchView;


- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation 
{
    return (UIInterfaceOrientationIsLandscape(interfaceOrientation));
}


- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    assert(touchView);
    
	CGRect screenRect = [[UIScreen mainScreen] bounds];
	CGRect rect;
	
	if (toInterfaceOrientation == UIInterfaceOrientationPortrait || 
        toInterfaceOrientation == UIInterfaceOrientationPortraitUpsideDown)
    {
        rect = screenRect;
	}
	else if (toInterfaceOrientation == UIInterfaceOrientationLandscapeLeft || 
             toInterfaceOrientation == UIInterfaceOrientationLandscapeRight)
    {
        rect.size = CGSizeMake(screenRect.size.height, screenRect.size.width);
	}
    
    float scale = 1.0f;
	if([[UIScreen mainScreen] respondsToSelector:@selector(scale)]) 
	{
		scale = [[UIScreen mainScreen] scale];
	}
	
	if (scale != 1) 
    {
		rect.size.width *= scale;
		rect.size.height *= scale;
	}
    touchView.frame = rect;
}


- (void)didReceiveMemoryWarning 
{
    [super didReceiveMemoryWarning];
}


- (void)viewDidUnload 
{
    [super viewDidUnload];
}


- (void)dealloc 
{
    [touchView release];
    [super dealloc];
}

@end
