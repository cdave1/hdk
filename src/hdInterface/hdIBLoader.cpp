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

#include "hdIBLoader.h"

using namespace std;


static hdIBLoader * m_ViewControllerManagerInstance = NULL;


hdIBLoader * hdIBLoader::Instance()
{
	if (m_ViewControllerManagerInstance == NULL)
	{
		m_ViewControllerManagerInstance = new hdIBLoader();
	}
	return m_ViewControllerManagerInstance;
}


void hdIBLoader::TearDown()
{	
	delete m_ViewControllerManagerInstance;
	m_ViewControllerManagerInstance = NULL;
}


hdIBLoader::hdIBLoader()
{
}


hdIBLoader::~hdIBLoader()
{
}


hdIBViewController* hdIBLoader::FindViewController(const char* filename)
{
	return this->LoadViewControllerFromText(filename);
}


bool hdIBLoader::BackupViewControllerFile(const char *path)
{
	char * buffer;
	long size;
	char backupPath[256];
	
	snprintf(backupPath, 256, "%s.backup", path);
	
	std::ifstream ifs(path);
	
	if (!ifs.is_open())
	{
		hdPrintf("No backup was made, because there is no file to backup (%s).\n", path);
		return false;
	}
	
	if (!ifs.good())
	{
		hdPrintf("BackupViewControllerFile: file problem at %s\n", path);
		ifs.close();
		return false;
	}
	
	std::ofstream backupStream(backupPath);
	ifs.seekg(0, std::ifstream::end);
	size = ifs.tellg();
	ifs.seekg(0);
	
	buffer = new char [size];
	ifs.read(buffer, size);
	
	backupStream.write(buffer, size);
	
	delete [] buffer;
	
	ifs.close();
	backupStream.close();
	return true;
}


bool hdIBLoader::SaveViewController(const hdIBViewController *vc, const char *path)
{
	// make a plaintext copy too
	std::ostringstream outstream(ios::in | ios::out	);
	boost::archive::text_oarchive textArchive(outstream);
	textArchive << (* vc);
	
	long txtLen = outstream.str().size();
	char *txtBuf;
	char txtPath[256];
	
	if (!(txtBuf = (char *)calloc(1, sizeof(char) * txtLen)))
	{
		hdPrintf("CALLOC\n");
		return false;
	}
	outstream.rdbuf()->sgetn(txtBuf, txtLen);
	
	std::ofstream txtFile(path);
	
	if (!txtFile.is_open())
	{
		free(txtBuf);
		return false;
	}
	
	if (!txtFile.good())
	{
		free(txtBuf);
		txtFile.close();
		return false;
	}
	
	txtFile.write(txtBuf, txtLen);
	txtFile.close();
	free(txtBuf);
	return true;
}


int hdIBLoader::SaveViewControllerBinary(const hdIBViewController *vc, const char *path)
{
	/* std::iostream stuff */
	char *ossBuffer;
	long ossLen;
	char *ifsBuffer;
	long ifsLen;
	
	/* zlib stuff */
	Bytef *destBuffer;
	uLongf destLen;
	Bytef *sourceBuffer;
	uLongf sourceLen;
	int zRes;
	char binaryPath[512];
	
	if (strlen(path) == 0) 
	{
		hdPrintf("ViewController::SaveViewController error: empty file path \n");
		return -1;
	}
	
	snprintf(binaryPath, 512, "%s.bin", path);
	hdPrintf("Attempting to save binary world file: %s\n", binaryPath);
	
	std::ostringstream oss(ios::binary | ios::in | ios::out	);
	boost::archive::binary_oarchive boa(oss);
	boa << (* vc);
	
	/*
	 * Compress the output stream before saving to disk.
	 From zlib:
	 
	 int compress(byte *dest, long *destLen, byte *source, long sourceLen)
	 - sourceLen: byte lenght of the source buffer
	 On entry:
	 - destLen is the total size of the destination buffer. The buffer
	 must be at 0.1% larger than sourceLen + 12 bytes
	 On exit:
	 - destLen is the actual size of the compressed buffer...
	 */
	ossLen = oss.str().size();
	ossBuffer = new char[ossLen];
	oss.rdbuf()->sgetn(ossBuffer, ossLen);
	
	sourceLen = (uLongf)ossLen;
	sourceBuffer = new Bytef[sourceLen];
	memcpy(sourceBuffer, ossBuffer, ossLen * sizeof(Bytef));
	
	destLen = (uLongf)((float)sourceLen * 1.5f);
	destBuffer = new Bytef[destLen];
	
	zRes = compress(destBuffer, &destLen, sourceBuffer, sourceLen);
	hdAssert(zRes == Z_OK);
	
	/*
	 * Prefix the uncompressed filesize to the stream.
	 */
	ifsLen = (long)(destLen + 16);
	ifsBuffer = new char[ifsLen];
	
	snprintf(ifsBuffer, 16, "%-16ld", (long)sourceLen);
	memcpy(ifsBuffer+16, destBuffer, (long)(ifsLen * sizeof(char)));
	
	/*
	 * Write the compressed string to file.
	 */
	std::ofstream ofs(binaryPath);
	hdAssert(ofs.is_open());
	ofs.write(ifsBuffer, ifsLen);
	ofs.close();
	
	delete [] ossBuffer;
	delete [] sourceBuffer;
	delete [] destBuffer;
	delete [] ifsBuffer;
	
	return 0;
}


