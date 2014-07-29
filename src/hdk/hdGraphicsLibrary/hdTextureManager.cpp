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

#include "hdTextureManager.h"

#include <hdk/hdMath.h>
#include <hdk/hdCore/hdConfig.h>

#include "hdPNGLoader.h"
#include "hdTGALoader.h"
#ifdef WINDOWS
#include "hdDDSLoader.h"
#endif

static hdTextureManager* m_TextureManagerInstance = NULL;
static int maxTextureBPP;
static std::string m_textureQualityLevel;

hdTextureManager* hdTextureManager::Instance()
{
    if (m_TextureManagerInstance == NULL)
    {
        m_TextureManagerInstance = new hdTextureManager();
    }
    return m_TextureManagerInstance;
}


void hdTextureManager::TearDown()
{
    delete m_TextureManagerInstance;
    m_TextureManagerInstance = NULL;
}


hdTextureManager::hdTextureManager()
{
    m_textures = new hdPointerList<hdTexture, kMaxTextures>();

    unsigned char *ptr;
    unsigned char *data;
    int x, y;

    // create a checkerboard texture
    data = (unsigned char*)calloc(1, 16 * 16 * 4 );
    for( y = 0; y < 16; ++y )
    {
        for( x = 0; x < 16; ++x )
        {
            ptr = &data[ (y * 16 + x) * 4 ];
            if( (y < 8) ^ (x < 8) )
            {
                ptr[ 0 ] = ptr[ 1 ] = ptr[ 2 ] = 0x00;
                ptr[ 3 ] = 0xFF;
            }
            else
            {
                ptr[ 0 ] = ptr[ 1 ] = ptr[ 2 ] = 0xFF;
                ptr[ 3 ] = 0xFF;
            }
        }
    }

    m_nullTexture = this->LoadTexture( "***r_notexture***", data, 16, 16, TT_Pic, 4);
    free(data);

    maxTextureBPP = strtol(hdConfig::GetValueForKey(CONFIG_MAXTEXTUREBPP_KEY).c_str(), NULL, 0);
    maxTextureBPP = hdClamp(maxTextureBPP, 1, 8);

    m_textureQualityLevel = hdConfig::GetValueForKey(CONFIG_TEXTUREQUALITYLEVEL_KEY);

    hdglError("error after attempting to bind null texture");
}


hdTextureManager::~hdTextureManager()
{
    hdTexture *t;

    while(!m_textures->IsEmpty())
    {
        t = m_textures->GetItems()[0];
        glDeleteTextures(1, &t->texnum);
        hdglError("glDeleteTextures");
        m_textures->RemoveAtIndex(0);
    }
    delete m_textures;
}


hdTexture* hdTextureManager::GetTexture(const int location)
{
    if (location >= m_textures->GetItemCount()) return NULL;
    return (hdTexture *)m_textures->GetItems()[location];
}


hdTexture* hdTextureManager::FindTexture(const char* name, texturetype_t type)
{
    hdTexture	*tex;
    unsigned char	*data;
    unsigned short	width, height;
    unsigned short	bytes;

    if (name == NULL) return NULL;
    if (strlen(name) == 0) return NULL;

    for (int i = 0; i < m_textures->GetItemCount(); i++)
    {
        if (strncmp(m_textures->GetItems()[i]->name, name, kMaxTexturePathSize) == 0)
        {
            return (hdTexture*)m_textures->GetItems()[i];
        }
    }

    hdPrintf("[hdTextureManager] Looking for texture file: %s\n", name);

    if (std::string(name).find(std::string(".tga")) != std::string::npos)
    {

        LoadTGA( name, &data, &width, &height, &bytes );
        if ( data ) {
            tex = this->LoadTexture( name, data, width, height, type, bytes );

            free( data );
            return tex;
        }
    }

    if (std::string(name).find(std::string(".png")) != std::string::npos)
    {
        LoadPNG( name, &data, &width, &height, &bytes );
        if ( data ) {
            tex = this->LoadTexture( name, data, width, height, type, bytes );
#if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
            tex->isPremultipliedAlpha = true;
#endif
            free( data );
            return tex;
        }
    }

    hdPrintf("[hdTextureManager] ERROR: could not find texture file: %s\n", name);
    return m_nullTexture;
}


