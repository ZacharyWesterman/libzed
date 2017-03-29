/**
 * File:            directory_operations.h
 * Namespace:       z::file
 * Description:     Provides methods for directory
 *                  shortening, and separation of
 *                  file name and directory.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   13 Mar. 2017
**/

#pragma once
#ifndef DIRECTORY_OPERATIONS_H_INCLUDED
#define DIRECTORY_OPERATIONS_H_INCLUDED

#include <z/core/string.h>

namespace z
{
    namespace file
    {
        ///Template for directory shortening function,
        ///which removed extra symbols from a directory string.
        ///(e.g. "C:\a1\b1\..\b2\foo.bar" -> "C:\a1\b2\foo.bar")
        template <typename char>
        core::string<char> shorten(const core::string<char>& input)
        {
            core::string<char> output = input;

            int last_slash = -1;

            for (int i=0; i<output.length(); i++)
            {
                if ((output[i] == '\\') ||
                    (output[i] == '/'))
                {
                    if (last_slash == i-1)
                    {
                        output.remove(i,i);
                        i--;
                    }

                    last_slash = i;
                }
                else if (output.foundAt(i, ".."))
                {
                    output.
                }
            }

            return output;
        }
    }
}

#endif // DIRECTORY_OPERATIONS_H_INCLUDED
