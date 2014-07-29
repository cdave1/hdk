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

#import "BlockPropertyWindow.h"
#import "LevelEditor.h"
#import "NSImageView+Extensions.h"
#import "NSWindowController+Extensions.h"

@implementation BlockPropertyWindow

- (id) init
{
    if ((self = [super initWithWindowNibName:@"BlockPropertyWindow"]))
    {
        [[NSNotificationCenter defaultCenter]
         addObserver:self
         selector:@selector(itemWasSelected)
         name:kSingleItemSelectedNotification
         object:nil];

        [[NSNotificationCenter defaultCenter]
         addObserver:self
         selector:@selector(itemWasUnselected)
         name:kSingleItemUnselectedNotification
         object:nil];



        [[NSNotificationCenter defaultCenter]
         addObserver:self
         selector:@selector(multipleItemsSelected)
         name:kMultipleItemsSelectedNotification
         object:nil];

        // initially hidden
        [self.window orderOut:self];

        if ([self.window respondsToSelector:@selector(setFloatingPanel:)])
        {
            [((NSPanel *)self.window) setFloatingPanel:YES];
        }

        [_texture registerForDraggedTypes:[NSArray arrayWithObjects:NSCreateFileContentsPboardType(@"tga"),NSCreateFileContentsPboardType(@"png"),nil]];
    }
    return self;
}


- (void)itemWasSelected
{
    const hdGameObject *selected;
    if (!(selected = [LevelEditor sharedInstance]->GetSelectedGameObjectSingle()))
    {
        hdPrintf("No game object was selected.");
        return;
    }

    if (selected->GetUserType() != e_totemTypeBlock)
    {
        [self.window orderOut:self];
        return;
    }
    [self.window orderBack:self];
    [self setAllEnabled:YES];
    if (((totemBlock*)selected)->IsTextureChangeable())
    {
        [_texture setEnabled:YES];
        [_texture setImageAtContentRepositoryPath:((totemBlock*)selected)->GetTextureName()];
        [_textureName setStringValue:[NSString stringWithUTF8String:((totemBlock*)selected)->GetTextureName()]];
    }
    else
    {
        [_texture setEnabled:NO];
    }

    if (((totemBlock *)selected)->GetTint())
    {
        NSColor *color = [NSColor colorWithDeviceRed:((totemBlock *)selected)->GetTint()[0]
                                               green:((totemBlock *)selected)->GetTint()[1]
                                                blue:((totemBlock *)selected)->GetTint()[2]
                                               alpha:((totemBlock *)selected)->GetTint()[3]];
        [_tint setColor:color];
    }
    else
    {
        [_tint setColor:[NSColor whiteColor]];
    }

    /*
     The following is a bit pedantic, given the structure of the block type and material
     enums and menu enums, but I'm paranoid about one of the lists being out of whack and having to
     spend hours tracking down the bug.
     */
    NSUInteger materialComboBoxIndex = 0, totemBlockTypeComboBoxIndex = 0;
    for (int i = 0; i < hdMin((NSInteger)e_totemMaterial_Count, [_materialComboBox numberOfItems]); i++)
    {
        if (totemMaterialMenuItems[i].material == ((totemBlock *)selected)->GetMaterial())
        {
            materialComboBoxIndex = i;
            break;
        }
    }

    for (int i = 0; i < hdMin((NSInteger)e_totemBlockType_Count, [_blockTypeComboBox numberOfItems]); i++)
    {
        if (totemBlockTypeMenuItems[i].blockType == ((totemBlock *)selected)->GetBlockType())
        {
            totemBlockTypeComboBoxIndex = i;
            break;
        }
    }

    [_materialComboBox selectItemAtIndex:materialComboBoxIndex];
    [_blockTypeComboBox selectItemAtIndex:totemBlockTypeComboBoxIndex];

    [_blockTag setIntValue:((totemBlock *)selected)->GetTag()];
    [_depth setFloatValue:-((totemBlock *)selected)->GetDepth()]; /* keep the minus! */
    [_zOffset setFloatValue:((totemBlock *)selected)->GetZOffset()];
    [_tiling setFloatValue:((totemBlock *)selected)->GetTextureRepeatX()];

    [self updateTextInterface];
}


- (void)multipleItemsSelected
{
    if (![LevelEditor sharedInstance]->SelectedItemsContainsType(e_totemTypeBlock)) return;

    [self.window orderBack:self];
    [self setAllEnabled:NO];

    [_texture setEnabled:YES];
    [_textureName setEnabled:YES];
    [_tint setEnabled:YES];
    [_materialComboBox setEnabled:YES];
    [_blockTypeComboBox setEnabled:YES];
    [_depth setEnabled:YES];
    [_zOffset setEnabled:YES];
    [_blockTag setEnabled:YES];
    [self updateTextInterface];
}


- (void)itemWasUnselected
{
    [self.window orderOut:self];
}


- (IBAction)valueWasChanged:(id)sender
{
}


- (NSDragOperation)draggingEntered:(id < NSDraggingInfo >)sender
{
    return NSDragOperationGeneric;
}


- (BOOL)performDragOperation:(id <NSDraggingInfo>)sender
{
    return YES;
}


