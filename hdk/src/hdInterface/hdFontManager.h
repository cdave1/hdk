/*
 *  hdFontManager.h
 *  hdGameEngine
 *
 *  Created by david on 14/05/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
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
	
	hdFont* LoadFluidFontFile(const char *filename); //, unsigned char **pic, unsigned short *width, unsigned short *height, unsigned short *bytes);
	
protected:
	hdFontManager();
	
	~hdFontManager();
	
public:
	
	static hdFontManager* Instance();
	
	static void TearDown();
	
	hdFont* FindFont(const char* name);
	
	hdFont* LoadFont(const char* name, unsigned char* data, unsigned short width, unsigned short height, unsigned short bytes);
	
	hdFont* GetFont(const int location);
	
	hdFont* GetNullFont()
	{
		return m_nullFont;
	}
	
	void DestroyFonts();
};

#endif