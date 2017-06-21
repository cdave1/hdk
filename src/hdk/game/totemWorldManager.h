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

#ifndef _H_HDK_WORLD_MANAGER_
#define _H_HDK_WORLD_MANAGER_

#undef Byte
#include <zlib.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#define RETURN_AS_POINTER 1

#include <hdk/game.h>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

class totemWorld;

/*
 * Cache of binary files - saves us having to go
 * to the fs everytime.
 */
typedef struct
{
    char *filename;
    totemWorld* world;
    unsigned long binLength;
    char *bin;
} totemWorldCache;


class totemWorldManager
{
public:

    static totemWorldManager* Instance();

    static void TearDown();

    void ClearCache();

    void SetCacheBinariesFlag(bool cacheBinaries);

    totemWorld* FindTotemWorld(const char* filename);

#ifdef LEVEL_EDITOR
    bool BackupTotemWorldFile(const char *path);

    bool SaveTotemWorld(const totemWorld *tw, const char *path);

    int SaveTotemWorldBinary(const totemWorld *tw, const char *path);

    totemWorld * LoadTotemWorldFromText(const char* filename);
#endif

    int DestroyTotemWorld(const totemWorld *tw);

protected:
    totemWorldManager();

    ~totemWorldManager();

private:
    totemWorld* LoadTotemWorld(const char* filename);

#if RETURN_AS_POINTER == 1
    totemWorld* LoadTotemWorldFromUncompressedBinary(const char *bin, unsigned long binLength);
#else
    static totemWorld LoadTotemWorldFromUncompressedBinary(const char *bin, unsigned long binLength);
#endif
    
    hdTypedefList<totemWorldCache, 32>* m_cache;
    
    bool m_cacheBinaries;

};

#endif
