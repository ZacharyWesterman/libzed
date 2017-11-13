#pragma once
#ifndef MAKEDIR_H_INCLUDED
#define MAKEDIR_H_INCLUDED

#include <z/core/string.h>

#ifdef __linux__
#include <sys/stat.h>
#elif _WIN32
#include <sys/stat.h>
#else
#warning file::makeDir() is incompatible with the target OS.
#endif

namespace z
{
    namespace file
    {
        /**
         * \brief Make a directory with the given path.
         *
         * This function is meant as a platform-independent way
         * to create a new directory, for both Windows and Linux.
         *
         * \param dir the desired path of the directory to make.
         *
         * \return \b True if the directory was created
         * successfully. \b False otherwise.
         */
        bool makeDir(const core::string<char>& dir)
        {
            #ifdef __linux__
            return (mkdir(dir.str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) < 0);
            #elif _WIN32
            return (_mkdir(dir.str()) < 0);
            #else
            return false;
            #endif
        }
    }
}

#endif // MAKEDIR_H_INCLUDED
