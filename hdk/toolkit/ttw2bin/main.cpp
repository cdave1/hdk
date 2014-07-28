#define DEBUG 1

#include <stdio.h>
#include <iostream>
#include "hdk_min.h"
#include "totemGame.h"

/**
 * Converts ttw text file into a 32 bit binary for use on mobile devices
 * running iPhone OS.
 *
 * Load in a list of absolute file paths as args, convert 'em to bin by shoving "bin" on the end of the file name.
 */
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
