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

#include "hdCompression.h"

#undef Byte

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "hdCommon.h"
#include <zlib.h>

int hdCompression_Zip(const char* inBuffer, const long inLen,
                      char **outBuffer, long* outLen)
{
    /* zlib stuff */
    Bytef *zDestBuffer;
    uLongf zDestLen;
    Bytef *zSourceBuffer;
    uLongf zSourceLen;
    int zRes;

    /*
     * Compress the output stream before saving to disk.
     From zlib:

     int compress(byte *dest, long *destLen, byte *source, long sourceLen)
     - sourceLen: byte lenght of the source buffer
     On entry:
     - destLen is the total size of the destination buffer. The buffer
     must be at 0.1% larger than sourceLen + 12 bytes
     On exit:
     - destLen is the actual size of the compressed buffer...
     */
    zSourceLen = (uLongf)inLen;
    if (NULL == (zSourceBuffer = (Bytef *)calloc(1, sizeof(Bytef) * (zSourceLen + 1))))
    {
        hdPrintf("MALLOC ERROR\n");
        return -1;
    }
    memcpy(zSourceBuffer, inBuffer, inLen * sizeof(char));

    zDestLen = (uLongf)(zSourceLen + 10000);

    if (NULL == (zDestBuffer = (Bytef *)calloc(1, sizeof(Bytef) * zDestLen)))
    {
        hdPrintf("MALLOC ERROR\n");
        return -1;
    }

    zRes = compress(zDestBuffer, &zDestLen, zSourceBuffer, zSourceLen);
    hdAssert(zRes == Z_OK);

    /*
     * Prefix the uncompressed filesize to the stream.
     */
    (*outLen) = (long)(zDestLen + 16);
    if (NULL == ((*outBuffer) = (char *)calloc(1, sizeof(char) * (zDestLen + 16))))
    {
        hdPrintf("MALLOC ERROR\n");
        return -1;
    }

    snprintf(*outBuffer, 16, "%-16ld", (long)zSourceLen);
    memcpy((*outBuffer)+16, zDestBuffer, (long)(zDestLen * sizeof(char)));

    free(zSourceBuffer);
    free(zDestBuffer);

    return 0;
}


int hdCompression_Unzip(const char* inBuffer, const long inLen,
                        char **outBuffer, long* outLen)
{
    /* zlib stuff */
    Bytef *zDestBuffer;
    uLongf zDestLen;
    Bytef *zSourceBuffer;
    uLongf zSourceLen;
    int zRes;

    /* first 16 bytes containing uncompressed file size */
    char tmp[16];
    strncpy(tmp, inBuffer, 16);
    zDestLen = (uLongf)strtol(tmp, NULL, 0);
    zDestBuffer = new Bytef[zDestLen];

    if ( zDestLen <= 0)
    {
        hdPrintf("Data file is not a compressed world file - aborting...");
        delete [] zDestBuffer;
        return -1;
    }

    zSourceLen = (uLongf)(inLen - 16);
    zSourceBuffer = new Bytef[zSourceLen];
    memcpy(zSourceBuffer, inBuffer+16, sizeof(Bytef) * zSourceLen);

    /*
     via zlib docs:

     ZEXTERN int ZEXPORT uncompress OF((Bytef *dest,   uLongf *destLen,
     const Bytef *source, uLong sourceLen));

     Decompresses the source buffer into the destination buffer.  sourceLen is
     the byte length of the source buffer. Upon entry, destLen is the total
     size of the destination buffer, which must be large enough to hold the
     entire uncompressed data. (The size of the uncompressed data must have
     been saved previously by the compressor and transmitted to the decompressor
     by some mechanism outside the scope of this compression library.)
     Upon exit, destLen is the actual size of the compressed buffer.
     This function can be used to decompress a whole file at once if the
     input file is mmap'ed.

     uncompress returns Z_OK if success, Z_MEM_ERROR if there was not
     enough memory, Z_BUF_ERROR if there was not enough room in the output
     buffer, or Z_DATA_ERROR if the input data was corrupted or incomplete.
     */
    zRes = uncompress(zDestBuffer, &zDestLen, zSourceBuffer, zSourceLen);
    if ( zRes != Z_OK)
    {
        hdPrintf("Could not decompress data in the file - aborting...");
        delete [] zDestBuffer;
        delete [] zSourceBuffer;
        return -1;
    }

    (*outLen) = (long)zDestLen;
    (*outBuffer) = new char[(*outLen)];
    memcpy(*outBuffer, zDestBuffer, (long)((*outLen) * sizeof(char)));

    delete [] zDestBuffer;
    delete [] zSourceBuffer;
    
    return 0;
}