hdTexture* hdTextureManager::ReloadTexture(hdTexture *texture, texturetype_t type)
{
    // There's no point resampling if nothing is going to change.
    if (texture->type != type)
    {
        char fname[kMaxTexturePathSize];
        snprintf(fname, kMaxTexturePathSize, "%s", texture->name);
        DestroyTexture(texture);
        return FindTexture(fname, type);
    }
    else
    {
        return texture;
    }
}


#if TARGET_OS_IPHONE == 1 || TARGET_IPHONE_SIMULATOR == 1
// PVRTC textures suck.  Don't load them.
/*
 static hdPVRTCLoader *pvrtcLoader = NULL;
 hdTexture* hdTextureManager::LoadPVRTexture(const char* name)
 {
 hdTexture	*tex;
 PVR_Texture_Header hdr;
 char filename[kMaxTexturePathSize];

 if (pvrtcLoader == NULL)
 {
 pvrtcLoader = new hdPVRTCLoader();
 }

 if (m_textures->IsFull())
 {
 hdError(-1, "Texture container is full.\n");
 return NULL;
 }

 tex = new hdTexture();
 tex->texnum = m_textures->GetItemCount();

 snprintf(filename, kMaxTexturePathSize, "%s/%s", hdGamedir(), name);

 hdglError("Start: LoadTextureFromPVR\n");
 if(!pvrtcLoader->LoadTextureFromPVR(filename, &(tex->texnum), &hdr))
 {
 hdError(-1, "Failed to load PVR texture.\n");
 return NULL;
 }
 hdglError("End: LoadTextureFromPVR");

 tex->width = hdr.dwWidth;
 tex->height = hdr.dwHeight;
 //tex->type = type;
 tex->bytes = hdr.dwTextureDataSize;
 snprintf(tex->name, kMaxTexturePathSize, name);

 return tex;
 }*/
#endif


