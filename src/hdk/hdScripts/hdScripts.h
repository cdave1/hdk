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

#ifndef HD_SCRIPTS_H
#define HD_SCRIPTS_H

#include <hdk/hdk.h>

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


extern void Scripts_Init();

extern void Scripts_TearDown();

extern void Scripts_LoadStringsFile(const char *worldName);

extern std::string Scripts_GetStringForKey(const char *key);

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
