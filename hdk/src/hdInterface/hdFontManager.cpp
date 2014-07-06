/*
 *  hdFontManager.cpp
 *  hdGameEngine
 *
 *  Created by david on 14/05/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdFontManager.h"



static hdFontManager* m_FontManagerInstance = NULL;

hdFontManager* hdFontManager::Instance()
{
	if (m_FontManagerInstance == NULL)
	{
		m_FontManagerInstance = new hdFontManager();
	}
	return m_FontManagerInstance;
}


void hdFontManager::TearDown()
{	
	delete m_FontManagerInstance;
	m_FontManagerInstance = NULL;
}


hdFontManager::hdFontManager()
{
	m_fonts = new hdPointerList<hdFont, kMaxFonts>();
	m_nullFont = NULL;
}


hdFontManager::~hdFontManager()
{
	hdFont *f;
	while(!m_fonts->IsEmpty())
	{
		f = m_fonts->GetItems()[0];
		m_fonts->RemoveAtIndex(0);
	}
	delete m_fonts;
}


hdFont* hdFontManager::GetFont(const int location)
{
	if (location >= m_fonts->GetItemCount()) return NULL;
	return (hdFont *)m_fonts->GetItems()[location];
}


hdFont* hdFontManager::FindFont(const char* name)
{
	//hdFont	*tex;
	
	for (int i = 0; i < m_fonts->GetItemCount(); ++i)
	{
		if (strncmp(m_fonts->GetItems()[i]->name, name, kMaxFilePathSize) == 0)
		{
			return (hdFont*)m_fonts->GetItems()[i];
		}
	}
	 
	return LoadFluidFontFile(name);
}


/*
 * .f files have a specific format. Characters are stored in a contiguous format.
 * We load the char data into a data structure, and then
 * prepare a buffer containing a map of the character pixels.
 *
 * NOTE: Character pixels are alpha values.
 */
