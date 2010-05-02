/*
 *  hdPNGLoader.cpp
 *  TotemGame
 *
 *  Created by david on 28/09/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdPNGLoader.h"

#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <UIKit/UIImage.h>


void LoadPNG( const char *filename, unsigned char **pic, unsigned short *width, unsigned short *height, unsigned short *bytes )
{
	
	filehandle_t *hFile;
	
	unsigned int fileDataLength;
	unsigned char *fileData;
	
	NSData *fileDataForUIImage;
	
	CGImageRef image;
	CGContextRef context;
	unsigned char *buffer;
	
	
	size_t w, h, channels;
	
	hdPrintf("[PNGLoader]: Attempting to open (%s) for reading\n", filename);
	
	*pic = NULL;
	
	hFile = FileSystem_OpenFile( filename, 0 );
	if(!hFile)
    {
		hdPrintf("[PNGLoader]: Could not open (%s) for reading. Couldn't find file\n", filename);
		goto PNGLOADFAILED;
    }
	
	// Load all the file info into a buffer, and then NSData*
	fileDataLength = FileSystem_GetFileSize(hFile);
	
	if (0 == fileDataLength)
	{
		hdPrintf("[PNGLoader]: Could not open (%s) for reading. File was empty.\n", filename);
		goto PNGLOADFAILED;		
	}
	
	if (NULL == (fileData = (unsigned char*)calloc(1, sizeof(unsigned char) * fileDataLength)))
	{
		hdPrintf("[PNGLoader]: MALLOC FAIL loading file %s\n", filename);
		FileSystem_CloseFile(hFile);
		goto PNGLOADFAILED;
	}
	
	if (0 == FileSystem_ReadFile(fileData, fileDataLength, 1, hFile))
	{
		hdPrintf("[PNGLoader]: Error reading file data in %s.\n", filename);
		free(fileData);
		FileSystem_CloseFile(hFile);
		goto PNGLOADFAILED;				
	}
	
	
	FileSystem_CloseFile(hFile);
	
	fileDataForUIImage = [NSData dataWithBytes:fileData length:fileDataLength];
	free(fileData);
	if (nil == fileDataForUIImage)
	{
		hdPrintf("[PNGLoader]: Error creating NSData with png data from %s.\n", filename);
		goto PNGLOADFAILED;			
	}
	
	// Creates a Core Graphics image from an image file
	// This function doesn't do any (slowwww on iPhone) caching.
	image = [UIImage imageWithData:fileDataForUIImage].CGImage;
	
	if (image == nil) 
	{
		hdPrintf("[PNGLoader]: could not load file at %s.\n", filename);
		goto PNGLOADFAILED;
	}
	
	w = CGImageGetWidth(image);
	h = CGImageGetHeight(image);
	channels = 4; // CGImageGetBytesPerRow(image) / w;
	
	// Needs to be a power of two
	if (false == ((w & -w) == w && (h & -h) == h))
	{
		hdPrintf("[PNGLoader]: could not load png. Width and/or height not a power of 2.\n", filename);
		goto PNGLOADFAILED;
	}
	
	if (NULL == (buffer = (unsigned char *) calloc(1, w * h * channels)))
	{
		hdPrintf("[PNGLoader]: MALLOC FAIL loading file %s\n", filename);
		goto PNGLOADFAILED;
	} 
	
	// Uses the bitmatp creation function provided by the Core Graphics framework. 
	
#if 1 
	 context = CGBitmapContextCreate(buffer, w, h, 
									 8, w * channels, 
									 CGImageGetColorSpace(image), 
									 kCGImageAlphaPremultipliedLast);
	//memset(buffer, 0, (w * h * channels));
	CGContextSetBlendMode(context, kCGBlendModeCopy);
	
#else	
	if (channels == 4)
	{
		// RGBA
		context = CGBitmapContextCreate(buffer, w, h, 
									8, CGImageGetBytesPerRow(image), 
									CGImageGetColorSpace(image), 
									kCGImageAlphaPremultipliedLast);
	 } 
	else
	{
		// HACK: Assume RGB. You'll get garbage if it's luminosity data.
		context = CGBitmapContextCreate(buffer, w, h, 
										8, CGImageGetBytesPerRow(image), 
										CGImageGetColorSpace(image), 
										kCGImageAlphaNone);		
	}
#endif
	
	// After you create the context, you can draw the sprite image to the context.
	CGContextDrawImage(context, CGRectMake(0.0, 0.0, (CGFloat)w, (CGFloat)h), image);
	
	// You don't need the context at this point, so you need to release it to avoid memory leaks.
	CGContextRelease(context);
	
	
	*pic = buffer;
	*width = w;
	*height = h;
	*bytes = channels;
	
	return;
	
	
PNGLOADFAILED:
	*pic = NULL;
	*width = 0;
	*height = 0;
	*bytes = 0;
}



#else

#include "png.h"

/*
 * PNGLib interface
 */

png_bytep *row_pointers;
int rowbytes;

