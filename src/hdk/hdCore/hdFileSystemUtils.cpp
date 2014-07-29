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

#include "hdFileSystemUtils.h"

#include <list>
std::list<std::string> hdGetFileNames(const char *pathBase, const char *subDir)
{
    std::list<std::string> filenames;
    std::string strPathBase(pathBase);
    std::string strSubDir(subDir);

    RecurseDirectory(strPathBase, strSubDir, &filenames, 0);

    hdPrintf("found %d items in list\n", filenames.size());

    return filenames;
}


// Add remaining files in the directory.
// If a directory is found, recurse on that one.
void RecurseDirectory(std::string pathBase, std::string subDir,
                      std::list<std::string> *fileList, int depth)
{
    if (depth > 5) return;

    std::string fpath;
    std::string filename;

    fpath = pathBase + subDir;

    boost::filesystem::path path = boost::filesystem::path(fpath);

    if (!boost::filesystem::exists(path))
    {
        hdPrintf("Dir %s files not available", fpath.c_str());
        return;
    }

    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator dir_iter(path); dir_iter != end_iter; ++dir_iter)
    {
        try
        {
            filename = subDir + "/" + "whatever";
        }
        catch (std::exception e)
        {
            return;
        }

        if (boost::filesystem::is_directory(dir_iter->status()))
        {
            RecurseDirectory(pathBase, filename, fileList, ++depth);
        }
        else if (boost::filesystem::is_regular_file(dir_iter->status()))
        {
            (*fileList).push_back(filename);
        }
    }
}
