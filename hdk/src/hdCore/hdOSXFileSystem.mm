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
char gamedir[ 512 ];

void FileSystem_Init(const char* baseDir)
{	
	if (singleton == nil)
	{
		singleton = [[hdOSXFileSystem alloc] init];
		//snprintf(gamedir, 512, "%s/", baseDir);
		snprintf(gamedir, 512, "%s/BaseDir/", baseDir);
#ifndef LEVEL_EDITOR
		hdPakFileSystem::InitPakFileSystem(gamedir);
#endif
		//hdPakFileSystem::InitPakFileSystem(gamedir);
	}
}




char *FileSystem_BaseDir( void )
{
	assert(strlen(gamedir) > 0);
	return gamedir;
}



/*
 -----------------------------------------------------------------------------
 Function: hdGetLoadedFilePointer() -Get file pointer.
 
 Parameters: 
 filestream -[in] Target file handle.
 origin -[in] Pointer position
 SEEK_SET -Beginning of file.
 SEEK_CUR -Current position of file pointer.
 SEEK_END -End of file.
 
 Returns: File pointer on success, otherwise NULL.
 
 Notes: 
 -----------------------------------------------------------------------------
 */
void *FileSystem_GetLoadedFilePointer( filehandle_t *fhandle, signed int origin )
{
	switch( origin )
	{
		case SEEK_SET:
			return( (void *)fhandle->ptrStart );
			
		case SEEK_END:
			return( (void *)fhandle->ptrEnd );
			
		case SEEK_CUR:
			return( (void *)fhandle->ptrCurrent );
	}
	
	return NULL;
}

/*
 -----------------------------------------------------------------------------
 Function: hdGetFileSize() -Get the length of a file.
 
 Parameters: filestream -[in] Target file handle.
 
 Returns: The file length in bytes.
 
 Notes: 
 -----------------------------------------------------------------------------
 */
unsigned int FileSystem_GetFileSize( filehandle_t *fhandle )
{
	return fhandle->filesize;
}

/*
 -----------------------------------------------------------------------------
 Function: hdFileSeek() -Moves the file pointer to a specified location.
 
 Parameters: 
 fhandle -[in] Pointer to filehandle_t structure.
 offset -[in] Number of bytes from origin
 origin -[in] Initial position
 SEEK_SET -Beginning of file.
 SEEK_CUR -Current position of file pointer.
 SEEK_END -End of file.
 
 Returns: If successful zero, otherwise a nonzero value.
 
 Notes: 
 -----------------------------------------------------------------------------
 */
unsigned int FileSystem_FileSeek( filehandle_t *fhandle, unsigned int offset, unsigned int origin )
{
	switch( origin )
	{
		case SEEK_SET:
			if( offset < 0 ||
			   offset > fhandle->filesize )
			{
				return 1;
			}
			
			fhandle->ptrCurrent = fhandle->ptrStart + offset;
			break;
			
		case SEEK_END:
			if( offset > 0 )
			{
				return 1;
			}
			
			// offset is negative 
			if( (fhandle->filesize + offset) < 0  )
			{
				return 1;
			}
			
			// offset is negative 
			fhandle->ptrCurrent = fhandle->ptrEnd + offset;
			break;
			
		case SEEK_CUR:
			if( offset < 0 )
			{
				// offset is negative
				if( ((fhandle->ptrCurrent - fhandle->ptrStart) + offset) < 0 )
				{
					return 1;
				}
			}
			
			if( offset > 0 )
			{
				if( offset > (fhandle->ptrEnd - fhandle->ptrCurrent) )
				{
					return 1;
				}
			}
			
			fhandle->ptrCurrent += offset;
			break;
			
		default:
			return 1;
	}
	
	return 0;
}


/*
 -----------------------------------------------------------------------------
 Function: hdCloseFile -Close file handle.
 
 Parameters: filestream -[in] Pointer to valid FILE structure.		 
 
 Returns: Nothing.
 
 Notes: Closes a file stream that was returned by hdFOpenFile.
 
 -----------------------------------------------------------------------------
 */
void FileSystem_CloseFile( filehandle_t *fhandle )
{
	if( fhandle->filedata)
	{
#ifdef USE_MMAP
		if (fhandle->mmapped)
		{
			if ( munmap( fhandle->filedata, fhandle->filesize ) != 0 ) {
				assert( 0 );
			}
		}
		else
		{
			free( fhandle->filedata );
		}
#else
		free( fhandle->filedata );
#endif
		fhandle->filedata = NULL;
	}
	
	free( fhandle );
}


/*
 -----------------------------------------------------------------------------
 Function: hdOpenFile -Open file from the file system.
 
 By default, look in pakFileSystem first!
 -----------------------------------------------------------------------------
 */
