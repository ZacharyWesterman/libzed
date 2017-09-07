#pragma once
#ifndef DIRECTORYOPERATIONS_H_INCLUDED
#define DIRECTORYOPERATIONS_H_INCLUDED

#include <z/core/string.h>

namespace z
{
    namespace file
    {
        /**
         * \brief Shorten the given directory string.
         *
         * Removes any extra symbols from the given directory string.
         * Extra slashes are removed, as well as redundant symbols
         * like \b "/./". Unnecessary directory backtracking is
         * also removed.
         * <BR>(e.g. "C:/a1/b1/../b2/foo.bar" -> "C:/a1/b2/foo.bar")
         *
         * \param dir the given directory.
         *
         * \return The directory with redundant symbols removed.
         */
        core::string<char> shorten(const core::string<char>& dir)
        {
            core::string<char> output = dir;

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
