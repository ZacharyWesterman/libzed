#pragma once
#ifndef EXISTS_H_INCLUDED
#define EXISTS_H_INCLUDED

#include <z/core/string.h>

#include <fstream>

namespace z
{
    namespace file
    {
        /**
         * \brief Check whether a file with the given
         * name exists.
         *
         * \param filename the full file name, including
         * directory.
         *
         * \return \b True if the file exists. \b False
         * otherwise.
         */
        bool exists(const core::string<char>& filename)
        {
            std::ifstream file (filename.str());

            bool can_open = file.good();

            file.close();

            return can_open;
        }
    }
}

#endif // EXISTS_H_INCLUDED
