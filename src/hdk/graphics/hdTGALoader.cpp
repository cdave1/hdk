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

/*
 * Acknowledgement:
 *
 * Most of this code derived from Michael Liebscher's port of Wolfenstein 3D, which
 * was in turn derived from GIMP. <johnnycanuck@users.sourceforge.net>
 */

#include "hdTGALoader.h"

#include <hdk/core/hdFileSystem.h>

#define TGA_HEADER_SIZE		18

unsigned char *p_buf;


/* TRUEVISION-XFILE magic signature string */
static unsigned char magic[ 18 ] =
{
    0x54, 0x52, 0x55, 0x45, 0x56, 0x49, 0x53, 0x49, 0x4f,
    0x4e, 0x2d, 0x58, 0x46, 0x49, 0x4c, 0x45, 0x2e, 0x0
};


typedef struct _TargaHeader
{
    unsigned char 	idLength;
    unsigned char	colorMapType;

    unsigned char	imageType;
    /* Known image types. */
#define TGA_TYPE_MAPPED      1
#define TGA_TYPE_COLOR       2
#define TGA_TYPE_GRAY        3

    unsigned char imageCompression;
    /* Only known compression is RLE */
#define TGA_COMP_NONE        0
#define TGA_COMP_RLE         1

    /* Color Map Specification. */
    unsigned short	colorMapIndex;
    unsigned short colorMapLength;
    unsigned char	colorMapSize;

    /* Image Specification. */
    unsigned short	xOrigin;
    unsigned short yOrigin;

    unsigned short width;
    unsigned short height;

    unsigned char bpp;
    unsigned char bytes;

    unsigned char alphaBits;
    unsigned char flipHoriz;
    unsigned char flipVert;

} TargaHeader;


void flip_line( unsigned char *buffer, TargaHeader *info )
{
    unsigned char  temp;
    unsigned char *alt;
    unsigned int    x, s;

    alt = buffer + (info->bytes * (info->width - 1));

    for( x = 0; x * 2 <= info->width; ++x )
    {
        for( s = 0; s < info->bytes; ++s )
        {
            temp = buffer[ s ];
            buffer[ s ] = alt[ s ];
            alt[ s ] = temp;
        }

        buffer += info->bytes;
        alt -= info->bytes;
    }
}


void upsample( unsigned char *dest, unsigned char *src,
              unsigned int width, unsigned int bytes, unsigned char alphaBits )
{
    unsigned int x;

    for( x = 0 ; x < width ; ++x )
    {
        dest[0] =  ((src[1] << 1) & 0xf8);
        dest[0] += (dest[0] >> 5);

        dest[1] =  ((src[0] & 0xe0) >> 2) + ((src[1] & 0x03) << 6);
        dest[1] += (dest[1] >> 5);

        dest[2] =  ((src[0] << 3) & 0xf8);
        dest[2] += (dest[2] >> 5);

        switch( alphaBits )
        {
            case 1:
                dest[ 3 ] = (src[ 1 ] & 0x80) ? 0 : 255;
                dest += 4;
                break;

            default:
                dest += 3;
        }

        src += bytes;
    }
}


void bgr2rgb( unsigned char *dest, unsigned char *src,
             unsigned int width, unsigned int bytes, unsigned int alpha )
{
    unsigned int x;

    if (bytes == 2)
    {
        for( x = 0 ; x < width ; ++x )
        {
            /* stored in bgra format across 2 bytes - convert to rgba across two bytes */
            /* TODO: did not know tga 16bpp was in 5551 format when I wrote hte code below */
            /* fix this later */
            *(dest++) = src[0]; //(src[0] & 0xF0) | (src[1] & 0x0F);
            *(dest++) = src[1]; //(src[1] & 0xF0) | (src[0] & 0x0F);
            src += bytes;
        }
    }
    else
    {

        if( alpha )
        {
            for( x = 0 ; x < width ; ++x )
            {
                *(dest++) = src[2];
                *(dest++) = src[1];
                *(dest++) = src[0];

                *(dest++) = src[3];

                src += bytes;
            }
        }
        else
        {
            for( x = 0 ; x < width ; ++x )
            {
                *(dest++) = src[2];
                *(dest++) = src[1];
                *(dest++) = src[0];

                src += bytes;
            }
        }
    }
}


