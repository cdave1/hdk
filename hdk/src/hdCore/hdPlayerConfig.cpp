/*
 *  hdPlayerConfig.cpp
 *  TotemGame
 *
 *  Created by david on 19/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdPlayerConfig.h"

static map<string, string> configLookupTable;


string hdPlayerConfig::GetValue(const char *key)
{
	string sKey(key);
	
	if (configLookupTable.count(sKey) == 0) return string(key);
	
	return configLookupTable[sKey];
}


bool hdPlayerConfig::SetValue(const char *key, const char *value)
{
	string sKey(key);
	string sValue(value);
	
	configLookupTable[sKey] = sValue;
	
	return true;
}


bool hdPlayerConfig::SaveConfigFile()
{
	// create file.
	// key = value pairs to string and dump to a file at path
//	return true;
	
	int createRes, writeRes;
	string sConfig;
	char tmp[256];
	
	if (!FileSystem_FileExists("player.config"))
	{
		createRes = FileSystem_CreateFile("player.config");
		if (0 != createRes) 
			return createRes;
	}
	
	map<string, string>::iterator iter;
	
	/*
	 * Create each pak file.
	 */
	for (iter = configLookupTable.begin(); 
		 iter != configLookupTable.end(); 
		 iter++)
	{
		snprintf(tmp, 256, "%s = \"%s\"\n", (*iter).first.c_str(), (*iter).second.c_str());
		
		sConfig = sConfig + string(tmp);
	}
	
	writeRes = FileSystem_WriteToFile("player.config", sConfig.c_str(), sConfig.length());
	
	return writeRes;
}


bool hdPlayerConfig::LoadConfigFile()
{
	string line;
	void *buffer;
	uint32 pos;
	int len;
	
	configLookupTable.clear();
	
	// Matchings
	boost::smatch matchings;
	
	// string line pattern: $Key = "$Value" 
	boost::regex stringLinePattern("^\\s*([[:alnum:]]+)\\s*[=]\\s*[\"]([[:alnum:].,]+)[\"]\\s*$");
	
	if (false == FileSystem_FileExists("player.config"))
	{
		hdPrintf("[hdPlayerConfig::LoadConfigFile] player.config does not exist.\n");
		return false;
	}
	
	
	if (0 != FileSystem_ReadFromFile(&buffer, &len, "player.config"))
	{
		return false;
	}
	
	std::istringstream iss(ios::in | ios::out);
	iss.rdbuf()->sputn((const char*)buffer, len);
	free(buffer);
	
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
	
	return true;
}
