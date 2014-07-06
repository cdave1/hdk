/*
 *  hdPakMaker.cpp
 *  PakMaker
 *
 *  Created by david on 2/10/09.
 *  Copyright 2009 n/a. All rights reserved.
 *
 */

#include "hdPakFileSystem.h"

void hdPakFileSystem::MakePak(const char* baseDir, const char* pakManifestFilePath, const char *destDir)
{
	char tmp[256];
	void *pakManifestFileBytes;
	int pakManifestFileLen;
	string line;
	
	boost::smatch matchings;
	
	hdPakFileManifest *pakFileManifest;
	hdPakFile *pakFile;
	hdPakFileEntry *pakFileEntry;
	
	int pakFileSize;
	char *pakFileBytes;
	char *uncompressedBytes;
	char *compressedBytes;
	hdPakFileEntry *tmpPakFileEntry;
	
	// first test: zip the bytes pakFileListFile and write to dest.
	if (0 != FileSystem_ReadFromFile(&pakManifestFileBytes, &pakManifestFileLen, pakManifestFilePath))
	{
		hdPrintf("Failed to read file at path: %s\n", pakManifestFilePath);
		return;
	}
	
	pakFileManifest = new hdPakFileManifest();
	pakFileManifest->m_pakFileCount = 0;
	pakFile = NULL;
	pakFileEntry = NULL;

	boost::regex dirLinePattern("^(.+):([Z])*$");
	
	std::istringstream iss((char *)pakManifestFileBytes);
	if (!iss.good())
	{
		hdPrintf("Could not load strings file: %s\n", pakManifestFilePath);
		return;
	}
	
	// Parse line by line
	while(!iss.eof())
	{
		getline(iss, line);
		
		if (boost::regex_match(line, matchings, dirLinePattern))
		{
			hdAssert(line == matchings[0]);
			hdAssert(3 == matchings.size());
			
			pakFile = new hdPakFile();
			pakFileEntry = NULL;
			
			snprintf(tmp, 256, "pak%0d.pak", pakFileManifest->m_pakFiles.size() + 1);
			
			pakFile->m_pakFileName = string(tmp); 
			pakFile->m_directoryName = matchings[1];
			pakFile->m_isCompressed = (matchings[2] == string("Z"));
			
			pakFileManifest->m_pakFiles[pakFile->m_pakFileName] = pakFile;
		}
		else
		{
			if (line.length() > 0)
			{
				// Assume file.
				hdAssert(pakFile != NULL);
				//hdAssert(pakFileEntry == NULL);
				
				pakFileEntry = new hdPakFileEntry();
				snprintf(pakFileEntry->m_fileName, 256, "%s/%s", pakFile->m_directoryName.c_str(), line.c_str());
				
				pakFileEntry->m_uncompressedSize = 0;
				pakFileEntry->m_compressedSize = 0;
				pakFileEntry->m_start = -1;
				
				pakFile->m_files[string(pakFileEntry->m_fileName)] = pakFileEntry;
				
				// Add file to lookup table.
				pakFileManifest->m_fileLookupTable[string(pakFileEntry->m_fileName)] = pakFile->m_pakFileName;
			}
		}
	}
	
	map<string, hdPakFile*>::iterator pakFiles;
	map<string, hdPakFileEntry*>::iterator pakFileContents;
	
	pakFileSize = 0;
	pakFileBytes = NULL;
	uncompressedBytes = NULL;
	compressedBytes = NULL;
	
	/*
	 * Create each pak file.
	 */
	for (pakFiles = pakFileManifest->m_pakFiles.begin(); 
		 pakFiles != pakFileManifest->m_pakFiles.end(); 
		 pakFiles++)
	{
		printf("%s (%s)\n", (*pakFiles).second->m_directoryName.c_str(), (*pakFiles).second->m_pakFileName.c_str());
		
		hdAssert(pakFileBytes == NULL);
		hdAssert(pakFileSize == 0);
		
		for (pakFileContents = (*pakFiles).second->m_files.begin(); 
			 pakFileContents != (*pakFiles).second->m_files.end(); 
			 pakFileContents++)
		{
			uncompressedBytes = NULL;
			compressedBytes = NULL;
			tmpPakFileEntry = (*pakFileContents).second;
			
			printf("\t%s\n", tmpPakFileEntry->m_fileName);
			snprintf(tmp, 256, "%s/%s", baseDir, tmpPakFileEntry->m_fileName);
			
			if (0 != FileSystem_ReadFromFile((void**)&uncompressedBytes, (int*)&(tmpPakFileEntry->m_uncompressedSize), tmp))
			{
				hdPrintf("Failed to read file at path: %s\n", tmp);
			}
			else
			{
				if ((*pakFiles).second->m_isCompressed)
				{
					// zip the bytes
					if (0 != hdCompression_Zip(uncompressedBytes, 
											   tmpPakFileEntry->m_uncompressedSize, 
											   &compressedBytes, 
											   &(tmpPakFileEntry->m_compressedSize)))
					{
						hdPrintf("hdCompression_Zip FAILED %s\n", tmp);
						break;
					}				
				}
				else
				{
					if (NULL == (compressedBytes = (char *)malloc(sizeof(char) * tmpPakFileEntry->m_uncompressedSize)))
					{
						hdPrintf("MALLOC FAILED %s\n", tmp);
						break;
					}	
					memcpy(compressedBytes, uncompressedBytes, tmpPakFileEntry->m_uncompressedSize);
					tmpPakFileEntry->m_compressedSize = tmpPakFileEntry->m_uncompressedSize;
				}
				
				tmpPakFileEntry->m_start = pakFileSize;
				pakFileSize += tmpPakFileEntry->m_compressedSize;
				
				if (pakFileBytes == NULL)
				{
					if (NULL == (pakFileBytes = (char *)malloc(sizeof(char) * pakFileSize)))
					{
						hdPrintf("MALLOC FAILED %s\n", tmp);
						break;
					}
				}
				else
				{
					// reallocate space.
					if (NULL == (pakFileBytes = (char *)realloc(pakFileBytes, sizeof(char) * pakFileSize)))
					{
						hdPrintf("REALLOC FAILED %s\n", tmp);
						break;
					}
				}
				
				// We now have enough space to shove the zipped bytes into the pak file
				memcpy(pakFileBytes + (sizeof(char) * (pakFileSize - tmpPakFileEntry->m_compressedSize)), 
					   compressedBytes, 
					   tmpPakFileEntry->m_compressedSize);
				
				
				
				// Return resources.
				if (uncompressedBytes != NULL) 
					free(uncompressedBytes);
				
				if (compressedBytes != NULL) 
					free(compressedBytes);	
				
				uncompressedBytes = NULL;
				compressedBytes = NULL;
			}			
		} // eof second loop.
		
		// Our pak file should be ready
		if (pakFileSize > 0 && pakFileBytes != NULL)
		{
			// pak file name:
			
			snprintf(tmp, 256, "%s/%s", destDir, (*pakFiles).second->m_pakFileName.c_str());
			
			if (false == FileSystem_FileExists(tmp))
			{
				FileSystem_CreateFile(tmp);
			}
			
			if (0 != FileSystem_WriteToFile(tmp, pakFileBytes, pakFileSize))
			{
				hdPrintf("Failed to write file to path: %s\n", tmp);
			}
			
			hdPrintf("Wrote: %s (%d bytes)\n", tmp, pakFileSize);
			free(pakFileBytes);
		}
		
		pakFileBytes = NULL;
		pakFileSize = 0;
		tmpPakFileEntry = NULL;
		uncompressedBytes = NULL;
		compressedBytes = NULL;
	}
	
	/*
	 * Pak filesystem manifest - always at pak0.pak
	 */
	int createRes, writeRes;
	char *ossBuffer;
	long ossLen;
	char *zipped;
	long zippedLen;
	
	snprintf(tmp, 256, "%s/pak0.pak", destDir);
	
	if (!FileSystem_FileExists(tmp))
	{
		createRes = FileSystem_CreateFile(tmp);
		hdAssert (0 == createRes);
	}
	
	std::ostringstream oss(ios::in | ios::out);
	boost::archive::binary_oarchive boa(oss);
	boa << (* pakFileManifest);
	
	ossLen = oss.str().size() + 1;
	if ((ossBuffer = new char[ossLen]) == NULL)
	{
		printf("Out of memory.\n");
		return;
	}
	
	oss.rdbuf()->sgetn(ossBuffer, ossLen);
	
	hdAssert(0 == hdCompression_Zip(ossBuffer, ossLen,
									&zipped, &zippedLen));
	
	writeRes = FileSystem_WriteToFile(tmp, zipped, zippedLen);
	free(zipped);
	
	delete [] ossBuffer;

}