unsigned int rle_read(	filehandle_t *fp, unsigned char *buffer,
                      TargaHeader *info )
{
    static unsigned int   repeat = 0;
    static unsigned int   direct = 0;
    static unsigned char sample[ 4 ];
    unsigned int head;
    unsigned char	temphead;
    unsigned int x, k;

    for( x = 0; x < info->width; ++x )
    {
        if( repeat == 0 && direct == 0 )
        {
            FileSystem_ReadFile( &temphead, 1, 1, fp );
            head = temphead;

            if( head >= 128 )
            {
                repeat = head - 127;

                if( FileSystem_ReadFile( sample, info->bytes, 1, fp ) < 1 )
                {
                    return EOF;
                }
            }
            else
            {
                direct = head + 1;
            }
        }

        if( repeat > 0 )
        {
            for( k = 0 ; k < info->bytes ; ++k )
            {
                buffer[ k ] = sample[ k ];
            }

            repeat--;
        }
        else /* direct > 0 */
        {
            if( FileSystem_ReadFile( buffer, info->bytes, 1, fp ) < 1 )
            {
                return EOF;
            }

            direct--;
        }

        buffer += info->bytes;
    }

    return 0;
}


void read_line( filehandle_t	*fp,
               unsigned char		*row,
               unsigned char		*buffer,
               TargaHeader     *info )
{
    if( info->imageCompression == TGA_COMP_RLE )
    {
        if( rle_read( fp, buffer, info ) == EOF )
        {
            return;
        }
    }
    else
    {
        FileSystem_ReadFile( buffer, info->bytes, info->width, fp );
    }

    if( info->flipHoriz )
    {
        flip_line( buffer, info );
    }

    if( info->imageType == TGA_TYPE_COLOR )
    {
        if( info->bpp == 15 )
        {
            upsample( row, buffer, info->width, info->bytes, info->alphaBits );
        }
        else
        {
            bgr2rgb( row, buffer, info->width, info->bytes, info->bytes == 4 ? 1 : 0 );
        }
    }
    else
    {
        memcpy( row, buffer, info->width * info->bpp );
    }
}


