#pragma once
#ifndef COPY_H_INCLUDED
#define COPY_H_INCLUDED

#include <z/core/string.h>
#include <fstream>

namespace z
{
    namespace file
    {
        /**
         * \brief Copy data from one file to another.
         *
         * \param fileInput the path of the file to copy from.
         * \param fileOutput the path of the file to copy to.
         *
         * \return \b True if the file was copied successfully.
         * \b False otherwise.
         */
        bool copy(const core::string<utf8>& fileInput,
                  const core::string<utf8>& fileOutput)
        {
            std::ifstream src((char*)fileInput.cstring(), std::ios::binary);
            std::ofstream dest((char*)fileOutput.cstring(), std::ios::binary);
            dest << src.rdbuf();
            return src && dest;
        }
    }
}

#endif // COPY_H_INCLUDED
