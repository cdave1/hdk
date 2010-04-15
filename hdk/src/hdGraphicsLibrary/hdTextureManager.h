/*
 *  hdTextureManager.h
 *  AnimationEngine
 *
 *  Created by david on 9/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_TEXTURE_MANAGER_H_
#define _HD_TEXTURE_MANAGER_H_

#include "hdMath.h"
#include "hdGraphicsLibrary.h"
#include "hdCollections.h"
#include "hdCore/hdConfig.h"

#ifndef LEVEL_EDITOR
//#include "GraphicsDevice.h"
#endif

#ifdef WINDOWS
#include "hdDDSLoader.h"
#endif


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