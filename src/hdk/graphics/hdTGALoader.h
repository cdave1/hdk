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

#ifndef _HD_TGA_LOADER_H_
#define _HD_TGA_LOADER_H_

#ifdef __cplusplus
extern "C" {
#endif

    extern void LoadTGA( const char *filename, unsigned char **pic, unsigned short *width, unsigned short *height, unsigned short *bytes );

#ifdef __cplusplus
}
#endif

#endif
