/**
 * File:            convert_char_type.h
 * Namespace:       z::core
 *
 * Description:     Functions for converting between
 *                  character types.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   2 Feb. 2017
**/

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


        void narrow(const wchar_t& input, char& output)
        {
            if (input < 128)
                output = (char)input;
            else
                output = '?';
        }

        void widen(const char& input, wchar_t& output)
        {
            output = (wchar_t)input;
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



        ///Functions for converting between character types
        void inline convertChr(char& to, const wchar_t& from)
        {
            narrow(from, to);
        }

        void inline convertChr(char& to, const char& from)
        {
            to = from;
        }

        void inline convertChr(wchar_t& to, const char& from)
        {
            widen(from, to);
        }

        void inline convertChr(wchar_t& to, const wchar_t& from)
        {
            to = from;
        }
    }
}

#endif // CONVERT_CHAR_TYPE_H_INCLUDED
