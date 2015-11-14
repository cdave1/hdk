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

#include "hdPlayerConfig.h"

#include <sstream>
#include <regex>
#include <string>
#include <map>

#include <hdk/hdCore/hdFileSystem.h>

static std::map<std::string, std::string> configLookupTable;


std::string hdPlayerConfig::GetValue(const char *key)
{
    std::string sKey(key);

    if (configLookupTable.count(sKey) == 0) return std::string(key);

    return configLookupTable[sKey];
}


bool hdPlayerConfig::SetValue(const char *key, const char *value)
{
    std::string sKey(key);
    std::string sValue(value);

    configLookupTable[sKey] = sValue;

    return true;
}


bool hdPlayerConfig::SaveConfigFile()
{
    // create file.
    // key = value pairs to string and dump to a file at path

    int createRes, writeRes;
    std::string sConfig;
    char tmp[256];

    if (!FileSystem_FileExists("player.config"))
    {
        createRes = FileSystem_CreateFile("player.config");
        if (0 != createRes)
            return createRes;
    }

    std::map<std::string, std::string>::iterator iter;

    /*
     * Create each pak file.
     */
    for (iter = configLookupTable.begin();
         iter != configLookupTable.end();
         iter++)
    {
        snprintf(tmp, 256, "%s = \"%s\"\n", (*iter).first.c_str(), (*iter).second.c_str());

        sConfig = sConfig + std::string(tmp);
    }

    writeRes = FileSystem_WriteToFile("player.config", sConfig.c_str(), sConfig.length());

    return writeRes;
}


bool hdPlayerConfig::LoadConfigFile()
{
    std::string line;
    void *buffer;
    size_t pos;
    int len;

    configLookupTable.clear();

    // Matchings
    std::smatch matchings;

    // string line pattern: $Key = "$Value"
    std::regex stringLinePattern("^\\s*([[:alnum:]]+)\\s*[=]\\s*[\"]([[:alnum:].,]+)[\"]\\s*$");

    if (false == FileSystem_FileExists("player.config"))
    {
        hdPrintf("[hdPlayerConfig::LoadConfigFile] player.config does not exist.\n");
        return false;
    }

    if (0 != FileSystem_ReadFromFile(&buffer, &len, "player.config"))
    {
        return false;
    }

    std::istringstream iss(std::ios::in | std::ios::out);
    iss.rdbuf()->sputn((const char*)buffer, len);
    free(buffer);

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
            
            configLookupTable[matchings[1]] = s;
        }
    }
    
    return true;
}
