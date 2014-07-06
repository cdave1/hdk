/*
 *  hdMessage.cpp
 *  TotemGame
 *
 *  Created by david on 14/08/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */


#include "hdScripts.h"

static hdTypedefList<hdMessage, 256> *currentCache = NULL;
static char currentWorldFilePath[256];

static map<string, string> stringTable;
static bool stringsFileWasLoaded = false;


void Scripts_Init()
{
	if (currentCache == NULL)
	{
		currentCache = new hdTypedefList<hdMessage, 256>();
	}
}


void Scripts_TearDown()
{
	if (currentCache != NULL)
	{
		delete currentCache;
		currentCache = NULL;
	}
}


void Scripts_LoadStringsFile(const char *stringsFilePath)
{
	string line;
	unsigned pos;
	filehandle_t *hnd;
	
	stringTable.clear();
	
	// Matchings
	boost::smatch matchings;
	
	// string line pattern: $Key "$Value" 
	boost::regex stringLinePattern("^\\s*([^[:space:]]+)\\s+[\"](.+)[\"]$");
	
	if (NULL == (hnd = FileSystem_OpenFile(stringsFilePath, 0)))
	{
		hdPrintf("Could not load strings file: %s\n", stringsFilePath);
		return;
	}
	
	std::istringstream iss((char *)hnd->filedata);
	if (!iss.good())
	{
		hdPrintf("Could not load strings file: %s\n", stringsFilePath);
		return;
	}
	
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
			
			stringTable[matchings[1]] = s;
		}
	}
	FileSystem_CloseFile(hnd);
	stringsFileWasLoaded = true;
}


string Scripts_GetStringForKey(const char *key)
{
	hdAssert(stringsFileWasLoaded);
	
	string sKey(key);
	
	if (stringTable.count(sKey) == 0) return string(key);
	
	return stringTable[sKey];
}



// Loads the world with the given name...
static int s_worldId = -1;
void Scripts_LoadScript(const char *scriptfilepath, int worldId)
{
	hdAssert(currentCache != NULL);
	
	if (strcmp(scriptfilepath, currentWorldFilePath) == 0) 
	{
		hdPrintf("[SCRIPTING] Scripts file already loaded for world: %s\n", scriptfilepath);
		return;
	}
	
	s_worldId = worldId;
	
	hdPrintf("[SCRIPTING] Loading script file: %s\n", scriptfilepath);
		
	snprintf(currentWorldFilePath, 256, scriptfilepath);
	currentCache->RemoveAll();
	ParseWorldMessageScript(scriptfilepath);
}


// Get hte message in the currently loaded world 
int Scripts_GetLevelMessagesForTag(hdMessage** messages, 
								   const int& messageCountMax,
								   const int& context, 
								   const int& target)
{
	int matches;
	unsigned i;
	bool targetMatch;
	
	hdAssert(currentCache != NULL);
	
	matches = 0;
	targetMatch = false;
	for (i = 0; i < currentCache->GetItemCount(); ++i)
	{
		if (target == HD_SCRIPTS_WILDCARD_TAG) 
		{
			targetMatch = true;
		}
		else
		{
			targetMatch = currentCache->GetItems()[i].targetId == target;
		}
		
		if (currentCache->GetItems()[i].contextId == context && targetMatch)
		{
			messages[matches++] = &currentCache->GetItems()[i];
		}
		
		if (matches >= messageCountMax)
		{
			break;
		}
	}
	
	return matches;
}


int Scripts_CountContextMessagesForTag(const int& context, 
									 const int& target)
{
	int matches;
	unsigned i;
	
	hdAssert(currentCache != NULL);
	
	matches = 0;
	for (i = 0; i < currentCache->GetItemCount(); ++i)
	{
		if (currentCache->GetItems()[i].contextId == context && 
			currentCache->GetItems()[i].targetId == target)
		{
			++matches;
		}
	}
	
	return matches;
}



