/*
 *  osxFileSystem.mm
 *  PakMaker
 *
 *  Created by david on 2/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdOSXFileSystem.h"


#define DEBUG 1



@implementation hdOSXFileSystem


static hdOSXFileSystem *singleton = nil;


void FileSystem_Init()
{	
	if (singleton == nil)
	{
		singleton = [[hdOSXFileSystem alloc] init];
	}
}


int FileSystem_CreateFile(const char* fileName)
{
	assert(singleton != nil);
	return [singleton CreateFile:[NSString stringWithCString:fileName encoding:NSASCIIStringEncoding]];
}


bool FileSystem_FileExists(const char* fileName)
{
	assert(singleton != nil);
	return [singleton FileExists:[NSString stringWithCString:fileName encoding:NSASCIIStringEncoding]];
}


int FileSystem_ReadFromFile(void** dest, int* len, const char* fileName)
{
	assert(singleton != nil);
	NSData *data = [singleton ReadFromFile:[NSString stringWithCString:fileName encoding:NSASCIIStringEncoding]];
	if (data == nil)
		return -1;
	
	if (((*dest) = malloc(sizeof(unsigned char) * [data length])) == NULL)
	{
		printf("Out of mem\n");
		return -1;
	}
	
	(*len) = [data length];
	[data getBytes:(*dest)];
	return 0;
}


int FileSystem_WriteToFile(const char* fileName, const void* bytes, const unsigned long len)
{
	assert(singleton != nil);
	
	return [singleton WriteToFile:[NSString stringWithCString:fileName encoding:NSASCIIStringEncoding]
						 withData:[NSData dataWithBytes:bytes length:len]];
}


void FileSystem_Teardown()
{
	assert(singleton != nil);
	[singleton release];
	singleton = nil;
}


- (NSInteger)CreateFile:(NSString *)fileName
{	
	NSFileManager *fileManager = [NSFileManager defaultManager];
	if ([fileManager fileExistsAtPath:fileName])
	{
#ifdef DEBUG
		printf("iPhone FileSystem error: cannot overwrite file %s:\n", 
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
#endif
		return -1;
	}
	
	if (NO == [fileManager createFileAtPath:fileName contents:[NSData data] attributes:[NSDictionary dictionary]])
	{
#ifdef DEBUG
		printf("iPhone FileSystem error: cannot create file (%s) at path:\n", 
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
#endif		
	}
	return 0;
}


- (BOOL)FileExists:(NSString *)fileName
{
	NSFileManager *fileManager = [NSFileManager defaultManager];
	return [fileManager isWritableFileAtPath:fileName];
}


- (NSInteger)WriteToFile:(NSString *)fileName withData:(NSData *)fileData
{
	NSFileManager *fileManager = [NSFileManager defaultManager];
	
	if (NO == [fileManager isWritableFileAtPath:fileName])
	{
#ifdef DEBUG
		printf("iPhone FileSystem error: cannot write to file %s because it does not exist\n", 
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
#endif
		return -1;
	}
	
	NSFileHandle *hnd = [NSFileHandle fileHandleForWritingAtPath:fileName];
	assert(hnd != nil);
	
	@try
	{ 
		[hnd writeData:fileData];
	} 
	@catch(NSException *exception)
	{
		/*
		 * Explicitly log the exception.
		 */
		printf("iPhone FileSystem error: cannot write DATA to file %s because of the following reason:\n", 
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
		printf("\tName: %s\n\tReason: %s\n",
			   [exception name],
			   [exception reason]);
		[hnd closeFile];
		return -1;
	}
	
	[hnd closeFile];
	return 0;
}


- (NSData *)ReadFromFile:(NSString *)fileName
{
	NSData *data = nil;
	NSFileManager *fileManager = [NSFileManager defaultManager];
	
	if (NO == [fileManager isReadableFileAtPath:fileName])
	{
#ifdef DEBUG
		printf("iPhone FileSystem error: cannot read from file %s because it does not exist\n", 
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
#endif
		return nil;
	}
	
	NSFileHandle *hnd = [NSFileHandle fileHandleForReadingAtPath:fileName];
	assert(hnd != nil);
	
	@try
	{ 
		data = [hnd availableData];
	} 
	@catch(NSException *exception)
	{
		/*
		 * Explicitly log the exception.
		 */
		printf("iPhone FileSystem error: cannot read DATA from file %s because of the following reason:\n", 
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
		printf("\tName: %s\n\tReason: %s\n",
			   [exception name],
			   [exception reason]);
	}
	
	[hnd closeFile];
	return data;
}

@end