hdIBViewController * hdIBLoader::LoadViewControllerFromText(const char* filename)
{
	return this->LoadViewControllerFromTextInternal(filename, HD_FS_ABSOLUTE_PATH);
}


hdIBViewController * hdIBLoader::LoadViewControllerFromTextPakFile(const char* filename)
{
	return this->LoadViewControllerFromTextInternal(filename, 0);
}


hdIBViewController * hdIBLoader::LoadViewControllerFromTextInternal(const char* filename, unsigned int FlagsAndAttributes)
{
	char *txtBuf;
	long txtLen, readLen;
	
	char levelfilepath[256];
	filehandle_t *txtFileHnd;
	
	snprintf(levelfilepath, 256, "%s", filename);
	hdPrintf("\n\nAttempting to load world file: %s\n", levelfilepath);
	txtFileHnd = FileSystem_OpenFile( levelfilepath, FlagsAndAttributes);
	if(!txtFileHnd)
    {
		hdPrintf( "Could not open (%s) for reading...\n", levelfilepath );
		return NULL;
    }
	
	txtLen = FileSystem_GetFileSize(txtFileHnd);
	if (txtLen == 0)
	{
		hdPrintf( "World File (%s) was empty...\n", levelfilepath );
		FileSystem_CloseFile(txtFileHnd);
#ifdef DEBUG
		hdAssert(false);
#else
		return NULL;
#endif
	}
	
	if (!(txtBuf = (char *)calloc(1, sizeof(char) * txtLen)))
	{
		hdPrintf("CALLOC");
		FileSystem_CloseFile(txtFileHnd);
#ifdef DEBUG
		hdAssert(false);
#else
		return NULL;
#endif
	}
	
	readLen = FileSystem_ReadFile(txtBuf, sizeof(char), txtLen, txtFileHnd);
	FileSystem_CloseFile(txtFileHnd);
	if (readLen == txtLen)
	{
		hdPrintf("readfile post condition ok %d\n", sizeof(char));
	}
	
	
	hdPrintf("ViewControllerManager::LoadViewControllerFromText\n");
	
	std::istringstream iss(ios::in | ios::out);
	streamsize ssize = iss.rdbuf()->sputn(txtBuf, txtLen);
	hdPrintf("Read %d bytes (expected: %d)\n",ssize,txtLen);
	free(txtBuf);
	boost::archive::text_iarchive textArchive(iss);
	hdIBViewController *vc = new hdIBViewController(); //perm;
	textArchive >> (*vc);
	hdPrintf("ViewControllerManager::LoadViewControllerFromText done\n");
	return vc; 
}


