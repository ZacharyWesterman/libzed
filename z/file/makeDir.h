#pragma once
#ifndef MAKEDIR_H_INCLUDED
#define MAKEDIR_H_INCLUDED

#include <z/core/string.h>

#ifdef __linux__
#include <sys/stat.h>
#else
#warning file::makeDir() is incompatible with the target OS.
#endif

namespace z
{
    namespace file
    {
        bool makeDir(const core::string<char>& dir)
        {
            #ifdef __linux__
            if (mkdir(dir.str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) >= 0)
                return true;
            else
                return false;
            #else
            return false;
            #endif
        }
    }
}

#endif // MAKEDIR_H_INCLUDED
