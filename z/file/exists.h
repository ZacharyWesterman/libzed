/**
 * File:            exists.h
 * Namespace:       z::file
 * Description:     The exists(core::string) function returns true if
 *                  the given file exists and can be read.
 *                  Returns false otherwise.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   29 Jun. 2017
**/

#pragma once
#ifndef EXISTS_H_INCLUDED
#define EXISTS_H_INCLUDED

#include <z/core/string.h>

#include <fstream>

namespace z
{
    namespace file
    {
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
