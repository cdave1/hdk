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

#ifndef _HD_IB_FONT_POLYGON_H_
#define _HD_IB_FONT_POLYGON_H_

#include "hdIBProxy.h"

class hdIBFontPolygon : public hdIBProxy
{
public:
    hdIBFontPolygon();

    hdIBFontPolygon(const char *fontFileName,
                    const hdVec3 &lower,
                    const hdVec3 &upper);

    ~hdIBFontPolygon();

    hdInterfaceBuilderProxyType_t GetInterfaceBuilderProxyType();

    hdReceiver * CreateInterfaceElement();

    void Draw() const;

    const char * GetFontFileName() const;

    void SetFontFileName(const char *fontFileName);

private:
    char m_fontFileName[kMaxTexturePathSize];
    char m_text[2048];

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & boost::serialization::base_object<hdIBProxy>(*this);
        ar & m_fontFileName;
        ar & m_text;
    }
};


inline hdInterfaceBuilderProxyType_t hdIBFontPolygon::GetInterfaceBuilderProxyType()
{
    return hdInterfaceBuilderProxyTypeFontPolygon;
}


inline const char * hdIBFontPolygon::GetFontFileName() const
{
    return m_fontFileName;
}


inline void hdIBFontPolygon::SetFontFileName(const char *fontFileName)
{
    snprintf(m_fontFileName, kMaxTexturePathSize, "%s", fontFileName);
}

#endif