hdFont* hdFontManager::LoadFluidFontFile(const char *filename)
{
	// Texture loader variables.
	int offX, offY;
	float widthScale, heightScale;  
	float left, right, top, bottom;
	int texIndex, charIndex;
	
	hdFont	*font = NULL;

	GLubyte *textureData = NULL;
	
	int texX = 0;  
	int texY = 0; 
	int padding = 2;
	
	int maxWidth = 0;
	int maxHeight = 0;
	int requiredPixelArea = 0;
	float pixelAreaRoot = 0;
	unsigned short texSquare = 0;
	
	filehandle_t *hFile;
	
	hdPrintf("Attempting to load font file: %s\n", filename);
	
	hFile = FileSystem_OpenFile( filename, 0 );
	if( ! hFile )
    {
		hdPrintf( "LoadFluidFontFile: Could not open (%s) for reading\n", filename );
		goto FONTFILELOADFAILED;
    }
	
	font = new hdFont();
	snprintf(font->name, kMaxFilePathSize, "%s", filename);
	m_fonts->Add(font);
	
	for (int i = 0; i < kFluidFontFileCharCount; ++i)
	{
		// Read in the char info
		font->fileChars[i].charId = i;
		
		FileSystem_ReadFile(&font->fileChars[i].byteWidth,	sizeof(signed int), 1, hFile);
		FileSystem_ReadFile(&font->fileChars[i].byteHeight, sizeof(signed int), 1, hFile);
		FileSystem_ReadFile(&font->fileChars[i].xOffset, sizeof(signed int), 1, hFile);
		FileSystem_ReadFile(&font->fileChars[i].yOffset, sizeof(signed int), 1, hFile);
		FileSystem_ReadFile(&font->fileChars[i].screenWidth, sizeof(signed int), 1, hFile);
		FileSystem_ReadFile(&font->fileChars[i].screenHeight, sizeof(signed int), 1, hFile);
		
		font->fileChars[i].pixelArea = (font->fileChars[i].byteWidth * font->fileChars[i].byteHeight);
		
		if (font->fileChars[i].pixelArea > (256*256))
		{
			hdPrintf( "Font character size was too large - assuming corrupted file and bailing out.\n");
			goto FONTFILELOADFAILED;
		}
		if (font->fileChars[i].pixelArea > 0)
		{
			font->fileChars[i].buffer = new unsigned char[(font->fileChars[i].pixelArea)];
			FileSystem_ReadFile(font->fileChars[i].buffer, sizeof(unsigned char), font->fileChars[i].pixelArea, hFile);
			
			// ASCII printable characters only.
			if (i >= FIRST_PRINTABLE_ASCII_INDEX && i <= LAST_PRINTABLE_ASCII_INDEX)
			{
				maxWidth = hdMax(maxWidth, font->fileChars[i].screenWidth);
				maxHeight = hdMax(maxHeight, font->fileChars[i].screenHeight);
				
#ifdef DEBUG_FONTS
				hdPrintf("\tLoading Character %c: screenwidth: %d, screenheight: %d, bytewidth: %d, byteheight: %d, ",
						 font->fileChars[i].charId, font->fileChars[i].screenWidth, font->fileChars[i].screenHeight,
						 font->fileChars[i].byteWidth, font->fileChars[i].byteHeight);
				hdPrintf("x-offset: %d, y-offset: %d\n",
						 font->fileChars[i].xOffset, font->fileChars[i].yOffset);
#endif
				
			}
			requiredPixelArea += (font->fileChars[i].screenWidth * font->fileChars[i].screenHeight * 1.7f); // .pixelArea;
		}
	}
	
	font->m_lineHeight = maxHeight;
	
	// HACK HACK HACK
	// We're using fixed height glyphs. This means we need enough 
	// y-padding in the font texture so we don't draw artifacts from other glyphs.
	// This is not a very intuitive solution though - should just use a yPadding
	// variable.
	maxHeight  *= 1.7;
	
	pixelAreaRoot = sqrtf(requiredPixelArea);
	
	if (pixelAreaRoot > (1<<10))
	{
		hdPrintf("Font characters were too large to fit on a single texture\n");
		goto FONTFILELOADFAILED;		
	}
	
	// We want enough room in the texture map to fit all of our pixels.
	for( texSquare = 1 ; texSquare < pixelAreaRoot ; texSquare <<= 1 );
	
	textureData = new GLubyte[texSquare * texSquare];
	if (textureData == NULL)
	{
		hdPrintf("MALLOC ERROR\n");
		goto FONTFILELOADFAILED;
	}
	
	memset(textureData, 0, texSquare * texSquare);
	
	for (int i = 0; i < kFluidFontFileCharCount; ++i)  
	{
		// ASCII printable characters only.
		if (i >= FIRST_PRINTABLE_ASCII_INDEX && i <= LAST_PRINTABLE_ASCII_INDEX)
		{
			if (texX + padding + font->fileChars[i].screenWidth >= texSquare)
			{
				texX = 0;  
				texY += maxHeight;
			} 
			
			offX = texX + (padding / 2);  
			offY = texSquare - 1 - (texY + (padding / 2));
			
			// Calculate texture coordinates for this character  
			widthScale = 1.0f / (float)(texSquare);  
			heightScale = 1.0f / (float)(texSquare);  
			
			left = (float)(offX - font->fileChars[i].xOffset) * widthScale;
			right = left + ((float)font->fileChars[i].screenWidth * widthScale);
			
			top = (float)(offY + (font->fileChars[i].screenHeight+ font->fileChars[i].yOffset)) * heightScale;
			font->fileChars[i].screenHeight = font->fileChars[i].screenHeight - font->fileChars[i].yOffset;
			bottom = top - ((float)font->fileChars[i].screenHeight * heightScale);
			
			font->fileChars[i].texCoords[0].Set(left, bottom, 0.0f);
			font->fileChars[i].texCoords[1].Set(left, top, 0.0f);  
			font->fileChars[i].texCoords[2].Set(right, bottom, 0.0f);
			font->fileChars[i].texCoords[3].Set(right, top, 0.0f);
			
			// Store the pixel data in the texture.
			// Should investigate using a single channel texture  
			for (int x = 0; x < font->fileChars[i].byteWidth; x++)  
			{  
				for (int y = font->fileChars[i].byteHeight - 1; y >= 0 ; y--)  
				{  
					texIndex = ((offY - y) * texSquare) + (offX + x);  
					charIndex = (y * font->fileChars[i].byteWidth) + x;
					hdAssert(texIndex >= 0 && texIndex < (texSquare * texSquare));
					textureData[texIndex] = font->fileChars[i].buffer[charIndex];
				}  
			}
			
			texX += (padding + font->fileChars[i].screenWidth);
		}
		
		// We created buffers for _all_ fluid font characters earlier.
		// TODO: this is a potential source of bugs, so possibly move the 
		// management of the buffers somewhere else (to the class, for instance).
		if (font->fileChars[i].pixelArea > 0)
		{
			delete [] font->fileChars[i].buffer;
			font->fileChars[i].buffer = NULL;
		}
	}
	
	// Use font manager to load the font info as ALPHA data.
	font->m_texture = hdTextureManager::Instance()->LoadTexture(font->name, textureData, texSquare, texSquare, TT_Font, 1);
	
	delete [] textureData;
	
	// HACK HACK HACK:
	// Space character does not appear to have either a width or height - just need a width.
	font->fileChars[(int)' '].screenWidth = maxHeight / 5;
	FileSystem_CloseFile(hFile);
	return font;
	
FONTFILELOADFAILED:
	if (font != NULL)
	{
		m_fonts->Remove(font);
	}
	
	if (hFile)
	{
		FileSystem_CloseFile(hFile);
	}
	return NULL;
}



