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

#import "TexturePaletteWindow.h"
#import "TextureImage.h"

@implementation TexturePaletteWindow

- (id) init
{
    if ((self = [super initWithWindowNibName:@"TexturePaletteWindow"]))
    {
        mImages = [[NSMutableArray alloc] init];

        // Get path for textures folder
        NSString *basePath = [NSString stringWithFormat:@"%@Textures/", [NSString stringWithUTF8String:FileSystem_BaseDir()]];
        NSFileManager *localFileManager = [[NSFileManager alloc] init];
        NSDirectoryEnumerator *dirEnum = [localFileManager enumeratorAtPath:basePath];

        NSString *file;

        while (file = [dirEnum nextObject])
        {
            TextureImage *image = [[TextureImage alloc] init];
            image.path = [NSString stringWithFormat:@"%@%@", basePath, file];
            image.title = file;

            hdPrintf("Loading texture: %s\n", [image.path UTF8String]);
            [mImages addObject:image];
        }
        [localFileManager release];
    }
    return self;
}


- (void) awakeFromNib
{
    [scrollView setDocumentView:mImageBrowser];
    [mImageBrowser reloadData];
    [mImageBrowser setAnimates:YES];
}


- (void) dealloc
{
    [mImages release];
    [super dealloc];
}


- (void) updateDatasource
{
    [mImageBrowser reloadData];
}


- (NSUInteger) numberOfItemsInImageBrowser:(IKImageBrowserView *) view
{
    hdPrintf("Images: %d\n", [mImages count]);
    return [mImages count];
}


- (id) imageBrowser:(IKImageBrowserView *) view itemAtIndex:(NSUInteger) index
{
    return [mImages objectAtIndex:index];
}

@end
