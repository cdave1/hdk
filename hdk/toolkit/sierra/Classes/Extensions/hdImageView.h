//
//  hdImageView.h
//  Sierra
//
//  Created by David Petrie on 4/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface hdImageView : NSImageView {
@private
	NSString *_currentImagePath;
}

- (const char *)getResourcePathOfImage;

@property (nonatomic, retain) NSString *currentImagePath;

@end
