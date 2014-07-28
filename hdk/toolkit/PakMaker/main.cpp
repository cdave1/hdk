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

#define DEBUG 1

#include <stdio.h>
#include <iostream>
#include "pakProjectLoader.h"

int main (int argc, char * const argv[])
{
    // - dest folder, absolute path.
    // - Input strings file
    if (argc <= 1)
    {
        PakLoader_TestPak("/Users/davidpetrie/Dropbox/Hackdirt/TotemGames/res/BaseDir/", "Scripts/en/smashed.strings");
        PakLoader_TestPak("/Users/davidpetrie/Dropbox/Hackdirt/TotemGames/res/BaseDir/", "Interface/hd_logo1_512.png");
        PakLoader_TestPak("/Users/davidpetrie/Dropbox/Hackdirt/TotemGames/res/BaseDir/", "Fonts/bebas8.f");
        PakLoader_TestPak("/Users/davidpetrie/Dropbox/Hackdirt/TotemGames/res/BaseDir/", "Fonts/bebas10.f");
        PakLoader_TestPak("/Users/davidpetrie/Dropbox/Hackdirt/TotemGames/res/BaseDir/", "Fonts/bebas11.f");
        PakLoader_TestPak("/Users/davidpetrie/Dropbox/Hackdirt/TotemGames/res/BaseDir/", "Fonts/bebas12.f");
        PakLoader_TestPak("/Users/davidpetrie/Dropbox/Hackdirt/TotemGames/res/BaseDir/", "Fonts/bebas14.f");
        PakLoader_TestPak("/Users/davidpetrie/Dropbox/Hackdirt/TotemGames/res/BaseDir/", "Fonts/bebas16.f");

    }
    else if (argc == 3) {
        hdPrintf("Creating Pak...\n");
        PakLoader_CreatePak("", argv[argc-2], argv[argc-1]);
    }
    else if (argc == 4)
    {
        hdPrintf("Creating Pak...\n");
        PakLoader_CreatePak(argv[argc-3], argv[argc-2], argv[argc-1]);
    }
    
    return 0;
}
