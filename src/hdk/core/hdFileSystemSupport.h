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

#ifndef HD_FILE_SYSTEM_SUPPORT
#define HD_FILE_SYSTEM_SUPPORT

typedef struct
{
    /* Following is used when the file is loaded into memory */
    bool bLoaded;			/* Was file loaded into memory? */
    bool mmapped; // Did we use mmap?

    unsigned int    filesize;				/* Size of file data in bytes */

    unsigned char   *ptrStart;				/* pointer to start of file data block */
    unsigned char   *ptrCurrent;			/* pointer to current position in file data block */
    unsigned char   *ptrEnd;				/* pointer to end of file data block */

    void *filedata;				/* file data loaded into memory */

} filehandle_t;

#endif
