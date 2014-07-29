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

/*
 * Retains a list of loaded fonts - load when required.
 *
 * Loads font info from .f files and stores pixel info
 * as a texture in hdTextureManager.
 *
 * Next it creates an instance of hdFont with mappings
 * to areas on the bitmap for each character.
 *
 * Singleton class - all structures are private.
 */

#ifndef _HD_FONT_MANAGER_
#define _HD_FONT_MANAGER_

#define kMaxFonts 128
#define kMaxFilePathSize 256

#include "hdGraphicsLibrary.h"
#include "hdCore/hdFileSystem.h"
#include "hdInterface/hdFont.h"

class hdFontManager
{
private:

    hdPointerList<hdFont, kMaxFonts>* m_fonts;

    hdFont* m_nullFont;

    hdFont* LoadFluidFontFile(const char *filename);


protected:
    hdFontManager();

    ~hdFontManager();


public:

    static hdFontManager* Instance();

    static void TearDown();

    hdFont* FindFont(const char* name);

    hdFont* LoadFont(const char* name, unsigned char* data,
                     unsigned short width, unsigned short height, unsigned short bytes);
    
    hdFont* GetFont(const int location);
    
    hdFont* GetNullFont()
    {
        return m_nullFont;
    }
    
    void DestroyFonts();
};

#endif