void LoadTGA( const char *filename, unsigned char **pic, unsigned short *width, unsigned short *height, unsigned short *bytes )
{
    TargaHeader		targa_header;
    unsigned char    header[ 18 ];
    unsigned char    footer[ 26 ];
    unsigned char    extension[ 495 ];
    unsigned int  cmap_bytes;
    unsigned int offset;
    unsigned char tga_cmap[4 * 256], gimp_cmap[3 * 256];
    unsigned char *buffer, *data, *row;
    int i;
    unsigned int datalength;
    filehandle_t *hFile;

    hdPrintf( "LoadTGA: Attempting to open (%s) for reading\n", filename );

    *pic = NULL;

    //
    // Load the file
    //
    hFile = FileSystem_OpenFile( filename, 0 );
    if( ! hFile )
    {
        hdPrintf( "LoadTGA: Could not open (%s) for reading\n", filename );

        goto TGALOADFAILED;
    }

    datalength = FileSystem_GetFileSize( hFile );

    if( ! FileSystem_FileSeek( hFile, -26L, SEEK_END ) )
    {
        /* Is file big enough for a footer? */
        if( FileSystem_ReadFile( footer, sizeof( footer ), 1, hFile ) != 1 )
        {
            hdPrintf( "Cannot read footer from (%s)\n" , filename );

            goto TGALOADFAILED;
        }
        else if( memcmp( footer + 8, magic, sizeof( magic ) ) == 0 )
        {
            /* Check the signature. */
            offset = footer[ 0 ] + (footer[ 1 ] * 256) + (footer[ 2 ] * 65536)
            + (footer[ 3 ] * 16777216);

            if( offset != 0 )
            {
                if( FileSystem_FileSeek( hFile, offset, SEEK_SET ) ||
                   FileSystem_ReadFile( extension, sizeof( extension ), 1, hFile ) != 1 )
                {
                    hdPrintf( "Cannot read extension from '%s'\n", filename );

                    goto TGALOADFAILED;
                }

                /* Eventually actually handle version 2 TGA here */

            }
        }
    }


    //
    // Get header information.
    //
    if( datalength < TGA_HEADER_SIZE )
    {
        hdPrintf( "Could not read header from (%s)\n", filename );

        goto TGALOADFAILED;
    }

    if( FileSystem_FileSeek( hFile, 0, SEEK_SET ) ||
       FileSystem_ReadFile( header, sizeof( header ), 1, hFile ) != 1 )
    {
        hdPrintf( "Cannot read header from (%s)\n", filename );

        goto TGALOADFAILED;
    }



    targa_header.idLength = header[ 0 ];
    targa_header.colorMapType = header[ 1 ];

    switch( header[ 2 ] )
    {
        case 1:
            targa_header.imageType = TGA_TYPE_MAPPED;
            targa_header.imageCompression = TGA_COMP_NONE;
            break;

        case 2:
            targa_header.imageType = TGA_TYPE_COLOR;
            targa_header.imageCompression = TGA_COMP_NONE;
            break;

        case 3:
            targa_header.imageType = TGA_TYPE_GRAY;
            targa_header.imageCompression = TGA_COMP_NONE;
            break;

        case 9:
            targa_header.imageType = TGA_TYPE_MAPPED;
            targa_header.imageCompression = TGA_COMP_RLE;
            break;

        case 10:
            targa_header.imageType = TGA_TYPE_COLOR;
            targa_header.imageCompression = TGA_COMP_RLE;
            break;

        case 11:
            targa_header.imageType = TGA_TYPE_GRAY;
            targa_header.imageCompression = TGA_COMP_RLE;
            break;


        default:
            targa_header.imageType = 0;
    }

    targa_header.colorMapIndex  = header[ 3 ] + header[ 4 ] * 256;
    targa_header.colorMapLength = header[ 5 ] + header[ 6 ] * 256;
    targa_header.colorMapSize   = header[ 7 ];

    targa_header.xOrigin = header[ 8  ] + header[ 9  ] * 256;
    targa_header.yOrigin = header[ 10 ] + header[ 11 ] * 256;
    targa_header.width   = header[ 12 ] + header[ 13 ] * 256;
    targa_header.height  = header[ 14 ] + header[ 15 ] * 256;

    targa_header.bpp       = header[ 16 ];
    targa_header.bytes     = (targa_header.bpp + 7) / 8;
    targa_header.alphaBits = header[ 17 ] & 0x0f; /* Just the low 4 bits */
    targa_header.flipHoriz = (header[ 17 ] & 0x10) ? 1 : 0;
    targa_header.flipVert  = (header[ 17 ] & 0x20) ? 0 : 1;

    //
    // Analyze header information.
    //
    switch( targa_header.imageType )
    {
        case TGA_TYPE_MAPPED:
            if( targa_header.bpp != 8 )
            {
                hdPrintf( "Unhandled sub-format in (%s)\n", filename );

                goto TGALOADFAILED;
            }

            goto TGALOADFAILED;

            break;

        case TGA_TYPE_COLOR:
            //if( targa_header.bpp != 15 && targa_header.bpp != 16 && targa_header.bpp != 24
            //   && targa_header.bpp != 32 )
            if( targa_header.bpp != 15 && targa_header.bpp != 16 && targa_header.bpp != 24
               && targa_header.bpp != 32  && targa_header.bpp != 4)
            {
                hdPrintf( "Unhandled sub-format in (%s)\n", filename );
                goto TGALOADFAILED;
            }
            break;

        case TGA_TYPE_GRAY:
            if( targa_header.bpp != 8 && (targa_header.alphaBits != 8 || (targa_header.bpp != 16 && targa_header.bpp != 15 )))
            {
                hdPrintf( "Unhandled sub-format in (%s)\n", filename );
                goto TGALOADFAILED;
            }


            goto TGALOADFAILED;


            break;

        default:
            hdPrintf( "Unknown image type for (%s)\n", filename );
            goto TGALOADFAILED;

    } /* end of switch targa_header.imageType */

    /* Plausible but unhandled formats */
    if( targa_header.bytes * 8 != targa_header.bpp && ! (targa_header.bytes == 2 && targa_header.bpp == 15) )
    {
        hdPrintf( "No support yet for TGA with these parameters\n" );

        goto TGALOADFAILED;
    }

    /* Check that we have a color map only when we need it. */
    if( targa_header.imageType == TGA_TYPE_MAPPED && targa_header.colorMapType != 1 )
    {
        hdPrintf( "Indexed image has invalid color map type %d\n",
                 targa_header.colorMapType );

        goto TGALOADFAILED;
    }
    else if( targa_header.imageType != TGA_TYPE_MAPPED && targa_header.colorMapType != 0 )
    {
        hdPrintf( "Non-indexed image has invalid color map type %d\n",
                 targa_header.colorMapType );

        goto TGALOADFAILED;
    }

    /* Skip the image ID field. */
    if( targa_header.idLength && FileSystem_FileSeek( hFile, targa_header.idLength, SEEK_CUR ) )
    {
        hdPrintf( "File (%s) is truncated or corrupted\n", filename );

        goto TGALOADFAILED;
    }



    /* Handle colormap */
    if( targa_header.colorMapType == 1 )
    {
        cmap_bytes = (targa_header.colorMapSize + 7 ) / 8;
        if( cmap_bytes <= 4 &&
           FileSystem_ReadFile( tga_cmap, targa_header.colorMapLength * cmap_bytes, 1, hFile ) == 1 )
        {
            if( targa_header.colorMapSize == 32 )
            {
                bgr2rgb( gimp_cmap, tga_cmap, targa_header.colorMapLength, cmap_bytes, 1);
            }
            else if( targa_header.colorMapSize == 24 )
            {
                bgr2rgb( gimp_cmap, tga_cmap, targa_header.colorMapLength, cmap_bytes, 0);
            }
            else if( targa_header.colorMapSize == 16 || targa_header.colorMapSize == 15 )
            {
                upsample( gimp_cmap, tga_cmap, targa_header.colorMapLength, cmap_bytes, targa_header.alphaBits);
            }

        }
        else
        {
            hdPrintf( "File (%s) is truncated or corrupted\n", filename );

            goto TGALOADFAILED;
        }
    }


    /* Allocate the data. */
    data = (unsigned char *) calloc(1, targa_header.width * targa_header.height * targa_header.bytes );
    if( data == NULL )
    {
        //MM_OUTOFMEM( "data" );
    }

    buffer = (unsigned char *) calloc(1, targa_header.width * targa_header.bytes );
    if( buffer == NULL )
    {
        free( data );
        //MM_OUTOFMEM( "buffer" );
    }

    if( targa_header.flipVert )
    {
        for( i = targa_header.height-1 ; i >= 0 ; --i )
        {
            row = data + (targa_header.width * targa_header.bytes * i);
            read_line( hFile, row, buffer, &targa_header );
        }
    }
    else
    {
        for( i = 0 ; i < targa_header.height ; ++i )
        {
            row = data + (targa_header.width * targa_header.bytes * i);
            read_line( hFile, row, buffer, &targa_header );
        }
    }
    
    
    free( buffer );
    
    FileSystem_CloseFile( hFile );
    
    *pic = data;
    *width = targa_header.width;
    *height = targa_header.height;
    *bytes = targa_header.bytes;
    
    return;
    
    
    
TGALOADFAILED:
    *pic = NULL;
    *width = 0;
    *height = 0;
    *bytes = 0;
    
    if( hFile )
    {
        FileSystem_CloseFile( hFile );
    }
}
