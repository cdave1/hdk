//
//  TextureImage.mm
//  Sierra
//
//  Created by David Petrie on 1/05/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import "TextureImage.h"


@implementation TextureImage
@synthesize path = _path;


- (NSString *) imageRepresentationType
{
    return IKImageBrowserPathRepresentationType;
}


- (id) imageRepresentation
{
    return _path;
}


- (NSString *) imageUID
{
    return _path;
}

@end
