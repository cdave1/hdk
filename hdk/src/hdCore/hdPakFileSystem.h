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

#ifndef HD_PAK_MAKER_H
#define HD_PAK_MAKER_H

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
    std::string m_pakFileName;
	
    std::string m_directoryName;
	
	bool m_isCompressed;
	
	std::map<std::string, hdPakFileEntry*> m_files;
	
	
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
	std::map<std::string, std::string> m_fileLookupTable;
	
	std::map<std::string, hdPakFile*> m_pakFiles;
	
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
