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
 * Converts ttw text file into a 32 bit binary for use on mobile devices
 * running iPhone OS.
 *
 * Load in a list of absolute file paths as args, convert 'em to bin by shoving "bin" on the end of
 * the file name.
 */

#define DEBUG 1

#include <stdio.h>
#include <iostream>
#include <hdk/hdk.h>
#include "totemGame.h"


int main (int argc, char * const argv[])
{
    if (argc == 1)
    {
        hdPrintf("Usage: ttw2bin [list-of-absolute-paths-to-ttw-files-separated-by-spaces]\n");
        return 0;
    }

    const char *src;
    for (int i = 0; i < argc; ++i)
    {
        if ((src = argv[i + 1]) && strlen(src) > 0)
        {
            totemWorld *tw = totemWorldManager::Instance()->LoadTotemWorldFromText(src);

            if (tw)
            {
                hdPrintf("World loaded successfully with name: %s\n", tw->GetName().c_str());
                totemWorldManager::Instance()->SaveTotemWorldBinary(tw, src);
                delete tw;
            }
        }
    }
    return 0;
}
