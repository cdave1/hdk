/*
 *  totemGameObject.h
 *  LevelEditor
 *
 *  Created by david on 27/03/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _H_TOTEM_GAME_OBJECT_
#define _H_TOTEM_GAME_OBJECT_

#include "hdGameEngine.h"
#include "hdGameWorld.h"
#include "totemEnums.h"
#include "hdFixed.h"

//#include "hdFixed.h"

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

class hdGameObject;
class hdGameWorld;
struct hdGameObjectDef;


// On load: loads block definitions and then creates them
// On save: generates a list of block definitions, saves.
class totemGameObject
{
public:
	friend class hdGameWorld;
	
	friend class hdGameObject;
	
	totemGameObject();
	
	totemGameObject(hdGameWorld *gameWorld,
					const hdVec2& aa, const hdVec2& bb, 
					const int name);
	
	~totemGameObject();
	
	void Init(hdGameWorld *gameWorld, const hdGameObjectDef def);
	
	void DestroyGameObject();
	
	const hdGameObjectDef GetDefinition();
	
	const hdGameObject* GetGameObject() const;
	
	hdGameObjectDef GetCurrentDefinition() const;
		
protected:
	hdGameObject* m_gameObject;
	
	hdGameWorld* m_gameWorld;
	
	hdGameObjectDef m_def;
	
	
	friend class boost::serialization::access;
	
	// Loading and saving
	//
	// loading must retrieve the def and then initialize
	// the block via the def.
	template<class Archive>
	void save(Archive & ar, const unsigned int version) const
	{
		hdGameObjectDef def = this->GetCurrentDefinition();
		
		ar & def.type;
		ar & def.hidden;
		ar & def.name;
		ar & def.userType;
		
		ar & def.position.x;
		ar & def.position.y;
		ar & def.position.z;
		ar & def.boundingBox.x;
		ar & def.boundingBox.y;
		ar & def.boundingBox.z;
		ar & def.angle.x;
		ar & def.angle.y;
		ar & def.angle.z;
	}
	
	
	// Loading and saving
	//
	// loading must retrieve the def and then initialize
	// the block via the def.
	template<class Archive>
	void load(Archive & ar, const unsigned int version)
	{
		hdGameObjectDef def;
		
		ar & def.type;
		ar & def.hidden;
		ar & def.name;
		ar & def.userType;
		
		float32 x, y, z;
		ar & x;
		ar & y;
		ar & z;
		def.position.x = fixed32(x);
		def.position.y = fixed32(y);
		def.position.z = fixed32(z);
		
		ar & x;
		ar & y;
		ar & z;
		def.boundingBox.x = fixed32(x);
		def.boundingBox.y = fixed32(y);
		def.boundingBox.z = fixed32(z);
		
		ar & x;
		ar & y;
		ar & z;
		def.angle.x = fixed32(x);
		def.angle.y = fixed32(y);
		def.angle.z = fixed32(z);
		
		m_def = def;
	}
	BOOST_SERIALIZATION_SPLIT_MEMBER()
	
	
	
};


inline const hdGameObjectDef totemGameObject::GetDefinition()
{
	return m_def;
}

inline const hdGameObject* totemGameObject::GetGameObject() const
{
	return m_gameObject;
}

#endif