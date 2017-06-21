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

#ifndef _HD_TEXTURE_MANAGER_H_
#define _HD_TEXTURE_MANAGER_H_

#include <hdk/hdCollections.h>
#include <hdk/hdGraphicsLibrary/hdGraphicsInterface.h>
#include <hdk/hdGraphicsLibrary/hdTexture.h>

class hdTextureManager
{
private:
    hdPointerList<hdTexture, kMaxTextures>* m_textures;

    hdTexture* m_nullTexture;

    int DownSamplePixels(unsigned short *dest, unsigned char *src,
                         unsigned short width, unsigned short height,
                         unsigned short srcBytes, GLenum destFormat);

    int DownSizePixels(unsigned char*& dest, unsigned char *src, unsigned short srcBPP,
                       unsigned short srcWidth, unsigned short srcHeight,
                       unsigned short& destWidth, unsigned short& destHeight,
                       GLenum format);

protected:
    hdTextureManager();

    ~hdTextureManager();

public:

    static hdTextureManager* Instance();

    static void TearDown();

    hdTexture* FindTexture(const char* name, texturetype_t type);

    hdTexture* ReloadTexture(hdTexture *texture, texturetype_t type);

    hdTexture* LoadTexture(const char* name, unsigned char* data, unsigned short width, unsigned short height, texturetype_t type, unsigned short bytes);

#if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
    //hdTexture* LoadPVRTexture(const char* name);
#endif

    hdTexture* GetTexture(const int location);

    hdTexture* GetNullTexture()
    {
        return m_nullTexture;
    }

    void DestroyTexture(hdTexture *texture);

    void DestroyTextures();
};

#define hdGetTexture16(__hdTextureName__) hdTextureManager::Instance()->FindTexture(__hdTextureName__, TT_16Pic)
#define hdGetTexture(__hdTextureName__) hdTextureManager::Instance()->FindTexture(__hdTextureName__, TT_Pic)

#endif