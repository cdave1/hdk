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

#include "Scripting.h"

#include <hdk/collections.h>
#include <hdk/core.h>
#include <map>
#include <regex>

static hdTypedefList<totemMessage, 256> *currentCache = NULL;
static hdTypedefList<totemGoal, 256> *goalCache = NULL;
static char currentWorldFilePath[256];

static std::map<std::string, std::string> stringTable;
static bool stringsFileWasLoaded = false;


void Scripts_Init()
{
    if (currentCache == NULL)
    {
        currentCache = new hdTypedefList<totemMessage, 256>();
    }

    if (goalCache == NULL)
    {
        goalCache = new hdTypedefList<totemGoal, 256>();
    }
}


void Scripts_TearDown()
{
    if (currentCache != NULL)
    {
        delete currentCache;
        currentCache = NULL;
    }

    if (goalCache != NULL)
    {
        delete goalCache;
        goalCache = NULL;
    }
}


void Scripts_LoadStringsFile(const char *stringsFilePath)
{
    std::string line;
    size_t pos;
    filehandle_t *hnd;

    stringTable.clear();

    // Matchings
    std::smatch matchings;

    // string line pattern: $Key "$Value"
    std::regex stringLinePattern("^\\s*([^[:space:]]+)\\s+[\"](.+)[\"]$");

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

        if (std::regex_match(line, matchings, stringLinePattern))
        {
            hdAssert(line == matchings[0]);
            hdAssert(3 == matchings.size());

            // New line hack
            std::string s(matchings[2]);
            while((pos = s.find("\\n")) != std::string::npos)
            {
                s.replace(pos, 2, 1, '\n');
            }

            stringTable[matchings[1]] = s;
        }
    }
    FileSystem_CloseFile(hnd);
    stringsFileWasLoaded = true;
}


std::string Scripts_GetStringForKey(const char *key)
{
    hdAssert(stringsFileWasLoaded);

    std::string sKey(key);

    if (stringTable.count(sKey) == 0) return std::string(key);

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

    snprintf(currentWorldFilePath, 256, "%s", scriptfilepath);
    currentCache->RemoveAll();
    goalCache->RemoveAll();
    ParseWorldMessageScript(scriptfilepath);
}


