/*
 *  hdFileSystemSupport.h
 *  TotemGame
 *
 *  Created by david on 3/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_FILE_SYSTEM_SUPPORT
#define HD_FILE_SYSTEM_SUPPORT

typedef struct
	{
	//	FILE *hFile;
		
		/* Following is used when the file is loaded into memory */
		bool bLoaded;			/* Was file loaded into memory? */
		bool mmapped; // Did we use mmap?
		
		unsigned int	filesize;				/* Size of file data in bytes */
		
		unsigned char	*ptrStart;				/* pointer to start of file data block */
		unsigned char	*ptrCurrent;			/* pointer to current position in file data block */
		unsigned char	*ptrEnd;				/* pointer to end of file data block */
		
		void *filedata;				/* file data loaded into memory */
		
	} filehandle_t;


#endif