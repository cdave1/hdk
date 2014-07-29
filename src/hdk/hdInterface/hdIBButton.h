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

#ifndef _HD_IB_BUTTON_H_
#define _HD_IB_BUTTON_H_

#include <hdk/hdInterface/hdIBProxy.h>

class hdIBButton : public hdIBProxy
{
public:
    hdIBButton();

    hdIBButton(const char *textureNormal,
               const char *textureOver,
               const char *textureUp,
               const hdVec3 &lower,
               const hdVec3 &upper);

    ~hdIBButton();

    hdInterfaceBuilderProxyType_t GetInterfaceBuilderProxyType();

    hdReceiver * CreateInterfaceElement();

    void Draw() const;

    char * GetNormalTexture();

    void SetNormalTexture(const char *texture);

    char * GetOverTexture();

    void SetOverTexture(const char *texture);

    char * GetUpTexture();

    void SetUpTexture(const char *texture);


private:
    char m_textureNormal[kMaxTexturePathSize];
    char m_textureOver[kMaxTexturePathSize];
    char m_textureUp[kMaxTexturePathSize];

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<hdIBProxy>(*this);
        ar & m_textureNormal;
        ar & m_textureOver;
        ar & m_textureUp;
    }
};


inline hdInterfaceBuilderProxyType_t hdIBButton::GetInterfaceBuilderProxyType()
{
    return hdInterfaceBuilderProxyTypeButton;
}


inline char * hdIBButton::GetNormalTexture()
{
    return m_textureNormal;
}


inline void hdIBButton::SetNormalTexture(const char *texture)
{
    snprintf(m_textureNormal, kMaxTexturePathSize, "%s", texture);
}


inline char * hdIBButton::GetOverTexture()
{
    return m_textureOver;
}


inline void hdIBButton::SetOverTexture(const char *texture)
{
    snprintf(m_textureOver, kMaxTexturePathSize, "%s", texture);
}


inline char * hdIBButton::GetUpTexture()
{
    return m_textureUp;
}


inline void hdIBButton::SetUpTexture(const char *texture)
{
    snprintf(m_textureUp, kMaxTexturePathSize, "%s", texture);
}

#endif