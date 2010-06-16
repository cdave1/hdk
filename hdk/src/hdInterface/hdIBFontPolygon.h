/*
 *  hdIBFontPolygon.h
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
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