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

#ifndef _HD_IB_IMAGE_H_
#define _HD_IB_IMAGE_H_

#include "hdIBProxy.h"

class hdIBImage : public hdIBProxy
{
public:
    hdIBImage();

    hdIBImage(const char *texture,
              const float x, const float y,
              const float w, const float h);

    hdIBImage(const char *texture,
              const hdVec3 &lower, const hdVec3 &upper);

    ~hdIBImage();

    hdInterfaceBuilderProxyType_t GetInterfaceBuilderProxyType();

    hdReceiver * CreateInterfaceElement();

    void Draw() const;

    char * GetTexture();

    void SetTexture(const char *texture);


private:
    char m_texture[kMaxTexturePathSize];
    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<hdIBProxy>(*this);
        ar & m_texture;
    }
};


inline hdInterfaceBuilderProxyType_t hdIBImage::GetInterfaceBuilderProxyType()
{
    return hdInterfaceBuilderProxyTypeImage;
}


inline char * hdIBImage::GetTexture()
{
    return m_texture;
}


inline void hdIBImage::SetTexture(const char *texture)
{
    snprintf(m_texture, kMaxTexturePathSize, "%s", texture);
}

#endif
