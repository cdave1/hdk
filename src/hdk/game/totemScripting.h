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

#ifndef _HDK_SCRIPTING_H
#define _HDK_SCRIPTING_H

#include <fstream>
#include <sstream>

#include <hdk/hdk.h>

#include "totemEnums.h"

typedef enum
{
    e_totemMessageTypeText = 0,
    e_totemMessageTypeImage,
    e_totemMessageTypeCustomImageText,
    e_totemMessageTypeAvatar,
    e_totemMessageTypeTitle,
    e_totemMessageTypeCount,
} e_totemMessageType;


typedef enum
{
    e_totemGoalTypeNormal = 0,
    e_totemGoalTypeMinBlocksPercentage,
    e_totemGoalTypeMinPoints,
    e_totemGoalTypeFragments,
    e_totemGoalTypeJackCollection,
    e_totemGoalTypeAllBlocksWithTag,
    e_totemGoalTypeMultiplier,
    e_totemGoalTypeCount,
} e_totemGoalType;


typedef struct
{
    int levelId;
    int targetId;
    char message[256];
    char texture[256];
    e_totemMessageType messageType;
} totemMessage;


#define HDK_GAME_GoalDescriptionStringSize 64
#define HDK_GAME_GoalProgressStringSize 256
typedef struct
{
    int levelId;
    int value;
    int state;
    e_totemGoalType goalType;
    char description[HDK_GAME_GoalDescriptionStringSize];
    char progressMessage[HDK_GAME_GoalProgressStringSize];
} totemGoal;


extern void Scripts_Init();

extern void Scripts_TearDown();

extern void Scripts_LoadStringsFile(const char *worldName);

extern std::string Scripts_GetStringForKey(const char *key);

extern void Scripts_LoadScript(const char *worldName, int worldId);

extern int Scripts_CountLevelMessagesForTag(const int& level,
                                            const int& target);

extern int Scripts_GetLevelMessagesForTag(totemMessage** messages,
                                          const int& messageCountMax,
                                          const int& level,
                                          const int& target);

extern int Scripts_GetGoalsForLevel(totemGoal *goals,
                                    const int& goalCountMax,
                                    const int& level);

void Scripts_SetDescriptionForGoal(totemGoal& goal);

void ParseWorldMessageScript(const char* worldName);

#endif
