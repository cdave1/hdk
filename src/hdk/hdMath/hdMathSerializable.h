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
 * Non-intrusive serialization for math structs and classes
 */

#ifndef _HD_MATH_SERIALIZABLE_
#define _HD_MATH_SERIALIZABLE_

#include <hdk/hdMath/hdMathCommon.h>

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
