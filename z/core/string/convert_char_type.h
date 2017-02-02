#pragma once
#ifndef CONVERT_CHAR_TYPE_H_INCLUDED
#define CONVERT_CHAR_TYPE_H_INCLUDED

namespace z
{
    namespace core
    {
        ///function for narrowing strings
        ///THIS ASSUMES THE CHARACTER ARRAYS ARE OF EQUAL LENGTH
        void narrow(wchar_t*& input, char*& output, int& length)
        {
            for (int i=0; i<length; i++)
            {
                wchar_t code = input[i];

                if (code < 128)
                    output[i] = (char)code;
                else
                {
                    output[i] = '?';

                    if ((code >= 0xD800) && (code <= 0xD8FF))
                        i++;
                }
            }
        }

        ///function for narrowing strings
        ///THIS ASSUMES THE CHARACTER ARRAYS ARE OF EQUAL LENGTH
        void widen(char*& input, wchar_t*& output, int& length)
        {
            for (int i=0; i<length; i++)
            {
                output[i] = (wchar_t)input[i];
            }
        }



        ///Functions for converting between string types
        void inline convertStr(char*& to, wchar_t*& from, int length)
        {
            to = new char[length];

            narrow(from, to, length);
        }

        void inline convertStr(char*& to, char*& from, int length)
        {
            to = from;
        }


        void inline convertStr(wchar_t*& to, char*& from, int length)
        {
            to = new wchar_t[length];

            widen(from, to, length);
        }

        void inline convertStr(wchar_t*& to, wchar_t*& from, int length)
        {
            to = from;
        }
    }
}

#endif // CONVERT_CHAR_TYPE_H_INCLUDED