// Buggy poor implentation of half of a packrat parser
void ParseWorldMessageScript(const char* scriptfilepath)
{
	string line;
	bool contextOpened;
	int currContextID;
	uint32 pos;
	hdMessage msg;
	char goalType[32];
	char key[32];
	filehandle_t *hnd;
	contextOpened = false;
	//currentCache->RemoveAll();
	
	if (NULL == (hnd = FileSystem_OpenFile(scriptfilepath, 0)))
	{
		hdPrintf("Could not load strings file: %s\n", scriptfilepath);
		return;
	}
	
	std::istringstream iss((char *)hnd->filedata);
	
	if (!iss.good())
	{
		hdPrintf("Could not load scripts file: %s\n", scriptfilepath);
		return;
	}
	
	// ("LEVEL")(Spaces)(Digits)(Optional Scaces)
	boost::regex contextLinePattern("^(LEVEL)\\s+(\\d+)\\s*$");
	
	// ("TEXT")(Spaces)(Digits)(Spaces)("Anything")
	boost::regex textLinePattern("^(TEXT)\\s+(\\d+)\\s+(.+)$");
	
	// ("IMAGE")(Spaces)(Digits)(Spaces)("Anything")
	boost::regex imageLinePattern("^(IMAGE)\\s+(\\d+)\\s+(.+)$");
	
	// ("CUSTOMTEXT")(Spaces)(Digits)(Spaces)(@ImageTextureName)(Scaces)(@Message)
	boost::regex customTextLinePattern("^(CUSTOMTEXT)\\s+(\\d+)\\s+([^[:space:]]+)\\s+[\"](.+)[\"]$");
	
	// ("CUSTOMTEXT")(Spaces)(Digits)(Spaces)(@ImageTextureName)(Scaces)(@Message)
	boost::regex avatarTextLinePattern("^(AVATARMESSAGE)\\s+(\\d+)\\s+([^[:space:]]+)\\s+[\"](.+)[\"]$");
		
	// Matchings
	boost::smatch matchings;
	
	// Parse line by line
	while(!iss.eof())
	{
		getline(iss, line);
		
		if (boost::regex_match(line, matchings, contextLinePattern))
		{
			contextOpened = true;
			hdAssert(line == matchings[0]);
			hdAssert(matchings.size() == 3);
			string s(matchings[2]);
			currContextID = (int)strtol(s.c_str(), NULL, 0);
		}
		else if (boost::regex_match(line, matchings, textLinePattern))
		{
			if (!contextOpened)
			{
				hdPrintf("Scripts file malformed, bailing out: %s\n", scriptfilepath);
				break;
			}
			
			hdAssert(line == matchings[0]);
			hdAssert(4 == matchings.size());
			
			msg.contextId = currContextID;
			msg.targetId = (int)strtol(string(matchings[2]).c_str(), NULL, 0);
			
			// New line hack
			string s(matchings[3]);
			while((pos = s.find("\\n")) != string::npos)
			{
				s.replace(pos, 2, 1, '\n');
			}
			
			snprintf(msg.message, 256, "%s", s.c_str());
			msg.messageType = e_hdMessageTypeText;
			
			if (HD_COLLECTIONS_ERROR_FULL == currentCache->Add(msg))
			{
				hdPrintf("Script cache full, bailing out: %s\n", scriptfilepath);
				break;
			}
		}
		else if (boost::regex_match(line, matchings, imageLinePattern))
		{
			if (!contextOpened)
			{
				hdPrintf("Scripts file malformed, bailing out: %s\n", scriptfilepath);
				break;
			}
			
			hdAssert(line == matchings[0]);
			hdAssert(4 == matchings.size());
			
			msg.contextId = currContextID;
			msg.targetId = (int)strtol(string(matchings[2]).c_str(), NULL, 0);
			snprintf(msg.texture, 256, "%s", string(matchings[3]).c_str());
			msg.messageType = e_hdMessageTypeImage;
			
			if (HD_COLLECTIONS_ERROR_FULL == currentCache->Add(msg))
			{
				hdPrintf("Script cache full, bailing out: %s\n", scriptfilepath);
				break;
			}
		}
		else if (boost::regex_match(line, matchings, customTextLinePattern))
		{
			if (!contextOpened)
			{
				hdPrintf("Scripts file malformed, bailing out: %s\n", scriptfilepath);
				break;
			}
			
			hdAssert(line == matchings[0]);
			hdAssert(5 == matchings.size());
			
			msg.contextId = currContextID;
			msg.targetId = (int)strtol(string(matchings[2]).c_str(), NULL, 0);
			snprintf(msg.texture, 256, "%s", string(matchings[3]).c_str());
			
			// New line hack
			string s(matchings[4]);
			while((pos = s.find("\\n")) != string::npos)
			{
				s.replace(pos, 2, 1, '\n');
			}
			
			snprintf(msg.message, 256, "%s", s.c_str());
			
			msg.messageType = e_hdMessageTypeCustomImageText;
			
			if (HD_COLLECTIONS_ERROR_FULL == currentCache->Add(msg))
			{
				hdPrintf("Script cache full, bailing out: %s\n", scriptfilepath);
				break;
			}
		}
		else if (boost::regex_match(line, matchings, avatarTextLinePattern))
		{
			if (!contextOpened)
			{
				hdPrintf("Scripts file malformed, bailing out: %s\n", scriptfilepath);
				break;
			}
			 
			hdAssert(line == matchings[0]);
			hdAssert(5 == matchings.size());
			
			msg.contextId = currContextID;
			msg.targetId = (int)strtol(string(matchings[2]).c_str(), NULL, 0);
			snprintf(msg.texture, 256, "%s", string(matchings[3]).c_str());
			
			// New line hack
			string s(matchings[4]);
			while((pos = s.find("\\n")) != string::npos)
			{
				s.replace(pos, 2, 1, '\n');
			}
			
			snprintf(msg.message, 256, "%s", s.c_str());
			
			msg.messageType = e_hdMessageTypeAvatar; //e_hdMessageTypeCustomImageText;
			
			if (HD_COLLECTIONS_ERROR_FULL == currentCache->Add(msg))
			{
				hdPrintf("Script cache full, bailing out: %s\n", scriptfilepath);
				break;
			}
		}
	}
	FileSystem_CloseFile(hnd);
}