hdIBViewController * hdIBLoader::LoadViewController(const char* filename)
{
	/* std::iostream stuff */
	char *hfBuffer;
	long hfLen;
	char *issBuffer;
	long issLen;
	
	/* zlib stuff */
	Bytef *destBuffer;
	uLongf destLen;
	Bytef *sourceBuffer;
	uLongf sourceLen;
	int zRes;
	
	unsigned int readLen;
	char levelfilepath[256];
	filehandle_t *hFile;
	
	snprintf(levelfilepath, 256, "%s", filename);
	hdPrintf("Attempting to load world file: %s\n", levelfilepath);
	hFile = FileSystem_OpenFile( levelfilepath, 0 );
	if(!hFile)
    {
		hdPrintf( "Could not open (%s) for reading...\n", levelfilepath );
		return NULL;
    }
	
	hfLen = FileSystem_GetFileSize(hFile);
	if (hfLen == 0)
	{
		hdPrintf( "World File (%s) was empty...\n", levelfilepath );
		FileSystem_CloseFile(hFile);
#ifdef DEBUG
		hdAssert(false);
#else
		return NULL;
#endif
	}
	
	hfBuffer = new char[hfLen];
	readLen = FileSystem_ReadFile(hfBuffer, sizeof(unsigned char), hfLen, hFile);
	
	/* first 16 bytes containing uncompressed file size */
	char tmp[16];
	strncpy(tmp, hfBuffer, 16);
	destLen = (uLongf)strtol(tmp, NULL, 0);
	destBuffer = new Bytef[destLen];
	
	if ( destLen <= 0)
	{
		hdPrintf("Data file is not a compressed world file - aborting...");
		FileSystem_CloseFile(hFile);
		delete [] hfBuffer;
		delete [] destBuffer;
		return NULL;
	}
	
	sourceLen = (uLongf)(hfLen - 16);
	sourceBuffer = new Bytef[sourceLen];
	memcpy(sourceBuffer, hfBuffer+16, sizeof(Bytef) * sourceLen);	
	
	/*
	 via zlib docs:
	 
	 ZEXTERN int ZEXPORT uncompress OF((Bytef *dest,   uLongf *destLen,
	 const Bytef *source, uLong sourceLen));
	 
	 Decompresses the source buffer into the destination buffer.  sourceLen is
	 the byte length of the source buffer. Upon entry, destLen is the total
	 size of the destination buffer, which must be large enough to hold the
	 entire uncompressed data. (The size of the uncompressed data must have
	 been saved previously by the compressor and transmitted to the decompressor
	 by some mechanism outside the scope of this compression library.)
	 Upon exit, destLen is the actual size of the compressed buffer.
	 This function can be used to decompress a whole file at once if the
	 input file is mmap'ed.
	 
	 uncompress returns Z_OK if success, Z_MEM_ERROR if there was not
	 enough memory, Z_BUF_ERROR if there was not enough room in the output
	 buffer, or Z_DATA_ERROR if the input data was corrupted or incomplete.
	 */
	zRes = uncompress(destBuffer, &destLen, sourceBuffer, sourceLen);
	if ( zRes != Z_OK)
	{
		hdPrintf("Could not decompress data in the file - aborting...");		
		delete [] hfBuffer;
		delete [] destBuffer;
		delete [] sourceBuffer;
		FileSystem_CloseFile(hFile);
		return NULL;
	}
	
	issLen = (long)destLen;
	issBuffer = new char[issLen];
	memcpy(issBuffer, destBuffer, (long)(issLen * sizeof(char)));
	FileSystem_CloseFile(hFile);
	
	hdIBViewController *vc = this->LoadViewControllerFromUncompressedBinary(issBuffer, issLen);
	
	delete [] issBuffer;
	delete [] hfBuffer;
	delete [] destBuffer;
	delete [] sourceBuffer;
	
	return vc;
}


hdIBViewController * hdIBLoader::LoadViewControllerFromUncompressedBinary(const char *bin, unsigned long binLength)
{
	hdIBViewController *vc = new hdIBViewController(); //perm;
	hdPrintf("ViewControllerManager::LoadViewControllerFromUncompressedBinary\n");
	
	std::istringstream iss(ios::in | ios::out | ios::binary);
	iss.rdbuf()->sputn(bin, binLength);
	boost::archive::binary_iarchive ia(iss);
	
	ia >> (*vc);
	return vc; 
}