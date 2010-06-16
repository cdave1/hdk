/*
 *  hdIBLoader.h
 *  hdkOSX
 *
 *  Created by David Petrie on 13/06/10.
 *  Copyright 2010 n/a. All rights reserved.
 *
 * Loader for ".hib" files.
 *
 */

#ifndef _HD_IB_LOADER_H_
#define _HD_IB_LOADER_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "hdIBViewController.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>

#include <boost/serialization/list.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>

#include "hdCore/hdCompression.h"

#define HD_VIEWCONTROLLER_FILE_EXTENSION "hib"

class hdIBLoader
{
public:
	
	static hdIBLoader* Instance();
	
	static void TearDown();
	
	hdIBViewController * FindViewController(const char* filename);
	
	bool BackupViewControllerFile(const char *path);
	
	bool SaveViewController(const hdIBViewController *vc, const char *path);
	
	int SaveViewControllerBinary(const hdIBViewController *vc, const char *path);
	
	hdIBViewController * LoadViewControllerFromText(const char* filename);
	
	hdIBViewController * LoadViewControllerFromTextPakFile(const char* filename);
	
private:
	hdIBViewController * LoadViewControllerFromTextInternal(const char* filename, unsigned int FlagsAndAttributes);
	
protected:
	hdIBLoader();
	
	~hdIBLoader();
	
	
private:
	hdIBViewController * LoadViewController(const char* filename);
	
	hdIBViewController * LoadViewControllerFromUncompressedBinary(const char *bin, unsigned long binLength);
};

#endif