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
		//PakLoader_CreatePak("/Users/david/Documents/Projects/TotemGames/GameResources/", 
		//					"/Users/david/Documents/Projects/TotemGames/GameResources/towersmash.manifest");
		
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
