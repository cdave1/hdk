/*
 *  hdIBButton.h
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#ifndef _HD_IB_BUTTON_H_
#define _HD_IB_BUTTON_H_

#include "hdIBProxy.h"

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