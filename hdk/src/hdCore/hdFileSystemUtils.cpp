/*
 *  hdFileSystemUtils.cpp
 *  hdk
 *
 *  Created by David Petrie on 10/04/10.
 *  Copyright 2010 Hackdirt Ltd. All rights reserved.
 *
 */

#include "hdFileSystemUtils.h"

//#ifdef LEVEL_EDITOR
#include <list>
list<string> hdGetFileNames(const char *pathBase, const char *subDir)
{
	list<string> filenames;
	//char** filenames = (char **)malloc(sizeof(char *) * 1024);
	int count = 0;
	//memset(filenames, 0, sizeof(filenames));
	
	//const char		*pathBase = hdGamedir();
	string strPathBase(pathBase);
	string strSubDir(subDir);
	
	//RecurseDirectory(pathBase, subDir, filenames, &count, 0);
	RecurseDirectory(strPathBase, strSubDir, &filenames, 0);
	
	hdPrintf("found %d items in list\n", filenames.size());
	
	return filenames;
}


// Add remaining files in the directory.
// If a directory is found, recurse on that one.
//void RecurseDirectory(const char* pathBase, const char* subDir, char** list, int* count, int depth)
//void RecurseDirectory(const char* pathBase, const char* subDir, char** list, int* count, int depth)
void RecurseDirectory(string pathBase, string subDir, list<string> *fileList, int depth)
{
	if (depth > 5) return;
	
	string fpath;
	string filename;
	
	fpath = pathBase + subDir;
	
	fs::path path = fs::path(fpath);
	
	if (!fs::exists(path))
	{
		hdPrintf("Dir %s files not available", fpath.c_str());
		return;
	}
	
	fs::directory_iterator end_iter;
	for (fs::directory_iterator dir_iter(path); dir_iter != end_iter; ++dir_iter)
	{
		try
		{
#warning "This code is broken"
			filename = subDir + "/" + "whatever"; //"std::string(dir_iter->path().filename());
		} catch (exception e)
		{
			return;
		}
		
		if (fs::is_directory(dir_iter->status()))
		{
			RecurseDirectory(pathBase, filename, fileList, ++depth);
		}
		else if (fs::is_regular_file(dir_iter->status()))
		{
			(*fileList).push_back(filename);
		}
	}
}
//#endif


