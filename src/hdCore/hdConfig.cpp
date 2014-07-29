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

#include "hdConfig.h"

static std::map<std::string, std::string> configLookupTable;
static bool configFileWasLoaded = false;


void hdConfig::LoadConfigFile(const char *configFilePath)
{
    std::string line;
    unsigned pos;
    filehandle_t *hnd;

    configLookupTable.clear();

    // Matchings
    boost::smatch matchings;

    // std::string line pattern: $Key = "$Value"
    boost::regex stringLinePattern("^\\s*([[:alnum:]]+)\\s*[=]\\s*[\"]([[:alnum:].,]+)[\"]\\s*$");

    hdPrintf("[hdConfig::LoadConfigFile] Loading config file at: %s\n", configFilePath);

    if (NULL == (hnd = FileSystem_OpenFile(configFilePath, 0)))
    {
        hdPrintf("[hdConfig::LoadConfigFile] Could not load config file at: %s. Loading Defaults\n", configFilePath);

        // Default values are deliberately conservative.
        configLookupTable[std::string(CONFIG_PHYSICSITERATIONSCOUNT_KEY)] = std::string("5");
        configLookupTable[std::string(CONFIG_FIXEDPHYSICSINTERVAL_KEY)] = std::string("0");
        configLookupTable[std::string(CONFIG_MAXTEXTUREBPP_KEY)] = std::string("2");
        configLookupTable[std::string(CONFIG_MINFRAMEDURATION_KEY)] = std::string("0.032");
        configLookupTable[std::string(CONFIG_TEXTUREQUALITYLEVEL_KEY)] = std::string("Medium");
        configLookupTable[std::string(CONFIG_EFFECTSQUALITYLEVEL_KEY)] = std::string("Medium");
        configLookupTable[std::string(CONFIG_LIGHTINGLEVEL_KEY)] = std::string("Medium");

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
            std::string s(matchings[2]);
            while((pos = s.find("\\n")) != std::string::npos)
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


std::string hdConfig::GetValueForKey(const char *key)
{
    std::string sKey(key);
    
    if (configLookupTable.count(sKey) == 0) return std::string(key);
    
    return configLookupTable[sKey];
}


bool hdConfig::ConfigWasInitialized()
{
    return configFileWasLoaded;
}