filehandle_t *FileSystem_OpenFile( const char *filename, unsigned int FlagsAndAttributes )
{
	char			netpath[ 256 ];
	filehandle_t	*hFile;
	const char		*pathBase;
	struct stat s;
	int		fd;

#ifndef LEVEL_EDITOR
	if (FlagsAndAttributes == 0)
	{
		hdPrintf("Searching pak files for %s\n", filename);
		if (NULL != (hFile = hdPakFileSystem::OpenFile(filename)))
		{
			return hFile;
		}
	}
#endif
	
	if ((FlagsAndAttributes & HD_FS_ABSOLUTE_PATH) == HD_FS_ABSOLUTE_PATH)
	{
		// Just use full filename
		snprintf( netpath, sizeof( netpath ), filename);
	}
	else
	{
		pathBase = FileSystem_BaseDir();
		snprintf( netpath, sizeof( netpath ), "%s%s", pathBase, filename );
	}
	
	// high performance file mapping path, avoiding stdio
	fd = open( netpath, O_RDONLY );
	if ( fd == -1 ) {
		return NULL;
	}
	fstat( fd, &s );
	
	hFile = (filehandle_t*)malloc( sizeof( filehandle_t ) );
	memset( hFile, 0, sizeof( filehandle_t ) );
	
	hFile->filesize = s.st_size;
#ifdef USE_MMAP	
	hFile->filedata = mmap( NULL, hFile->filesize, PROT_READ, MAP_FILE|MAP_PRIVATE, fd, 0 );
	if ( (int)hFile->filedata == -1 ) {
		printf( "mmap failed: %s\n", strerror( errno ) );
		assert( 0 );
	}
	hFile->mmapped = true;
#else
	hFile->filedata = malloc( hFile->filesize );
	read( fd, hFile->filedata, hFile->filesize );
	hFile->mmapped = false;
#endif
	hFile->ptrStart =  hFile->ptrCurrent = (unsigned char *)hFile->filedata;
	hFile->ptrEnd =  (unsigned char *)hFile->filedata + hFile->filesize;
	hFile->bLoaded = true;
	// mmap doesn't require the file to stay open
	close( fd );
	return hFile;
}


/*
 -----------------------------------------------------------------------------
 Function: hdReadFile -Reads data from a stream.
 
 Parameters: buffer -[in/out] Storage location for data.
 size -[in] Item size in bytes.
 count -[in] Maximum number of items to be read.
 fhandle -[in] Pointer to valid filehandle_t structure.
 
 Returns: On success number of full items actually read, otherwise -1.
 
 Notes:
 
 -----------------------------------------------------------------------------
 */
unsigned int FileSystem_ReadFile( void *buffer, unsigned int size, unsigned int count, filehandle_t *fhandle )
{		
	unsigned char	*buf = (unsigned char *)buffer;
	signed int i;
	
	if( (size * count) > (fhandle->ptrEnd - fhandle->ptrCurrent) )
	{
		signed int read;
		
		read = (fhandle->ptrEnd - fhandle->ptrCurrent);
		
		for( i = 0 ; i < (fhandle->ptrEnd - fhandle->ptrCurrent) ; ++i )
		{
			buf[ i ] = fhandle->ptrCurrent[ i ];
		}
		
		fhandle->ptrCurrent = fhandle->ptrEnd;
		
		return( read );
	}
	else
	{
		for( i = 0 ; i < (size * count) ; ++i, fhandle->ptrCurrent++ )
		{
			buf[ i ] = *fhandle->ptrCurrent;
		}
		
		return( (size * count) / size );
	}
	
	/* should never get here */
	return -1;
}


void FileSystem_Teardown()
{
	assert(singleton != nil);
	[singleton release];
	singleton = nil;
}



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
	{	
		printf("Data was nil\n");
		return -1;
	}
		
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


- (NSInteger)CreateFile:(NSString *)fileName
{	
	NSFileManager *fileManager = [NSFileManager defaultManager];
	if ([fileManager fileExistsAtPath:fileName])
	{
#ifdef DEBUG
		printf("OSX FileSystem error: cannot overwrite file %s:\n", 
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
#endif
		return -1;
	}
	
	if (NO == [fileManager createFileAtPath:fileName contents:[NSData data] attributes:[NSDictionary dictionary]])
	{
#ifdef DEBUG
		printf("OSX FileSystem error: cannot create file (%s) at path:\n", 
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
		printf("OSX FileSystem error: cannot write to file %s because it does not exist\n", 
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
		printf("OSX FileSystem error: cannot write DATA to file %s because of the following reason:\n", 
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
	BOOL isDir;
	NSFileManager *fileManager = [NSFileManager defaultManager];
	
	if (NO == [fileManager isReadableFileAtPath:fileName])
	{
#ifdef DEBUG
		printf("OSX FileSystem error: cannot read from file %s because it does not exist\n", 
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
#endif
		return nil;
	}
	
	[fileManager fileExistsAtPath:fileName isDirectory:&isDir];
	if (YES == isDir)
	{
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
		printf("OSX FileSystem error: cannot read DATA from file %s because of the following reason:\n", 
			   [fileName cStringUsingEncoding:NSASCIIStringEncoding]);
		printf("\tName: %s\n\tReason: %s\n",
			   [exception name],
			   [exception reason]);
	}
	
	[hnd closeFile];
	
	if ([data length] == 0)
	{
		return nil;
	}
	
	return data;
}

@end