static hdPakFileManifest *rdnlyPakFS = NULL;
static char pakBaseDir[256];

/*
 * Base pak file manifest assumed to be "pak0.pak" in the
 * given baseDir directory.
 *
 * It is stored as the zipped result of a boost binary
 * serialization of a hdPakFileManifest object.
 *
 * Why a zipped boost binary serialization? Cause we're assholes.
 */
void hdPakFileSystem::InitPakFileSystem(const char* baseDir)
{
	char tmp[256];
	void *zippedManifest, *unzippedManifest;
	long zippedLen, unzippedLen;
	filehandle_t *hnd;
	
	snprintf(pakBaseDir, 256, "%s", baseDir);
	snprintf(tmp, 256, "%s/pak0.pak", baseDir);
	
	/*
	if (!FileSystem_FileExists(tmp))
	{
		hdPrintf("[hdPakFileSystem] Pak file manifest %s does not exist!\n", tmp);
		rdnlyPakFS = NULL;
		return;
	}
	
	if (0 != FileSystem_ReadFromFile(&zippedManifest, (int *)&zippedLen, tmp))  
	{
		hdPrintf("[hdPakFileSystem] Couldn't read from pak file manifest %s\n", tmp);
		rdnlyPakFS = NULL;
		return;
	}*/
	
	if (NULL == (hnd = FileSystem_OpenFile(tmp, HD_FS_ABSOLUTE_PATH)))
	{
		hdPrintf("[hdPakFileSystem] Pak file manifest %s does not exist!\n", tmp);
		rdnlyPakFS = NULL;
		return;		
	}
	
	zippedLen = FileSystem_GetFileSize(hnd);
	if (NULL == (zippedManifest = (char *)malloc(sizeof(char) * zippedLen)))
	{
		hdPrintf("MALLOC FAIL!!!");
		hdAssert(false);
	}
	
	if (-1 == FileSystem_ReadFile(zippedManifest, zippedLen, 1, hnd))  
	{
		hdPrintf("[hdPakFileSystem] Couldn't read from pak file manifest %s\n", tmp);
		rdnlyPakFS = NULL;
		return;
	}
	
	
	if (0 != hdCompression_Unzip((char *)zippedManifest, zippedLen, 
								 (char **)&unzippedManifest, &unzippedLen))
	{
		hdPrintf("[hdPakFileSystem] Couldn't decompress pak file manifest %s\n", tmp);
		rdnlyPakFS = NULL;
		return;
	}
	
	
	rdnlyPakFS = new hdPakFileManifest();
	
	std::istringstream iss(ios::in | ios::out);
	iss.rdbuf()->sputn((const char*)unzippedManifest, unzippedLen);
	boost::archive::binary_iarchive ia(iss);
	ia >> (* rdnlyPakFS);
	
	free(zippedManifest);
	free(unzippedManifest);
	
	FileSystem_CloseFile(hnd);
	
	// TODO: error checks.
}


