/*
 *  hdMeshPolygon.h
 *  hdk
 *
 *  Created by David Petrie on 2/07/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 */

#ifndef HD_MESH_POLYGON_H
#define HD_MESH_POLYGON_H

#include "hdMath.h"
#include "hdPolygon.h"

class hdMeshPolygon : public hdPolygon
{
public:
	hdMeshPolygon();
	
	~hdMeshPolygon();
	
	const uint32 GetPolygonType() const;
	
	void SetPolygonType(const uint32 polygonType);
	
protected:
	
	uint32 m_polygonType;
	
private:
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		//ar.template register_type<hdIBImage>();
		ar & boost::serialization::base_object<hdPolygon>(*this);
		ar & m_polygonType;
	}
};


inline const uint32 hdMeshPolygon::GetPolygonType() const
{
	return m_polygonType;
}


inline void hdMeshPolygon::SetPolygonType(const uint32 polygonType)
{
	m_polygonType = polygonType;
}


#endif