hdTexture* hdTextureManager::LoadTexture(const char* name, unsigned char* data, unsigned short width, unsigned short height, texturetype_t type, unsigned short bytes)
{
    hdglError("Load Texture unknown source");

    hdTexture	*tex;
    unsigned char *scaled;
    unsigned short scaled_width, scaled_height;
#if !defined(TARGET_GL_OPENGL)
    unsigned char *downSized;
    unsigned short downSizedWidth, downSizedHeight;
    unsigned short* shortScaled;
    bool downSample;
#endif

    tex = new hdTexture();

    m_textures->Add(tex);
    tex->texnum = m_textures->GetItemCount();

    glGenTextures( 1, &tex->texnum );

    glBindTexture( GL_TEXTURE_2D, tex->texnum );

    tex->width = width;
    tex->height = height;
    tex->type = type;
    tex->bytes = bytes;
    tex->isPremultipliedAlpha = false;
    snprintf(tex->name, kMaxTexturePathSize, "%s", name);

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
        case TT_16Wall:
            tex->MipMap = true;
            tex->WrapS = Repeat;
            tex->WrapT = Repeat;
            tex->MinFilter = LinearMipMapNearest;
            tex->MagFilter = Linear;
            break;
        case TT_FastWall:
            tex->MipMap = true;
            tex->WrapS = Repeat;
            tex->WrapT = Repeat;
            tex->MinFilter = LinearMipMapOff;
            tex->MagFilter = Nearest;
            break;
        case TT_Font:
            tex->bpp = 16;
            tex->MipMap = true;
            tex->WrapS = Repeat;
            tex->WrapT = Repeat;
            tex->MinFilter = LinearMipMapOff;
            tex->MagFilter = Linear;
            break;
        case TT_ScaleableFont:
            tex->MipMap = true;
            tex->WrapS = Repeat;
            tex->WrapT = Repeat;
            tex->MinFilter = LinearMipMapLinear;
            tex->MagFilter = Linear;
            break;
        case TT_Background:
            tex->MipMap = true;
            tex->WrapS = Repeat;
            tex->WrapT = Repeat;
            tex->MinFilter = LinearMipMapLinear;
            tex->MagFilter = Linear;
            break;
        case TT_16Background:
            tex->bpp = 16;
            tex->MipMap = true;
            tex->WrapS = Repeat;
            tex->WrapT = Repeat;
            tex->MinFilter = LinearMipMapLinear;
            tex->MagFilter = Linear;
            break;
        case TT_Sprite:
        case TT_16Pic:
            tex->bpp = 16;
            tex->MipMap = false;
            tex->WrapS = Clamp;
            tex->WrapT = Clamp;
            tex->MinFilter = LinearMipMapNearest;
            tex->MagFilter = Linear;
            break;
        default:
            tex->MipMap = true;
            tex->WrapS = Repeat;
            tex->WrapT = Repeat;
            tex->MinFilter = LinearMipMapOff;
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

    GLenum pixelDataType = GL_UNSIGNED_BYTE;

    /*
     TODO: those formats above are stupid - the formats should be contained in the texture
     as an internal value, and then converted when loaded by openGL
     */
    GLenum internalFormat = GL_RGBA;
    GLenum externalFormat = GL_RGBA;

    switch (tex->bytes)
    {
        case 0:
        case 1:
            internalFormat = GL_ALPHA;
            externalFormat = GL_ALPHA;
            break;
        case 2:
            internalFormat = GL_LUMINANCE_ALPHA;
            externalFormat = GL_LUMINANCE_ALPHA;
            break;
        case 3:
            internalFormat = GL_RGB;
            externalFormat = GL_RGB;
            break;
        case 4:
            internalFormat = GL_RGBA;
            externalFormat = GL_RGBA;
            break;
        default:
            internalFormat = GL_RGB;
            externalFormat = GL_RGB;
            break;
    }

    /*
     * Some special texture types need to be downsampled before
     * uploading.
     */
    if( scaled_width == tex->width && scaled_height == tex->height )
    {
#if TARGET_GL_OPENGL
        scaled = data;
        glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, scaled_width, scaled_height, 0, externalFormat, pixelDataType, scaled );
#else

        // TODO: Clean up this shitty code.
        if (4 == maxTextureBPP)
        {
            downSample = false;
        }
        else
        {
            if (m_textureQualityLevel == "Low")
            {
                // Downsample fecking everything except for font textures.
                tex->MinFilter = LinearMipMapNearest;
                tex->MagFilter = Nearest;

                downSample = (type != TT_Font && type != TT_ScaleableFont);
            }
            else if (m_textureQualityLevel == "Medium")
            {
                if (type == TT_16Background || type == TT_Background || type == TT_16Wall || type == TT_Wall)
                {
                    tex->MinFilter = LinearMipMapNearest;
                    tex->MagFilter = Nearest;
                }

                downSample = (type == TT_16Background || type == TT_Background || type == TT_16Wall);
            }
            else
            {
                downSample = (type == TT_16Background || type == TT_Background || type == TT_16Wall);
            }
        }

        if (downSample)
        {
            if (tex->bytes == 3)
            {
                pixelDataType = GL_UNSIGNED_SHORT_5_6_5;
            }
            else if (tex->bytes == 4)
            {
                if (type == TT_16Wall)
                    pixelDataType = GL_UNSIGNED_SHORT_5_5_5_1;
                else
                    pixelDataType = GL_UNSIGNED_SHORT_4_4_4_4;
            }
            else
            {
                hdPrintf("Could not downsample texture: %s\n", name);
                return m_nullTexture ;
            }

            if (m_textureQualityLevel == "Low" ||
                (m_textureQualityLevel == "Medium" &&
                 type == TT_16Wall))
            {
                hdPrintf("Downsizing: %s\n", name);

                downSized = NULL;
                if (-1 == DownSizePixels(downSized, data, tex->bytes, scaled_width, scaled_height, downSizedWidth, downSizedHeight, 0))
                {
                    hdPrintf("Could not downsize texture: %s\n", name);
                    if (downSized != NULL) free(downSized);
                    return m_nullTexture ;
                }

                hdPrintf("Downsampling: %s\n", name);

                shortScaled = new unsigned short[downSizedWidth * downSizedHeight];
                if (-1 == DownSamplePixels(shortScaled, downSized, downSizedWidth,
                                           downSizedHeight, tex->bytes, pixelDataType))
                {
                    hdPrintf("Could not downsample texture: %s\n", name);
                    if (downSized != NULL) free(downSized);
                    delete [] shortScaled;
                    return m_nullTexture ;
                }

                if (tex->MipMap)
                {
                    glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
                }
                else
                {
                    glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
                }
                glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, downSizedWidth,
                             downSizedHeight, 0, externalFormat, pixelDataType, shortScaled );

                hdglError("Texture 16bit conversion");

                free(downSized);
                delete [] shortScaled;

            }
            else
            {
                shortScaled = new unsigned short[scaled_width * scaled_height];
                if (-1 == DownSamplePixels(shortScaled, data, scaled_width,
                                           scaled_height, tex->bytes, pixelDataType))
                {
                    hdPrintf("Could not downsample texture: %s\n", name);
                    delete [] shortScaled;
                    return m_nullTexture ;
                }

                if (tex->MipMap)
                {
                    glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
                }
                else
                {
                    glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
                }
                glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, scaled_width,
                             scaled_height, 0, externalFormat, pixelDataType, shortScaled );

                hdglError("Texture 16bit conversion");

                delete [] shortScaled;
            }
        }
        else
        {
            scaled = data;
            if (tex->MipMap)
            {
                glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
            }
            else
            {
                glTexParameterf(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE); 
            }
            glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, scaled_width, scaled_height, 0, externalFormat, pixelDataType, scaled );
        }	