void info_callback(png_structp png_ptr, png_infop info)
{
	unsigned i;
	png_color_8p sig_bit;
	
	// Big endian to Little endian (not sure if needed) 
	//if (info->bit_depth == 16)
	//{
	//	png_set_swap(png_ptr);
	//}
	
	//if (png_get_sBIT(png_ptr, info, &sig_bit))
	//	png_set_shift(png_ptr, sig_bit);
	
	png_read_update_info(png_ptr, info);
	rowbytes = info->rowbytes;
	
	// Allocate row pointer space.
	if (info->height > 0)
	{
		if (NULL == (row_pointers = (png_bytep *)calloc(1, sizeof(png_bytep) * info->height)))
		{
			hdPrintf("Malloc fail\n");
			return;
		}
		
		for (i = 0; i < info->height; ++i)
		{
			if (NULL == (row_pointers[i] = (png_byte *)calloc(1, sizeof(png_byte) * rowbytes)))
			{
				hdPrintf("Malloc fail\n");
				return;				
			}
			//memset(row_pointers[i], 0, sizeof(png_byte) * rowbytes);
		}
	}
	else
	{
		row_pointers = NULL;
	}
}


void row_callback(png_structp png_ptr, png_bytep new_row, png_uint_32 row_num, int pass)
{
	png_progressive_combine_row(png_ptr, row_pointers[row_num], new_row);
}


void end_callback(png_structp png_ptr, png_infop info)
{
}


void LoadPNG( const char *filename, unsigned char **pic, unsigned short *width, unsigned short *height, unsigned short *bytes )
{
	png_structp png_ptr;
	
	png_byte header[8];
	png_byte *raw;
	png_infop png_info_ptr;
	png_infop png_end_info_ptr;
	
	void *error_ptr;
	
	filehandle_t *pngFile;
	
	unsigned i;
	unsigned int w, h, channels;
	int len;
	unsigned char *buffer;
	
	hdPrintf("[PNGLoader]: Attempting to open (%s) for reading\n", filename);
	
	row_pointers = NULL;
	
	pngFile = NULL;
	*pic = NULL;
	
	if (filename[0] == '/')
	{
		filename += 1;
	}
	
	if (NULL == (pngFile = FileSystem_OpenFile(filename, 0)))
    {
		hdPrintf("[PNGLoader]: Could not open (%s) for reading. Couldn't find file\n", filename);
		goto PNGLOADFAILED;
    }
	
	// Check the png sig
	FileSystem_ReadFile(header, 8, 1, pngFile);
	FileSystem_FileSeek(pngFile, 0, SEEK_SET);
	if (0 != png_sig_cmp(header, 0, 8))
	{
		hdPrintf("[PNGLoader]: PNGLIB says file was not a png! %s\n", filename);
		goto PNGLOADFAILED;		
	}
	
	if (NULL == (png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
				 (png_voidp)error_ptr, NULL, NULL)))
	{
		hdPrintf("[PNGLoader]: PNGLIB could not create read struct. %s\n", filename);
		goto PNGLOADFAILED;	
	}
	
	if (NULL == (png_info_ptr = png_create_info_struct(png_ptr)))
	{
		hdPrintf("[PNGLoader]: PNGLIB could not create info struct. %s\n", filename);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		goto PNGLOADFAILED;	
	}
	
	if (NULL == (png_end_info_ptr = png_create_info_struct(png_ptr)))
	{
		hdPrintf("[PNGLoader]: PNGLIB could not create end info struct. %s\n", filename);
		png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
		goto PNGLOADFAILED;			
	}
	
	/*
	 * LOAD PNG FILE
	 */	
	raw = NULL;
	len = FileSystem_GetFileSize(pngFile);
	if (NULL == (raw = (png_byte *)calloc(1, sizeof(png_byte) * len)))
	{
		png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
		goto PNGLOADFAILED;	
	}
	
	FileSystem_ReadFile(raw, len, 1, pngFile);
	
	png_set_progressive_read_fn(png_ptr, NULL,
								info_callback, row_callback, end_callback);
	
	png_process_data(png_ptr, png_info_ptr, (png_bytep)raw, len);
	
	/*
	 * READ IMAGE DATA
	 */
	h = png_info_ptr->height;
	w = png_info_ptr->width;
	channels = png_info_ptr->channels;
	
	if (NULL == (buffer = (unsigned char *)calloc(1, h * png_info_ptr->rowbytes)))
	{
		hdPrintf("[PNGLoader]: MALLOC FAIL loading file %s\n", filename);
		goto PNGLOADFAILED;
	} 
	
	hdAssert(row_pointers != NULL);
	
	// Copy and clean the row pointers to local buffer.
	for (i = 0; i < h; ++i)
	{
		memcpy(buffer + (i * png_info_ptr->rowbytes), row_pointers[i], png_info_ptr->rowbytes);
		free(row_pointers[i]);
	}
	free(row_pointers);
	free(raw);
	row_pointers = NULL;
	
	/*
	 * CLEAN UP
	 */
	png_destroy_read_struct(&png_ptr, &png_info_ptr, NULL);
	
	FileSystem_CloseFile(pngFile);
	
	*pic = buffer;
	*width = w;
	*height = h;
	*bytes = channels;
	
	return;	

	
PNGLOADFAILED:
	if (raw != NULL)
		free(raw);
	
	if (pngFile != NULL)
		FileSystem_CloseFile(pngFile);
	
	*pic = NULL;
	*width = 0;
	*height = 0;
	*bytes = 0;
}
#endif