- (IBAction)UpdateSelectedBlockMaterial:(id)sender
{
    totemBlock* selected = NULL;
    for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
    {
        if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_totemTypeBlock)
        {
            selected = (totemBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
            selected->SetMaterial(totemMaterialMenuItems[[_materialComboBox indexOfSelectedItem]].material);
        }
    }
}


- (IBAction)UpdateSelectedBlockType:(id)sender
{
    totemBlock* selected = NULL;
    for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
    {
        if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_totemTypeBlock)
        {
            selected = (totemBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
            selected->SetBlockType(totemBlockTypeMenuItems[[_blockTypeComboBox indexOfSelectedItem]].blockType);

            selected->ResetAABB();
            selected->ResetOBB();
        }
    }
}


- (IBAction)UpdateSelectedBlockZOffset:(id)sender
{
    float zOffset;
    totemBlock* selected = NULL;
    for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
    {
        if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_totemTypeBlock)
        {
            selected = (totemBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
            zOffset = [_zOffset floatValue];
            zOffset = hdClamp(zOffset, -10.0f, 10.0f);
            selected->SetZOffset(zOffset);
        }
    }
    [self updateTextInterface];
}


- (IBAction)UpdateSlidersFromText:(id)sender
{
    [_zOffset setFloatValue:[_zOffsetText floatValue]];
    [self UpdateSelectedBlockZOffset:nil];
    [_tiling setFloatValue:[_tilingText floatValue]];
    [self UpdateSelectedBlockTiling:nil];
    [_depth setFloatValue:[_depthText floatValue]];
    [self UpdateSelectedBlockDepth:nil];
}


/**
 * Fired by drag and drop event onto imageview
 */
- (IBAction)UpdateSelectedBlockTexture:(id)sender
{
    totemBlock* selected = NULL;
    const char *textureName = [_texture getResourcePathOfImage];
    if (!textureName) return;

    for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
    {
        if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_totemTypeBlock)
        {
            selected = (totemBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
            if (selected && (selected->IsTextureChangeable()))
            {
                selected->SetTextureName(textureName);
                selected->ResetTextureCoords();
            }
        }
    }
    [_textureName setStringValue:[NSString stringWithUTF8String:textureName]];
}


- (IBAction)UpdateSelectedBlockTiling:(id)sender
{
    hdVec3 dim;
    float hwRatio, xTiling;
    totemBlock* selected = NULL;

    for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
    {
        if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_totemTypeBlock)
        {
            selected = (totemBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
            dim = (selected->GetAABB().upper - selected->GetAABB().lower);

            if (dim.x <= 0.0f) return;

            hwRatio = (dim.y / dim.x);
            xTiling		= [_tiling floatValue];

            if (xTiling < 0.01f) xTiling = 1.0f;

            selected->SetTextureRepeatX(xTiling);
            selected->SetTextureRepeatY(xTiling);

            selected->ResetTextureCoords();
        }
    }
    [self updateTextInterface];
}


- (IBAction)UpdateSelectedBlockDepth:(id)sender
{
    totemBlock* selected = NULL;
    for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
    {
        if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_totemTypeBlock)
        {
            selected = (totemBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
            if ([_depth floatValue] == 0)
            {
                selected->SetDepth(-0.02f);
            }
            else
            {
                selected->SetDepth(-[_depth floatValue]);
            }
        }
    }
    [self updateTextInterface];
}


- (IBAction)UpdateSelectedBlockTag:(id)sender
{
    totemBlock* selected = NULL;
    for (int i = 0; i < [LevelEditor sharedInstance]->GetSelectedGameObjectsCount(); ++i)
    {
        if ([LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i)->GetUserType() == e_totemTypeBlock)
        {
            selected = (totemBlock *)[LevelEditor sharedInstance]->GetSelectedGameObjectAtIndex(i);
            selected->SetTag([_blockTag intValue]);
        }
    }
}


- (void)updateTextInterface
{
    [_zOffsetText setFloatValue:[_zOffset floatValue]];
    [_tilingText setFloatValue:[_tiling floatValue]];
    [_depthText setFloatValue:[_depth floatValue]];
}


#pragma mark -
#pragma mark NSComboBoxDelegate functions

- (void)comboBoxSelectionDidChange:(NSNotification *)notification
{
    NSLog(@"%@", [notification name]);
}


- (void)comboBoxWillDismiss:(NSNotification *)notification
{
    NSLog(@"%@", [notification name]);
}


#pragma mark -
#pragma mark NSComboBoxDataSource functions

- (id)comboBox:(NSComboBox *)aComboBox objectValueForItemAtIndex:(NSInteger)index
{
    if (aComboBox == _materialComboBox)
    {
        return [NSString stringWithUTF8String:totemMaterialMenuItems[index].materialName];
    }
    else if (aComboBox == _blockTypeComboBox)
    {
        return [NSString stringWithUTF8String:totemBlockTypeMenuItems[index].blockTypeName];
    }
    return nil;
}


- (NSInteger)numberOfItemsInComboBox:(NSComboBox *)aComboBox
{
    if (aComboBox == _materialComboBox)
    {
        return e_totemMaterial_Count;
    }
    else if (aComboBox == _blockTypeComboBox)
    {
        return e_totemBlockType_Count;
    }
    return 0;
}

@end
