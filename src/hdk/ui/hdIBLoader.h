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

#ifndef _HD_IB_LOADER_H_
#define _HD_IB_LOADER_H_

#include <hdk/ui/hdIBViewController.h>

#include <hdk/core/hdCompression.h>

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