#endif	
    }
    else
    {
        if (NULL == (scaled = (unsigned char*)calloc(1, scaled_width * scaled_height * tex->bytes )))
        {
            hdPrintf("MALLOC ERROR");
            return NULL;
        }
        glTexImage2D( GL_TEXTURE_2D, 0, internalFormat, scaled_width, scaled_height, 0, externalFormat, pixelDataType, scaled );
        free( scaled );
    }
    
    hdglError("glTexImage2D");
    
#if TARGET_GL_OPENGL == 1
    if (tex->MipMap)
    {
        glGenerateMipmapEXT(GL_TEXTURE_2D);
    }
    
#else
    if (tex->MipMap)
    {
        glGenerateMipmapOES(GL_TEXTURE_2D);
    }
#endif	
    
    hdglError("glGenerateMipmapOES");
    hdPrintf("[hdTextureManager] Finished Loading texture: %s\n\n", tex->name);
    return tex;
}


int hdTextureManager::DownSizePixels(unsigned char*& dest, unsigned char *src, unsigned short srcBPP, 
                                     unsigned short srcWidth, unsigned short srcHeight, 
                                     unsigned short& destWidth, unsigned short& destHeight, 
                                     GLenum format)
{
    unsigned col, row, byte;
    unsigned char *ppOdd, *ppEven;
    unsigned short pixelSum;
    size_t sz;
    
    if (srcWidth <= 1 || srcHeight <= 1)
    {
        hdPrintf("hdTextureManager::DownSizePixels: Can't downsize any further.");
        return -1;		
    }
    
    sz = sizeof(unsigned char);
    
    destWidth = srcWidth >> 1;
    destHeight = srcHeight >> 1;
    
    if (NULL == (dest = (unsigned char*)calloc(1, sz * srcBPP * destWidth * destHeight)))
    {
        hdPrintf("hdTextureManager::DownSizePixels OUT OF MEMORY.");
        return -1;
    }
    
    ppOdd = src;
    ppEven = src + (sz * srcWidth * srcBPP);
    
    row = 0;
    while(row++ < destHeight)
    {
        col = 0;
        while (col++ < destWidth)
        {
            byte = 0;
            
            // Condense a 2x2 pixel square in source into a single pixel on dest. We need to do this
            // for each byte of the pixels, according to the number of bytes per pixel.
            //
            // We don't care too much about the format; this will work whether the bytes are alpha,
            // luminosity, RGB, RGBA, ABGR etc etc.
            while(byte++ < srcBPP)
            {
                pixelSum = (
                            *ppOdd + 
                            *(ppOdd + (sz * srcBPP)) +
                            *ppEven +
                            *(ppEven + (sz * srcBPP))
                            );
                *dest = (unsigned char)(pixelSum >> 2);
                
                dest++;
                ppOdd++;
                ppEven++;
            }
            ppOdd = ppOdd + (sz * srcBPP);
            ppEven = ppEven + (sz * srcBPP);
        }
        ppOdd = ppOdd + (sz * srcWidth * srcBPP);
        ppEven = ppEven + (sz * srcWidth * srcBPP);
    }
    
    dest -= (sz * srcBPP * destWidth * destHeight);
    
    return 0;
}


