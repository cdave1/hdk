/*
 *  hdIBViewController.h
 *  hdkOSX
 *
 *  Created by David Petrie on 12/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

/*
 * The interface builder saves and loads IB View Controllers.
 *
 * Main purpose is to maintain a list of IBProxy elements, and to
 * produce an hdViewController with various interface elements.
 */

#ifndef _HD_IB_VIEW_CONTROLLER_H_
#define _HD_IB_VIEW_CONTROLLER_H_

#include "hdIBProxy.h"
#include "hdIBImage.h"
#include "hdIBButton.h"
#include "hdIBFontPolygon.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#define MAX_VIEW_CONTROLLER_PROXIES 128

class hdIBViewController : public hdDrawable
{
public:
	hdIBViewController() {};
	
	hdIBViewController(const char *name);
	
	hdIBViewController(const char *name, const float width, const float height);
	
	~hdIBViewController();
	
	hdSerializablePointerList<hdIBProxy, MAX_VIEW_CONTROLLER_PROXIES> * GetProxyList();
	
	void CreateInterfaceObjects();
	
	const int AABBQuery(const hdAABB& aabb, hdIBProxy ** objects, int maxObjects);
	
	const float GetWidth() const;
	
	const float GetHeight() const;
	
	void SetDimensions(const float width, const float height);
	
	const char * GetName() const;
	
	void SetName(const char *name);
	
	void Draw() const;
	
private:
	float m_width;
	float m_height;
	char m_name[256];
	hdSerializablePointerList<hdIBProxy, MAX_VIEW_CONTROLLER_PROXIES> * m_proxies;
	
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar.template register_type<hdIBFontPolygon>();
		ar.template register_type<hdIBButton>();
		ar.template register_type<hdIBImage>();
		ar.template register_type<hdIBProxy>();
		ar.template register_type<hdPolygon>();
		ar & m_width;
		ar & m_height;
		ar & m_name;
		ar & m_proxies;
	}
	
};


inline hdSerializablePointerList<hdIBProxy, MAX_VIEW_CONTROLLER_PROXIES> * hdIBViewController::GetProxyList()
{
	return m_proxies;
}


inline const float hdIBViewController::GetWidth() const
{
	return m_width;
}


inline const float hdIBViewController::GetHeight() const
{
	return m_height;
}


inline void hdIBViewController::SetDimensions(const float width, const float height)
{
	m_width = width;
	m_height = height;
}


inline const char * hdIBViewController::GetName() const
{
	return m_name;
}


inline void hdIBViewController::SetName(const char *name)
{
	snprintf(m_name, 256, "%s", name);
}


#endif