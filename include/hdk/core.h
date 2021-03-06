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

#ifndef _HD_CORE_H_
#define _HD_CORE_H_

#undef BOOST_HAS_INT128

#include <hdk/core/hdCommon.h>
#include <hdk/core/hdApplication.h>
#include <hdk/core/hdCompression.h>
#include <hdk/core/hdPakFileSystem.h>
#include <hdk/core/hdFileSystem.h>
#include <hdk/core/hdSystemSettings.h>
#include <hdk/core/hdOSFunctions.h>
#include <hdk/core/hdConfig.h>
#include <hdk/core/hdPlayerConfig.h>
#include <hdk/core/hdScreenSettings.h>

#ifdef LEVEL_EDITOR
#include <hdk/core/hdFileSystemUtils.h>
#endif

#include <stdlib.h>
#include <math.h>
#include <algorithm>

#include <hdk/math/hdMath.h>
#include <hdk/math/hdMatrix.h>
#include <hdk/math/hdGeometry.h>
#include <hdk/math/hdMathSerializable.h>

#endif
