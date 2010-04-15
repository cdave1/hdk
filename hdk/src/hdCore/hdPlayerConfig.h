/*
 *  hdUserConfig.h
 *  TotemGame
 *
 *  Created by david on 19/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_PLAYER_CONFIG_H
#define HD_PLAYER_CONFIG_H

using namespace std;

#include <sstream>

#include <boost/regex.hpp>

#include "hdFileSystem.h"

/*
 * Writeable config file class
 *
 * TODO: Merge with hdConfig 
 */
class hdPlayerConfig
{
public:
	
	static string GetValue(const char *key);
	
	static bool SetValue(const char *key, const char *value);
	

	
	
private:
	friend class AppController;
	
	
	static bool SaveConfigFile();
	
	static bool LoadConfigFile();
	
	
	
};


#endif