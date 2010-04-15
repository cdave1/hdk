//
//  hdIPhoneFSWrapper.h
//  TotemGame
//
//  Created by david on 30/07/09.
//  Copyright 2009 n/a. All rights reserved.
//

#ifndef _HD_IPHONEFILESYSTEM_H_
#define _HD_IPHONEFILESYSTEM_H_

#define USE_MMAP

#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <fcntl.h>


#import <Foundation/Foundation.h>


#include "hdFileSystem.h"
#include "hdPakFileSystem.h"

@interface hdIPhoneFileSystem : NSObject 
{

}


/*
 * Creates an editable file in docs folder.
 *
 * Returns 0 on success, negative number on fail. Will return
 * -1 if an file with the given name already exists.
 */
- (NSInteger)CreateFile:(NSString *)fileName;


/*
 * File existance in document folder. Returns false
 * if the file does not exist; prints out NSError contents
 * if NSError.
 */
- (BOOL)FileExists:(NSString *)fileName;


/*
 * Writes data to a file in the docs folder.
 *
 * If the file does not exist, an error will be returned.
 *
 * Returns 0 on success.
 */
- (NSInteger)WriteToFile:(NSString *)fileName withData:(NSData *)fileData;


/*
 * Read all data from file into NSData object.
 */
- (NSData *)ReadFromFile:(NSString *)fileName;

@end




#endif