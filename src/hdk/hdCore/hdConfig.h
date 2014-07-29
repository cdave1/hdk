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

#ifndef HD_CONFIG_H
#define HD_CONFIG_H

#include <string>

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
