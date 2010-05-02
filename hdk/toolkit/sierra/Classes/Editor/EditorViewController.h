//
//  EditorViewController.h
//  Sierra
//
//  Created by David Petrie on 30/04/10.
//  Copyright 2010 n/a. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "EditorView.h"

@interface EditorViewController : NSViewController 
{
@private
	EditorView *_editorView;
	
}

@property (nonatomic, retain) EditorView *editorView;

- (void) handleResize;

@end
