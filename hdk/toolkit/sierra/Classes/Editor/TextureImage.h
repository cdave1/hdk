//
//  TextureImage.h
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <Quartz/Quartz.h>

@interface TextureImage : NSObject
{
@private
    NSString *_path;
}

@property (nonatomic, copy) NSString *path;

@end