/*
 * Translate pixels from separate bytes to a single short.
 *
 * We have pixels stored in 4 adjacent bytes, in RGBA configuration.
 *
 * Need to combine these bytes into a single short,
 * preserving the position of each color.
 */
int hdTextureManager::DownSamplePixels(unsigned short *dest, unsigned char *src, 
                                       unsigned short width, unsigned short height, 
                                       unsigned short srcBytes, GLenum destFormat)
{
    int pixels, i, j;
    
    pixels = (width * height);
    j = 0;
    
    if (srcBytes == 4 && destFormat == GL_UNSIGNED_SHORT_4_4_4_4)
    {
        // Format: RRRRGGGGBBBBAAAA
        for (i = 0; i < pixels; ++i)
        {
            j = i << 2;
            dest[i] =  (src[j] & 0xF0);
            dest[i] |= (src[j + 1] >> 4);
            dest[i] =  dest[i] << 8;
            dest[i] |= (src[j + 2] & 0xF0);
            dest[i] |= (src[j + 3] >> 4);
        }
        return 0;
    }
    else if (srcBytes == 4 && destFormat == GL_UNSIGNED_SHORT_5_5_5_1)
    {
        // Format: RRRRRGGGGGBBBBBA
        for (i = 0; i < pixels; ++i)
        {
            j = i << 2;
            dest[i] =  (src[j] >> 3);
            dest[i] =  dest[i] << 5;
            dest[i] |= (src[j + 1] >> 3);
            dest[i] =  dest[i] << 5;
            dest[i] |= (src[j + 2] >> 3);
            dest[i] =  dest[i] << 1;
            dest[i] |= (src[j + 3] >> 7);
        }
        return 0;
    }
    else if (srcBytes == 3 && destFormat == GL_UNSIGNED_SHORT_5_6_5)
    {
        // Format: RRRRRGGGGGGBBBBB
        for (i = 0; i < pixels; ++i)
        {
            j = i * 3;
            dest[i] =  (src[j] >> 3);
            dest[i] =  dest[i] << 6;
            dest[i] |= (src[j + 1] >> 2);
            dest[i] =  dest[i] << 5;
            dest[i] |= (src[j + 2] >> 3);
        }
        return 0;
    }
    else
    {
        return -1;
    }
}


void hdTextureManager::DestroyTexture(hdTexture *texture)
{
    if (texture != NULL)
    {
        glDeleteTextures(1, &texture->texnum);
        hdglError("glDeleteTextures");
        m_textures->Remove(texture);
        //delete texture;
        //texture = NULL;
    }
}
