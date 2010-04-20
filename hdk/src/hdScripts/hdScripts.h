/*
 *  hdScript.h
 *  TotemGame
 *
 *  Created by david on 13/08/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_SCRIPTS_H
#define HD_SCRIPTS_H

using namespace std;
#include <fstream>
#include <sstream>

#include <boost/regex.hpp>

#include "hdCommon.h"
#include "hdMath.h"
#include "hdCollections.h"

#define HD_SCRIPTS_WILDCARD_TAG 9999

typedef enum
{
	e_hdMessageTypeText = 0,
	e_hdMessageTypeImage,
	e_hdMessageTypeCustomImageText,
	e_hdMessageTypeAvatar,
	e_hdMessageTypeTitle,
	e_hdMessageTypeCount,
} e_hdMessageType;


typedef struct 
{
	int contextId;
	int targetId;
	char message[256];
	char texture[256];
	e_hdMessageType messageType;
} hdMessage;


/*
#define kTotemGoalDescriptionStringSize 64
#define kTotemGoalProgressStringSize 256
typedef struct
{
	int levelId;
	int value;
	int state;
	e_hdGoalType goalType;
	char description[kTotemGoalDescriptionStringSize];
	char progressMessage[kTotemGoalProgressStringSize];
} hdGoal;*/


extern void Scripts_Init();


extern void Scripts_TearDown();


extern void Scripts_LoadStringsFile(const char *worldName);


extern string Scripts_GetStringForKey(const char *key);


extern void Scripts_LoadScript(const char *worldName, int worldId);


extern int Scripts_CountMessagesForTag(const int& context, 
											const int& target);

extern int Scripts_CountContextMessagesForTag(const int& context, 
											  const int& target);

extern int Scripts_GetLevelMessagesForTag(hdMessage** messages, 
										  const int& messageCountMax, 
										  const int& context, 
										  const int& target);


void ParseWorldMessageScript(const char* worldName);


#endif