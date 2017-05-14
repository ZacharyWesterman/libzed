/**
 * File:            directoryOperations.h
 * Namespace:       z::file
 * Description:     Provides methods for directory
 *                  shortening, and separation of
 *                  file name and directory.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   29 Mar. 2017
**/

#pragma once
#ifndef DIRECTORYOPERATIONS_H_INCLUDED
#define DIRECTORYOPERATIONS_H_INCLUDED

#include <z/core/string.h>

namespace z
{
    namespace file
    {
        ///Directory shortening function,
        ///which removes extra symbols from a directory string.
        ///(e.g. "C:/a1/b1/../b2/foo.bar" -> "C:/a1/b2/foo.bar")
        core::string<char> shorten(const core::string<char>& input)
        {
            core::string<char> output = input;

            int last_slash = -1;

            for (int i=0; i<output.length(); i++)
            {
                if ((output[i] == '\\') ||
                    (output[i] == '/'))
                {
                    if (output.foundAt("..", i+1) &&
                        (last_slash > -1))
                    {
                        output.remove(last_slash, i+2);
                        i--;
                    }
                    else if ((output[i+1] == '.') &&
                             ((output[i+2] == null) ||
                              (output[i+2] == '\\') ||
                              (output[i+2] == '/')))
                    {
                        output.remove(i+1,i+1);
                    }
                    else if (last_slash == i-1)
                    {
                        output.remove(i,i);
                        i--;
                    }

                    last_slash = i;
                }
            }

            if (last_slash == output.length() - 1)
                output.remove(output.length() - 1, output.length() - 1);

            return output;
        }
    }
}

#endif // DIRECTORYOPERATIONS_H_INCLUDED
