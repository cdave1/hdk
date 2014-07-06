/*
 *  hdMathSerializable.h
 *  AnimationEngine
 *
 *  Created by david on 2/04/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 * Non-intrusive serialization for math structs and classes
 */

#ifndef _HD_MATH_SERIALIZABLE_
#define _HD_MATH_SERIALIZABLE_

#include "hdMath/hdMathCommon.h"
typedef float float32;
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

namespace boost {
	namespace serialization {
		
		template<class Archive>
		void serialize(Archive & ar, hdVec2 & vec2, const unsigned int version)
		{
			ar & vec2.x;
			ar & vec2.y;
		}
		
		template<class Archive>
		void serialize(Archive & ar, hdVec3 & vec3, const unsigned int version)
		{
			float32 x, y, z;
			ar & x;
			ar & y;
			ar & z;
			vec3.x = fixed32(x);
			vec3.y = fixed32(y);
			vec3.z = fixed32(z);
		}
    }
}

#endif
