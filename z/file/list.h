#pragma once
#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <z/core/string.h>
#include <z/core/array.h>

#ifdef __linux__
#include <dirent.h>
#elif _WIN32
#include <windows.h>
#else
#warning "z/file/list.h" is incompatible with target OS.
#endif

namespace z
{
    namespace file
    {
        /**
         * \brief List all files of a given type
         * in the given directory.
         *
         * This function is meant to be a
         * platform-independent way of allowing the user
         * to get a list of all files in the given
         * directory with the given file extension.
         *
         * \param dir the working directory. If \b dir is \b "",
         * then it is assumed to be the current working directory.
         *
         * \param file_type the file extension. If the type is \b "*",
         * then all types are accepted. Otherwise, the file type
         * is expected to have no leading period.
         *
         * \return An array containing the names of all files of
         * the given type in the given directory.
         */
        core::array< core::string<utf8> >
            listFiles(const core::string<utf8>& dir,
                 const core::string<utf8>& file_type)
        {
            core::array< core::string<utf8> > output;

            core::string<utf8> search_path = dir;

            if (!dir.length())
                search_path += "./";
            else
                search_path += "/";


            #ifdef _WIN32
            search_path += "/*.";

            search_path += file_type;


            WIN32_FIND_DATA fd;
            HANDLE hFind = FindFirstFile((char*)search_path.cstring(), &fd);

            if (hFind != INVALID_HANDLE_VALUE)
            {
                do
                {
                    if(!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                    {
                        output.add(core::string<utf8>(fd.cFileName));
                    }
                }
                while (FindNextFile(hFind, &fd));

                FindClose(hFind);
            }
            #else

            bool any_extension;

            any_extension = (file_type == "*");



            DIR *dpdf;
            dirent *epdf;

            dpdf = opendir((char*)search_path.cstring());

            if (dpdf)
            {
                while ((epdf = readdir(dpdf)))
                {
                    core::string<utf8> filename (epdf->d_name);

                    if (epdf->d_type != DT_DIR)
                    {
                        if (any_extension)
                        {
                            output.add(filename);
                        }
                        else
                        {


                            if (filename.endsWith(core::string<utf8>('.') + file_type))
                                output.add(filename);
                        }
                    }
                }
            }

            closedir(dpdf);
            #endif

            return output;
        }



        /**
         * \brief List all sub-directories
         * in the given directory.
         *
         * This function is meant to be a
         * platform-independent way of allowing the user
         * to get a list of all sub-directories in the given
         * directory with the given file extension.
         *
         * \param dir the working directory. If \b dir is \b "",
         * then it is assumed to be the current working directory.
         *
         * \param showAll flag specifying whether to include
         * hidden directories in the output.
         *
         * \return An array containing the names of all
         * sub-directories in the given directory.
         */
        core::array< core::string<utf8> >
            listDirs(const core::string<utf8>& dir,
                     bool showAll = false)
        {
            core::array< core::string<utf8> > output;

            core::string<utf8> search_path = dir;

            if (!dir.length())
                search_path += "./";
            else
                search_path += "/";


            #ifdef _WIN32
            search_path += "/*";


            WIN32_FIND_DATA fd;
            HANDLE hFind = FindFirstFile((char*)search_path.cstring(), &fd);

            if (hFind != INVALID_HANDLE_VALUE)
            {
                do
                {
                    if(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                    {
                        core::string<utf8> filename = fd.cFileName;

                        if (showAll || (filename[0] != '.'))
                            output.add(filename);
                    }
                }
                while (FindNextFile(hFind, &fd));

                FindClose(hFind);
            }
            #else

            DIR *dpdf;
            dirent *epdf;

            dpdf = opendir((char*)search_path.cstring());

            if (dpdf)
            {
                while ((epdf = readdir(dpdf)))
                {
                    core::string<utf8> filename (epdf->d_name);

                    if (epdf->d_type == DT_DIR)
                    {
                        if (showAll || (filename[0] != '.'))
                            output.add(filename);
                    }
                }
            }

            closedir(dpdf);
            #endif

            return output;
        }
    }
}

#endif // LIST_H_INCLUDED
