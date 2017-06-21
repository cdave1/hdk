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
 
 Each proxy maintains the properties of a receiver of a
 particular type. It is responsible for:
 - creating interface elements and updating properties.
 - Setting the size and position of the element, where
   the proxy aabb is the same as the element's aabb.
 - drawing the particular interface element on the 
   screen of the interface builder.
 - creating interface items when loaded from a HIB file.
 
 */

#ifndef _HD_IB_PROXY_H_
#define _HD_IB_PROXY_H_


#include <hdk/physics.h>
#include <hdk/graphics.h>
#include <hdk/math.h>
#include <hdk/ui/hdIBEnums.h>
#include <hdk/ui/hdReceiver.h>


#define HDIBPROXY_GROUP_STRING_LEN 256
#define HDIBPROXY_TAG_STRING_LEN 256


class hdIBProxy : public hdPolygon, public hdDrawable
{
public:
	hdIBProxy();
	
	~hdIBProxy();
	
	virtual hdReceiver * CreateInterfaceElement();
	
	virtual void Draw() const;
	
	virtual hdInterfaceBuilderProxyType_t GetInterfaceBuilderProxyType();
	
	virtual void MoveDelta(const hdVec3 &delta);
	
	virtual void RotateDelta(const hdVec3& rotationVector, const hdVec3& rotationAxis);
	
	virtual void ScaleDelta(const hdVec3& scaleVector, const hdVec3& scaleAxis);
	
	const char * GetTag() const;
	
	void SetTag(const char *tag);
	
	const char * GetGroup() const;
	
	void SetGroup(const char *group);
	
	void SetZOffset(const float offset);
	
	const float GetZOffset() const;
	
	const hdReceiver * GetInterfaceElement() const;
	
protected:
	
	hdReceiver *m_interfaceElement;
	
	char m_tag[256];
	
	char m_group[256];
	
	float m_zOffset;

private:
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & boost::serialization::base_object<hdPolygon>(*this);
		ar & m_zOffset;
		ar & m_tag;
		ar & m_group;
	}
};


inline const hdReceiver * hdIBProxy::GetInterfaceElement() const
{
	return m_interfaceElement;
}


inline const char * hdIBProxy::GetTag() const
{
	return m_tag;
}


inline void hdIBProxy::SetTag(const char *tag)
{
	snprintf(m_tag, HDIBPROXY_TAG_STRING_LEN, "%s", tag);
}


inline const char * hdIBProxy::GetGroup() const
{
	return m_group;
}


inline void hdIBProxy::SetGroup(const char *group)
{
	snprintf(m_group, HDIBPROXY_GROUP_STRING_LEN, "%s", group);
}


inline const float hdIBProxy::GetZOffset() const
{
	return m_zOffset;
}

#endif
