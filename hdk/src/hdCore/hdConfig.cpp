/*
 *  hdConfig.cpp
 *  TotemGame
 *
 *  Created by david on 26/09/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdConfig.h"

static map<string, string> configLookupTable;
static bool configFileWasLoaded = false;




void hdConfig::LoadConfigFile(const char *configFilePath)
{
	string line;
	unsigned pos;
	filehandle_t *hnd;
	
	configLookupTable.clear();
	
	// Matchings
	boost::smatch matchings;
	
	// string line pattern: $Key = "$Value" 
	boost::regex stringLinePattern("^\\s*([[:alnum:]]+)\\s*[=]\\s*[\"]([[:alnum:].,]+)[\"]\\s*$");
	
	hdPrintf("[hdConfig::LoadConfigFile] Loading config file at: %s\n", configFilePath);
	
	if (NULL == (hnd = FileSystem_OpenFile(configFilePath, 0)))
	{
		hdPrintf("[hdConfig::LoadConfigFile] Could not load config file at: %s. Loading Defaults\n", configFilePath);
		
		// Default values are deliberately conservative.
		configLookupTable[string(CONFIG_PHYSICSITERATIONSCOUNT_KEY)] = string("5");
		configLookupTable[string(CONFIG_FIXEDPHYSICSINTERVAL_KEY)] = string("0");
		configLookupTable[string(CONFIG_MAXTEXTUREBPP_KEY)] = string("2");
		configLookupTable[string(CONFIG_MINFRAMEDURATION_KEY)] = string("0.032");
		configLookupTable[string(CONFIG_TEXTUREQUALITYLEVEL_KEY)] = string("Medium");
		configLookupTable[string(CONFIG_EFFECTSQUALITYLEVEL_KEY)] = string("Medium");
		configLookupTable[string(CONFIG_LIGHTINGLEVEL_KEY)] = string("Medium");
		
		configFileWasLoaded = true;
		return;
	}
	
	std::istringstream iss((char *)hnd->filedata);
	
	// Parse line by line
	while(!iss.eof())
	{
		getline(iss, line);
		
		if (boost::regex_match(line, matchings, stringLinePattern))
		{
			hdAssert(line == matchings[0]);
			hdAssert(3 == matchings.size());
			
			// New line hack
			string s(matchings[2]);
			while((pos = s.find("\\n")) != string::npos)
			{
				s.replace(pos, 2, 1, '\n');
			}
			
			configLookupTable[matchings[1]] = s;
		}
	}
	
	FileSystem_CloseFile(hnd);
	configFileWasLoaded = true;
}


void hdConfig::LoadNullConfig()
{
	configLookupTable.clear();
	configFileWasLoaded = true;

}


string hdConfig::GetValueForKey(const char *key)
{
	//hdAssert(configFileWasLoaded);
	
	string sKey(key);
	
	if (configLookupTable.count(sKey) == 0) return string(key);
	
	return configLookupTable[sKey];
}


bool hdConfig::ConfigWasInitialized()
{
	return configFileWasLoaded;
}