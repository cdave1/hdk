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

#import <Cocoa/Cocoa.h>
#include "hdInput.h"

@interface EditorView : NSOpenGLView
{
    int colorBits, depthBits;
    BOOL runningFullScreen;
    NSDictionary *originalDisplayMode;
@private
    mousevalues_t mouseValues;
    mousevalues_t cpyMouseValues;
    BOOL keepRenderThreadAlive;
    NSLock *_lock;
    NSOpenGLContext *_localContext;
}

- (id) initWithFrame:(NSRect)frame colorBits:(int)numColorBits
           depthBits:(int)numDepthBits fullscreen:(BOOL)runFullScreen;
- (void) reshape;
- (BOOL) isFullScreen;
- (mousevalues_t)GetMouseValues;
- (void)startRenderLoop;
- (void) handleResize;
@end