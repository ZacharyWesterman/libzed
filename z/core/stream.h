/**
 * File:            stream.h
 * Namespace:       z::core
 *
 * Description:     A template class for character streams.
 *                  Allows for condensing multiple objects
 *                  into one string.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   21 Feb. 2017
**/

#pragma once
#ifndef STREAM_H_INCLUDED
#define STREAM_H_INCLUDED

#include <type_traits>

#include "string.h"

#ifndef null
    #define null 0
#endif // null

static const int ETX = 3; //end of text
static const int SI  = 15; //shift-in
static const int SO  = 14; //shift-out

namespace z
{
    namespace core
    {
        template <typename CHAR>
        class stream
        {
        private:
            string<CHAR> data;

        public:

            int length() const {return data.length();}

            const CHAR* str() const {return data.str();}


            ///Stream output operator when left operand is a string
            stream& operator<<(const string<CHAR>& arg2)
            {
                data += arg2 + (CHAR)ETX;

                return *this;
            }

            ///Stream output operator when both operands are streams
            stream& operator<<(const stream& arg2)
            {
                data += arg2.data;

                return *this;
            }


            ///Other stream output operators
            //friend stream<char> operator<<(const string<char>&, const stream<char>&);
            //friend stream<wchar_t> operator<<(const string<wchar_t>&, const stream<wchar_t>&);

            //friend stream<char> operator<<(const string<char>&, const string<char>&);
            //friend stream<wchar_t> operator<<(const string<wchar_t>&, const string<wchar_t>&);



            ///Stream input operator for strings
            stream& operator>>(string<CHAR>& arg2)
            {
                CHAR ETX = 3; //end of text


                int stop  = data.find(ETX);

                if (stop  <= -1)
                {
                    arg2.clear();
                    data.clear();
                }
                else
                {
                    arg2 = data.substr(0, stop-1);

                    data.remove(0, stop);
                }

                return *this;
            }

            ///Stream input operator for characters
            stream& operator>>(CHAR& arg2)
            {
                int stop  = data.find((CHAR)ETX);

                if (stop  <= -1)
                {
                    arg2 = null;
                    data.clear();
                }
                else
                {
                    arg2 = data[0];

                    data.remove(0, stop);
                }

                return *this;
            }



            ///Stream input operator for numerical types
            template<
                typename T, //real type
                typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
            >
            stream& operator>>(T& arg2)
            {
                int stop  = data.find((CHAR)ETX);

                arg2 = (T)value(data.substr(0, stop-1));

                data.remove(0, stop);


                return *this;
            }


            void shift_in()
            {
                data += (CHAR)SI;
            }

            void shift_out()
            {
                data += (CHAR)SO;
            }

            stream pop()
            {
                int start = data.find((CHAR)SI);
                int stop  = data.find((CHAR)SO);


                stream output;

                if ((start > -1) &&
                    (stop > -1))
                {
                    output.data = data.substr(start+1, stop-1);

                    data.remove(start, stop);
                }

                return output;
            }

            bool isEmpty() {return (data.length() == 0);}
        };





        /*///Stream output operator when right operand is a string (narrow chars)
        stream<char> operator<<(const string<char>& arg1, const stream<char>& arg2)
        {
            stream<char> output;

            output.data = arg1 + (char)ETX;
            output.data += arg2.data;

            return output;
        }


        ///Stream output operator when right operand is a string (wide chars)
        stream<wchar_t> operator<<(const string<wchar_t>& arg1, const stream<wchar_t>& arg2)
        {
            stream<wchar_t> output;

            output.data = arg1 + (wchar_t)ETX;
            output.data += arg2.data;

            return output;
        }


        ///Stream output operator when both operands are strings (narrow chars)
        stream<char> operator<<(const string<char>& arg1, const string<char>& arg2)
        {
            stream<char> output;

            char ETX = 3; //end of text

            output.data = arg1 + ETX;
            output.data += arg2 + ETX;

            return output;
        }


        ///Stream output operator when both operands are strings (wide chars)
        stream<wchar_t> operator<<(const string<wchar_t>& arg1, const string<wchar_t>& arg2)
        {
            stream<wchar_t> output;

            wchar_t ETX = 3; //end of text

            output.data = arg1 + ETX;
            output.data += arg2 + ETX;

            return output;
        }*/
    }
}

#endif // STREAM_H_INCLUDED
