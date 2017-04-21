/**
 * File:            list_files_in_dir.h
 * Namespace:       z::file
 * Description:     The list_files_in_dir() function is meant to be a
 *                  platform-independent way of allowing the user
 *                  to get a list of all files in the given relative
 *                  directory with the given file extension.
 *
 * Usage:           list_files_in_dir(path, type, output)
 *                  if path is "", it is assumed to be the current working directory.
 *                  if the type is "*", then all types are accepted. otherwise,
 *                  file types are expected to have no leading '.'
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Mar. 2017
**/

#pragma once
#ifndef LIST_FILES_IN_DIR_H_INCLUDED
#define LIST_FILES_IN_DIR_H_INCLUDED

#include <z/core/string.h>
#include <z/core/array.h>

#ifdef __linux__

#include <dirent.h>

namespace z
{
    namespace file
    {
        void listFilesInDir(const core::string<char>& dir,
                            const core::string<char>& file_type,
                            core::array< core::string<char> >& output)
        {
            core::string<char> search_path = dir;

            if (!dir.length())
                search_path += "./";
            else
                search_path += "/";

            bool any_extension;

            any_extension = (file_type == "*");



            DIR *dpdf;
            dirent *epdf;

            dpdf = opendir(search_path.str());

            if (dpdf)
            {
                while ((epdf = readdir(dpdf)))
                {
                    core::string<char> filename (epdf->d_name);

                    if (epdf->d_type != DT_DIR)
                    {
                        if (any_extension)
                        {
                            output.add(filename);
                        }
                        else
                        {


                            if (filename.endsWith(core::string<char>('.') + file_type))
                                output.add(filename);
                        }
                    }
                }
            }

            closedir(dpdf);
        }
    }
}

#elif _WIN32

#include <windows.h>

namespace z
{
    namespace file
    {
        void listFilesInDir(const core::string<char>& dir,
                            const core::string<char>& file_type,
                            core::array< core::string<char> >& output)
        {
            core::string<char> search_path = dir;

            if (!dir.length())
                search_path += "./";
            else
                search_path += "/";

            search_path += "/*.";

            search_path += file_type;


            WIN32_FIND_DATA fd;
            HANDLE hFind = FindFirstFile(search_path.str(), &fd);

            if (hFind != INVALID_HANDLE_VALUE)
            {
                do
                {
                    if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                    {
                        output.add(core::string<char>(fd.cFileName));
                    }
                }
                while (FindNextFile(hFind, &fd));

                FindClose(hFind);
            }
        }
    }
}

#else
    #warning "list_files_in_dir.h" is incompatible with target OS.
#endif

#endif // LIST_FILES_IN_DIR_H_INCLUDED
