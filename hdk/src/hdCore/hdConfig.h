/*
 *  hdConfig.h
 *  TotemGame
 *
 *  Created by david on 26/09/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_CONFIG_H
#define HD_CONFIG_H

#include <sstream>
#include <boost/regex.hpp>

#include "hdFileSystem.h"

#define CONFIG_PHYSICSITERATIONSCOUNT_KEY "PhysicsIterationsCount"
#define CONFIG_FIXEDPHYSICSINTERVAL_KEY "FixedPhysicsInterval"
#define CONFIG_MAXTEXTUREBPP_KEY "MaxTextureBPP"
#define CONFIG_MINFRAMEDURATION_KEY "MinFrameDuration"
#define CONFIG_PHYSICSFIXEDFRAMEDURATION_KEY "PhysicsFixedFrameDuration"
#define CONFIG_TEXTUREQUALITYLEVEL_KEY "TextureQualityLevel"
#define CONFIG_EFFECTSQUALITYLEVEL_KEY "EffectsQualityLevel"
#define CONFIG_LIGHTINGLEVEL_KEY "LightingQualityLevel"

class hdConfig
{
public:
	static void LoadConfigFile(const char *worldName);
    
    static void LoadNullConfig();
	
	static std::string GetValueForKey(const char *key);
	
	static bool ConfigWasInitialized();
	
};

#endif