/*
 *  hdBody.h
 *  hdGameEngine
 *
 *  Created by david on 16/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef _HD_BODY_H_
#define _HD_BODY_H_

#include "hdMath.h"
#include "hdGeometry.h"

// The new and improved 2D game object.
//
// Ideas:
// - Concrete objects can inherit from hdBody to get all properties. Can not be done with
//   hdGameObject
// - hdBody can contain any number of objects.
// - hdBody is not meant to be drawn.
// - Defines a frame of reference for the object(s) it contains.
// - Wrapper around vertex containers.
// - hdBody is not used for collision detection, rays or anything
//   like that. It is simply a reference frame for child objects.
// - hdBodies can be created directly with new()



class hdBody
{
public:
	hdBody();
	
	const hdAABB& GetAABB() const;
	
	//void SetAABB(const hdAABB& aabb);
	
//	int AddGameObject(const hdGameObject* object);
	
	
	
private:
	hdAABB m_aabb;
};



#endif