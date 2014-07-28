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

#ifndef _HD_FILESYSTEM_UTILS_H_
#define _HD_FILESYSTEM_UTILS_H_

#define BOOST_FILESYSTEM_NO_DEPRECATED

#include "boost/filesystem.hpp"
#include <iostream>
#include <list>

#ifdef __cplusplus
extern "C" {
#endif

    std::list<std::string> hdGetFileNames(const char *pathBase, const char* subDir);
	
	void RecurseDirectory(std::string pathBase, std::string subDir,
                          std::list<std::string> *fileList, int depth);

#ifdef __cplusplus
}
#endif

#endif
