/*
 *  hdFileSystemUtils.h
 *  hdk
 *
 *  Created by David Petrie on 10/04/10.
 *  Copyright 2010 Hackdirt Ltd. All rights reserved.
 *
 */

#ifndef _HD_FILESYSTEM_UTILS_H_
#define _HD_FILESYSTEM_UTILS_H_

//#ifdef LEVEL_EDITOR
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include "boost/filesystem.hpp"
#include <iostream>
#include <list>
namespace fs = boost::filesystem;
using namespace std;


#ifdef __cplusplus
extern "C" {
#endif
	
	

	list<string> hdGetFileNames(const char *pathBase, const char* subDir);
	
	void RecurseDirectory(string pathBase, string subDir, list<string> *fileList, int depth);
	
	//void RecurseDirectory(const char* pathBase, const char* subDir, char** list, int* count, int depth);

	
	
	
#ifdef __cplusplus
}
#endif


//#endif

#endif