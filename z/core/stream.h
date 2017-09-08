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
        /**
         * \brief A template class for character streams.
         *
         * Allows for condensing multiple objects
         * into one string.<BR>
         * <B>This class is incomplete</B>
         *
         */
        template <typename CHAR>
        class stream
        {
        private:
            string<CHAR> data;

        public:

            /**
             * \brief Get the length of the stream.
             */
            int length() const {return data.length();}

            /**
             * \brief Get the stream character array.
             */
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


            ///Shift into a scope.
            void shift_in()
            {
                data += (CHAR)SI;
            }

            ///Shift out of a scope.
            void shift_out()
            {
                data += (CHAR)SO;
            }

            ///Get the data in the nearest scope.
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

            ///Check whether there is any data in the stream.
            bool isEmpty() {return (data.length() == 0);}
        };

    }
}

#endif // STREAM_H_INCLUDED