void hdPakFileSystem::PrintFileManifest()
{
	if (rdnlyPakFS == NULL)
	{
		hdPrintf("No file manifest.\n");
		return;
	}
	
	map<string, string>::iterator lookupIter;
	
	for (lookupIter = rdnlyPakFS->m_fileLookupTable.begin(); 
		 lookupIter != rdnlyPakFS->m_fileLookupTable.end(); 
		 lookupIter++)
	{
		printf("%s (%s)\n", (*lookupIter).first.c_str(), (*lookupIter).second.c_str());
	}
}



int hdPakFileSystem::OpenFile(const char* fileName,  char **fileBytes, int *fileLen)
{
	hdPakFile *pakFile;
	hdPakFileEntry *fileEntry;
	char tmp[256];
	char *compressedBuffer;
	char *uncompressedBuffer;
	long uncompressedSize;
	filehandle_t *pakFileHnd;
	string pakFileName;
	
	*fileBytes = NULL;
	*fileLen = 0;
	compressedBuffer = NULL;
	uncompressedBuffer = NULL;
	pakFileHnd = NULL;
	
	if (rdnlyPakFS == NULL)
	{
		return -1;
	}
	
	/*
	 * File needs to be in the file lookup table, as well as the
	 * specific pak file manifest.
	 */
	if (rdnlyPakFS->m_fileLookupTable.count(string(fileName)) != 1) 
	{
		hdPrintf("[hdPakFileSystem] Couldn't find file %s\n", fileName);
		goto OpenFileFail;		
	}
	pakFileName = rdnlyPakFS->m_fileLookupTable[string(fileName)];
	
	/*
	 * So it's a serious inconsistency if the lookup table contains
	 * the pak file but the pak file manifest doesn't.
	 *
	 * Kill the program if this happens.
	 */
	if (rdnlyPakFS->m_pakFiles.count(pakFileName) != 1)
	{
		hdPrintf("[hdPakFileSystem] INCONSISTENT STATE. KILLING. %s\n", fileName);
		hdAssert(false);
	}
	pakFile = rdnlyPakFS->m_pakFiles[pakFileName];
	
	/*
	 * File will definitely exist if we're this far.
	 */
	hdAssert(pakFile->m_files.size() > 0);
	hdAssert(pakFile->m_files.count(string(fileName)) == 1);
	
	fileEntry = pakFile->m_files[string(fileName)];
	
	hdAssert(strcmp(fileEntry->m_fileName, fileName) == 0);
	
	hdPrintf("Found %s in %s for key %s\n", fileEntry->m_fileName, pakFile->m_pakFileName.c_str(), fileName);
	
	/*
	 * Now we get the bytes out of the pak file.
	 *
	 * Use FileSystem_OpenFile for fastness
	 */
	snprintf(tmp, 256, "%s%s", pakBaseDir, pakFile->m_pakFileName.c_str());
	
	pakFileHnd = FileSystem_OpenFile(tmp, HD_FS_ABSOLUTE_PATH);
	if (!pakFileHnd)
	{
		hdPrintf("[hdPakFileSystem] Couldn't find a physical pak file %s\n", tmp);
		goto OpenFileFail;
	}
	
	if (NULL == (compressedBuffer = (char *)calloc(1, sizeof(char) * fileEntry->m_compressedSize)))
	{
		hdPrintf("[hdPakFileSystem] MALLOC FAIL %s\n", tmp);
		goto OpenFileFail;
	}
	
	if (0 != FileSystem_FileSeek(pakFileHnd, fileEntry->m_start, 0))
	{
		hdPrintf("[hdPakFileSystem] Couldn't seek to position in physical pak file %s\n", tmp);
		goto OpenFileFail;
	}
	
	if (-1 == FileSystem_ReadFile(compressedBuffer, fileEntry->m_compressedSize, 1, pakFileHnd))
	{
		hdPrintf("[hdPakFileSystem] Couldn't read any data in physical pak file %s\n", tmp);
		goto OpenFileFail;
	}
	
	if (pakFile->m_isCompressed)
	{
		hdCompression_Unzip(compressedBuffer, fileEntry->m_compressedSize, 
							&uncompressedBuffer, &uncompressedSize);
		free(compressedBuffer);
	}
	else
	{
		// Clear some space for the uncompressed buffer
		if (NULL == (uncompressedBuffer = (char *)calloc(1, sizeof(char) * fileEntry->m_uncompressedSize)))
		{
			hdPrintf("[hdPakFileSystem] No space for uncompressed buffer %s\n", tmp);
			goto OpenFileFail;
		}
		
		// straight copy from compressed to uncompressed
		memcpy(uncompressedBuffer, compressedBuffer, fileEntry->m_uncompressedSize);
		free(compressedBuffer);
		uncompressedSize = fileEntry->m_uncompressedSize;
	}
	
	// allocate bytes for the output buffer
	if (NULL == (*fileBytes = (char *)calloc(1, sizeof(char) * uncompressedSize)))
	{
		hdPrintf("[hdPakFileSystem] MALLOC FAIL %s\n", tmp);
		goto OpenFileFail;
	}
	
	memcpy(*fileBytes, uncompressedBuffer, uncompressedSize);
	*fileLen = uncompressedSize;
	
	free(uncompressedBuffer);
	FileSystem_CloseFile(pakFileHnd);

	return 0;

	
OpenFileFail:
	
	*fileBytes = NULL;
	*fileLen = 0;
	
	if (uncompressedBuffer)
		free(uncompressedBuffer);
	
	if (compressedBuffer)
		free(compressedBuffer);
	
	if (pakFileHnd)
		FileSystem_CloseFile(pakFileHnd);
	
	return -1;
	
}


filehandle_t *hdPakFileSystem::OpenFile(const char* fileName)
{
	filehandle_t *hnd;
	
	// if filename contains a leading slash, remove it.
	if (fileName[0] == '/')
	{
		fileName += 1;
	}
	
	hnd = (filehandle_t*)calloc(1, sizeof(filehandle_t));
	//memset(hnd, 0, sizeof( filehandle_t));
	
	if (-1 == hdPakFileSystem::OpenFile(fileName, (char **)&hnd->filedata, (int *)&hnd->filesize))
	{
		free(hnd);
		return NULL;
	}
	
	hnd->ptrStart =  hnd->ptrCurrent = (unsigned char *)hnd->filedata;
	hnd->ptrEnd =  (unsigned char *)hnd->filedata + hnd->filesize;
	hnd->bLoaded = true;
	hnd->mmapped = false;
	
	return hnd;
}
