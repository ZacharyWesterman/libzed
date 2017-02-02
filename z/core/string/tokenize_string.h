#pragma once
#ifndef TOKENIZE_STRING_H_INCLUDED
#define TOKENIZE_STRING_H_INCLUDED

#include "../string.h"
#include "../array.h"


namespace z
{
    namespace core
    {
        template <typename CHAR>
        void tokenize(const array< string<CHAR> >& delims,
                      const string<CHAR>& inString,
                      array< string<CHAR> >& outArray)
        {
            outArray.clear();


            bool prevNotDelim = false;

            string<CHAR> thisString;

            for (int i=0; i<inString.length(); i++)
            {
                for (uint j=0; j<delims.size(); j++)
                {
                    if (inString.foundAt(delim[j], j))
                    {
                        if (prevNotDelim)
                        {
                            outArray.append(thisString);
                            thisString.clear();
                        }

                        prevNotDelim = false;

                        i += delim[j].length();
                    }
                    else
                    {
                        thisString += inString[i];
                        prevNotDelim = true;
                    }
                }
            }

            if (thisString.size() > 0)
                outArray.append(thisString);
        }

        template <typename CHAR>
        void tokenize(const string<CHAR>& delim,
                      const string<CHAR>& inString,
                      array< string<CHAR> >& outArray)
        {
            array< string<CHAR> > delims;
            delims.append(delim);

            tokenize(delims, inString, outArray);
        }

        template <typename CHAR>
        void tokenize(const CHAR delim, const string<CHAR>& inString, array< string<CHAR> >& outArray)
        {
            string<CHAR> delimStr(delim);

            tokenize(delimStr, inString, outArray);
        }
    }
}

#endif // TOKENIZE_STRING_H_INCLUDED