int Scripts_GetLevelMessagesForTag(totemMessage** messages,
                                   const int& messageCountMax,
                                   const int& level,
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
        if (target == HDK_SCRIPTS_WILDCARD_TAG)
        {
            targetMatch = true;
        }
        else
        {
            targetMatch = currentCache->GetItems()[i].targetId == target;
        }

        if (currentCache->GetItems()[i].levelId == level && targetMatch)
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


int Scripts_CountLevelMessagesForTag(const int& level,
                                     const int& target)
{
    int matches;
    unsigned i;

    hdAssert(currentCache != NULL);

    matches = 0;
    for (i = 0; i < currentCache->GetItemCount(); ++i)
    {
        if (currentCache->GetItems()[i].levelId == level &&
            currentCache->GetItems()[i].targetId == target)
        {
            ++matches;
        }
    }

    return matches;
}


int Scripts_GetGoalsForLevel(totemGoal *goals,
                             const int& goalCountMax,
                             const int& level)
{
    int matches;
    unsigned i;

    hdAssert(goalCache != NULL);

    matches = 0;
    for (i = 0; i < goalCache->GetItemCount(); ++i)
    {
        if (goalCache->GetItems()[i].levelId == level)
        {
            goals[matches++] = goalCache->GetItems()[i];
        }

        if (matches >= goalCountMax)
        {
            break;
        }
    }

    return matches;
}



void Scripts_SetDescriptionForGoal(totemGoal& goal)
{
    switch(goal.goalType)
    {
        case e_hdkGoalTypeNormal:
            snprintf(goal.description, HDK_GAME_GoalDescriptionStringSize, "Goal_Normal");
            break;
        case e_hdkGoalTypeMinBlocksPercentage:
            snprintf(goal.description, HDK_GAME_GoalDescriptionStringSize, "Goal_MinBlocks");
            break;
        case e_hdkGoalTypeMinPoints:
            snprintf(goal.description, HDK_GAME_GoalDescriptionStringSize, "Goal_MinPoints");
            break;
        case e_hdkGoalTypeFragments:
            snprintf(goal.description, HDK_GAME_GoalDescriptionStringSize, "Goal_Fragments");
            break;
        case e_hdkGoalTypeJackCollection:
            snprintf(goal.description, HDK_GAME_GoalDescriptionStringSize, "Goal_Jacks");
            break;
        case e_hdkGoalTypeAllBlocksWithTag:
            break;
    }
}


// Buggy poor implentation of half of a packrat parser
void ParseWorldMessageScript(const char* scriptfilepath)
{
    std::string line;
    bool levelOpened;
    int currLevelID;
    size_t pos;
    totemMessage msg;
    totemGoal goal;
    char goalType[32];
    char key[32];
    filehandle_t *hnd;
    levelOpened = false;

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
    std::regex levelLinePattern("^(LEVEL)\\s+(\\d+)\\s*$");

    // ("TEXT")(Spaces)(Digits)(Spaces)("Anything")
    std::regex textLinePattern("^(TEXT)\\s+(\\d+)\\s+(.+)$");

    // ("IMAGE")(Spaces)(Digits)(Spaces)("Anything")
    std::regex imageLinePattern("^(IMAGE)\\s+(\\d+)\\s+(.+)$");

    // ("CUSTOMTEXT")(Spaces)(Digits)(Spaces)(@ImageTextureName)(Scaces)(@Message)
    std::regex customTextLinePattern("^(CUSTOMTEXT)\\s+(\\d+)\\s+([^[:space:]]+)\\s+[\"](.+)[\"]$");

    // ("CUSTOMTEXT")(Spaces)(Digits)(Spaces)(@ImageTextureName)(Scaces)(@Message)
    std::regex avatarTextLinePattern("^(AVATARMESSAGE)\\s+(\\d+)\\s+([^[:space:]]+)\\s+[\"](.+)[\"]$");

    // Goals
    std::regex goalLinePattern("^(GOAL)\\s+([A-Za-z]+)\\s+([[:digit:].]+)$");

    // Matchings
    std::smatch matchings;

    // Parse line by line
    while(!iss.eof())
    {
        getline(iss, line);

        if (std::regex_match(line, matchings, levelLinePattern))
        {
            levelOpened = true;
            hdAssert(line == matchings[0]);
            hdAssert(matchings.size() == 3);
            std::string s(matchings[2]);
            currLevelID = (int)strtol(s.c_str(), NULL, 0);
        }
        else if (std::regex_match(line, matchings, textLinePattern))
        {
            if (!levelOpened)
            {
                hdPrintf("Scripts file malformed, bailing out: %s\n", scriptfilepath);
                break;
            }

            hdAssert(line == matchings[0]);
            hdAssert(4 == matchings.size());

            msg.levelId = currLevelID;
            msg.targetId = (int)strtol(std::string(matchings[2]).c_str(), NULL, 0);

            // New line hack
            std::string s(matchings[3]);
            while((pos = s.find("\\n")) != std::string::npos)
            {
                s.replace(pos, 2, 1, '\n');
            }

            snprintf(msg.message, 256, "%s", s.c_str());
            msg.messageType = e_hdkMessageTypeText;

            if (HD_COLLECTIONS_ERROR_FULL == currentCache->Add(msg))
            {
                hdPrintf("Script cache full, bailing out: %s\n", scriptfilepath);
                break;
            }
        }
        else if (std::regex_match(line, matchings, imageLinePattern))
        {
            if (!levelOpened)
            {
                hdPrintf("Scripts file malformed, bailing out: %s\n", scriptfilepath);
                break;
            }

            hdAssert(line == matchings[0]);
            hdAssert(4 == matchings.size());

            msg.levelId = currLevelID;
            msg.targetId = (int)strtol(std::string(matchings[2]).c_str(), NULL, 0);
            snprintf(msg.texture, 256, "%s", std::string(matchings[3]).c_str());
            msg.messageType = e_hdkMessageTypeImage;

            if (HD_COLLECTIONS_ERROR_FULL == currentCache->Add(msg))
            {
                hdPrintf("Script cache full, bailing out: %s\n", scriptfilepath);
                break;
            }
        }
        else if (std::regex_match(line, matchings, customTextLinePattern))
        {
            if (!levelOpened)
            {
                hdPrintf("Scripts file malformed, bailing out: %s\n", scriptfilepath);
                break;
            }

            hdAssert(line == matchings[0]);
            hdAssert(5 == matchings.size());

            msg.levelId = currLevelID;
            msg.targetId = (int)strtol(std::string(matchings[2]).c_str(), NULL, 0);
            snprintf(msg.texture, 256, "%s", std::string(matchings[3]).c_str());

            // New line hack
            std::string s(matchings[4]);
            while((pos = s.find("\\n")) != std::string::npos)
            {
                s.replace(pos, 2, 1, '\n');
            }

            snprintf(msg.message, 256, "%s", s.c_str());


            msg.messageType = e_hdkMessageTypeCustomImageText;

            if (HD_COLLECTIONS_ERROR_FULL == currentCache->Add(msg))
            {
                hdPrintf("Script cache full, bailing out: %s\n", scriptfilepath);
                break;
            }
        }
        else if (std::regex_match(line, matchings, avatarTextLinePattern))
        {
            if (!levelOpened)
            {
                hdPrintf("Scripts file malformed, bailing out: %s\n", scriptfilepath);
                break;
            }

            hdAssert(line == matchings[0]);
            hdAssert(5 == matchings.size());
            
            msg.levelId = currLevelID;
            msg.targetId = (int)strtol(std::string(matchings[2]).c_str(), NULL, 0);
            snprintf(msg.texture, 256, "%s", std::string(matchings[3]).c_str());
            
            // New line hack
            std::string s(matchings[4]);
            while((pos = s.find("\\n")) != std::string::npos)
            {
                s.replace(pos, 2, 1, '\n');
            }
            
            snprintf(msg.message, 256, "%s", s.c_str());
            
            msg.messageType = e_hdkMessageTypeAvatar;
            
            if (HD_COLLECTIONS_ERROR_FULL == currentCache->Add(msg))
            {
                hdPrintf("Script cache full, bailing out: %s\n", scriptfilepath);
                break;
            }
        }
        else if (std::regex_match(line, matchings, goalLinePattern))
        {
            if (!levelOpened)
            {
                hdPrintf("Scripts file malformed, bailing out: %s\n", scriptfilepath);
                break;
            }
            
            hdAssert(line == matchings[0]);
            hdAssert(4 == matchings.size());
            
            goal.levelId = currLevelID;
            goal.state = 0;
            snprintf(goalType, 32, "%s", std::string(matchings[2]).c_str());
            
            goal.value = (int)strtol(std::string(matchings[3]).c_str(), NULL, 0);
            
            if (strncmp(goalType, "NORMAL", 256) == 0)
            {
                goal.goalType = e_hdkGoalTypeNormal;
                snprintf(goal.description, HDK_GAME_GoalDescriptionStringSize, 
                         Scripts_GetStringForKey("Goal_Normal").c_str(), goal.value);
            }
            if (strncmp(goalType, "MINPERCENT", 256) == 0)
            {
                goal.goalType = e_hdkGoalTypeMinBlocksPercentage;
                snprintf(goal.description, HDK_GAME_GoalDescriptionStringSize, 
                         Scripts_GetStringForKey("Goal_MinBlocks").c_str(), goal.value);
            }
            if (strncmp(goalType, "POINTS", 256) == 0)
            {
                goal.goalType = e_hdkGoalTypeMinPoints;
                snprintf(goal.description, HDK_GAME_GoalDescriptionStringSize, 
                         Scripts_GetStringForKey("Goal_MinPoints").c_str(), goal.value);
            }
            if (strncmp(goalType, "FRAGMENTS", 256) == 0)
            {
                goal.goalType = e_hdkGoalTypeFragments;
                snprintf(goal.description, HDK_GAME_GoalDescriptionStringSize, 
                         Scripts_GetStringForKey("Goal_Fragments").c_str(), goal.value);
            }
            if (strncmp(goalType, "JACKS", 256) == 0)
            { 
                if (s_worldId != -1)
                {
                    snprintf(key, 32, "Chapter%d_Collectible", s_worldId);
                }
                
                if (strcmp(Scripts_GetStringForKey(key).c_str(), key) == 0)
                {
                    snprintf(key, 32, "Default_Collectible");
                }
                
                goal.goalType = e_hdkGoalTypeJackCollection;
                snprintf(goal.description, HDK_GAME_GoalDescriptionStringSize, 
                         Scripts_GetStringForKey("Goal_Jacks").c_str(), 
                         goal.value,
                         Scripts_GetStringForKey(key).c_str());
            }
            if (strncmp(goalType, "TAGBLOCK", 256) == 0)
            {
                goal.goalType = e_hdkGoalTypeAllBlocksWithTag;				
            }
            if (strncmp(goalType, "MULTIPLIERPERCENT", 256) == 0)
            {
                goal.goalType = e_hdkGoalTypeMultiplier;
                goal.description[0] = '\0';
            }
            
            if (HD_COLLECTIONS_ERROR_FULL == goalCache->Add(goal))
            {
                hdPrintf("Goal cache full, bailing out: %s\n", scriptfilepath);
                break;
            }
        }
    }
    FileSystem_CloseFile(hnd);
}
