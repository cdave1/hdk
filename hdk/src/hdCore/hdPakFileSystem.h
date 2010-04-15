/*
 *  hdPakMaker.h
 *  PakMaker
 *
 *  Created by david on 2/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#ifndef HD_PAK_MAKER_H
#define HD_PAK_MAKER_H
using namespace std;
#include <fstream>

#include <boost/regex.hpp>
#include "hdFileSystem.h"
#include "hdCompression.h"

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/serialization/map.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>




class hdPakFileEntry
{
public:
	long m_uncompressedSize;
	
	long m_compressedSize;
	
	long m_start;
		
	char m_fileName[256];
	
private:
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_uncompressedSize;
		ar & m_compressedSize;
		ar & m_start;
		ar & m_fileName;
	}
};

class hdPakFile
{
public:
	string m_pakFileName;
	
	string m_directoryName;
	
	bool m_isCompressed;
	
	std::map<string, hdPakFileEntry*> m_files;
	
	
private:
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_pakFileName;
		ar & m_directoryName;
		ar & m_isCompressed;
		ar & m_files;
	}
};

class hdPakFileManifest
{
public:
	std::map<string, string> m_fileLookupTable;
	
	std::map<string, hdPakFile*> m_pakFiles;
	
	int m_pakFileCount;
	
	
private:
	friend class boost::serialization::access;
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & m_fileLookupTable;
		ar & m_pakFiles;
		ar & m_pakFileCount;
	}
};


class hdPakFileSystem
{
public:
	static void MakePak(const char* baseDir, const char* pakManifestFilePath, const char *destDir);
	
	static void InitPakFileSystem(const char* baseDir);
	
	static void PrintFileManifest();
	
	static int OpenFile(const char* fileName, char **fileBytes, int *fileLen);

	static filehandle_t *OpenFile(const char* fileName);
private:
	
};


#endif