hdFont* hdFontManager::LoadFont(const char* name, unsigned char* data, unsigned short width, unsigned short height, unsigned short bytes)
{
	/*
	hdglError("Load Texture unknown source");
	
	
	
	tex->width = width;
	tex->height = height;
	tex->type = type;
	tex->bytes = bytes;
	snprintf(tex->name, kMaxTexturePathSize, name);
	//	tex->name = name;
	
	switch( type )
	{
		case TT_Pic: 
			tex->MipMap = false;	
			tex->WrapS = Clamp;
			tex->WrapT = Clamp;
			tex->MinFilter = NearestMipMapOff;
			tex->MagFilter = Nearest;
			break;
			
		case TT_Wall:
			tex->MipMap = true;	
			tex->WrapS = Repeat;
			tex->WrapT = Repeat;
			//			tex->MinFilter = LinearMipMapLinear;
			tex->MinFilter = LinearMipMapNearest;
			tex->MagFilter = Linear;
			break;
			
		default:
			tex->WrapS = Repeat;
			tex->WrapT = Repeat;
			tex->MinFilter = NearestMipMapOff;
			tex->MagFilter = Nearest;
			break;
	}
	
	
	for( scaled_width = 1 ; scaled_width < tex->width ; scaled_width <<= 1 )
	{
		;
	}
	
	if(scaled_width > tex->width && tex->MipMap ) 
	{
		scaled_width >>= 1;
	}
	
	for( scaled_height = 1 ; scaled_height < tex->height ; scaled_height <<= 1 )
	{
		;
	}
	
	if(scaled_height > tex->height && tex->MipMap )
	{
		scaled_height >>= 1;
	}
	
	// let people sample down the world textures for speed
	if( tex->MipMap )
	{
		//scaled_width >>= (int)gl_picmip->value;
		//scaled_height >>= (int)gl_picmip->value;
	}
	
	// don't ever bother with > glMaxTexSize textures
	if( scaled_width > hdglMaxTexSize )
	{
		scaled_width = hdglMaxTexSize;
	}
	if( scaled_height > hdglMaxTexSize )
	{
		scaled_height = hdglMaxTexSize;
	}
	
	if( scaled_width < 1 )
	{
		scaled_width = 1;
	}
	if( scaled_height < 1 )
	{
		scaled_height = 1;
	}
	
	tex->upload_width = scaled_width;
	tex->upload_height = scaled_height;
	
	if( scaled_width == tex->width && scaled_height == tex->height )
	{
		scaled = data;
	}
	else
	{
		scaled = (unsigned char*)malloc( scaled_width * scaled_height * tex->bytes );
		//TM_ResampleFont( data, tex->width, tex->height, scaled, scaled_width, scaled_height, tex->bytes, INTERPOLATION_NONE );
	}
	
	{
		// upload base image	
		GLenum internalFormat[] = { GL_LUMINANCE, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_RGB, GL_RGBA };
#if 0		
		GLenum externalFormat[] = { GL_LUMINANCE, GL_LUMINANCE, GL_LUMINANCE_ALPHA, GL_UNSIGNED_SHORT_5_6_5,  GL_UNSIGNED_SHORT_5_5_5_1 };
		glTexImage2D( GL_TEXTURE_2D, 0, internalFormat[ tex->bytes ], scaled_width, scaled_height, 0, tex->bytes == 4 ? GL_RGBA : GL_RGB, externalFormat[ tex->bytes ], scaled );
#else		
		glTexImage2D( GL_TEXTURE_2D, 0, internalFormat[ tex->bytes ], scaled_width, scaled_height, 0, tex->bytes == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, scaled );
#endif
		
		
		// upload mipmaps if required
#ifndef LEVEL_EDITOR
		glGenerateMipmapOES( GL_TEXTURE_2D );
#else		
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		//if( tex->MipMap )
		//{
		//	int miplevel = 0;
		
		//	while( TM_MipMap( scaled, &scaled_width, &scaled_height, tex->bytes ) )
		//	{
		//		glTexImage2D( GL_TEXTURE_2D, ++miplevel, internalFormat[ tex->bytes ], scaled_width, scaled_height, 0, tex->bytes == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, scaled );
		//	}
		//}
#endif		
	}
	
	if ( scaled != data ) {
		free( scaled );
	}
	
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, WrapToGL( tex->WrapS ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, WrapToGL( tex->WrapT ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilterToGL( tex->MipMap, tex->MinFilter ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilterToGL( tex->MagFilter ) );		
	
#ifndef LEVEL_EDITOR
	if ( type == TT_Wall ) {
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 2.0f );
	} else {
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 0 );
	}
#endif
	
	hdPrintf("Loading texture: %s\n", name);
	hdglError(name);
	
	return tex;*/
	return NULL;
}