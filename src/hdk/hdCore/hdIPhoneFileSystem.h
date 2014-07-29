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

#ifndef _HD_IPHONEFILESYSTEM_H_
#define _HD_IPHONEFILESYSTEM_H_

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