#pragma once
#ifndef EXISTS_H_INCLUDED
#define EXISTS_H_INCLUDED

#include <z/core/string.h>

#include <sys/stat.h>

namespace z
{
    namespace file
    {
        /**
         * \brief Check whether a file or directory with
         * the given name exists.
         *
         * \param pathname the relative or absolute path
         * of the file or directory.
         *
         * \return \b True if the file or directory exists.
         * \b False otherwise.
         */
        bool exists(const core::string<char>& pathname)
        {
            struct stat info;

            if (stat(pathname.str(), &info) != 0)
                return true; //directory item exists
            else
                return false; //does not exist
        }
    }
}

#endif // EXISTS_H_INCLUDED
