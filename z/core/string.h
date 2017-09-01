/**
 * File:            string.h
 * Namespace:       z::core
 *
 * Description:     A template for character strings. The two
 *                  currently supported character types are
 *                  char and wchar_t. Strings can be created with
 *                  'z::core::string<TYPE> foo', where 'TYPE' is
 *                  either char or wchar_t.
 *                  This class is also type-independent, that is
 *                  it handles all type conversions automatically.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   1 Sep. 2017
**/


#pragma once
#ifndef STRING_H_INCLUDED
#define STRING_H_INCLUDED

#define null 0
#ifndef NULL
    #define NULL 0
#endif // NULL

//this is for initializing numbers properly
#include <type_traits>
#include <complex>

#include "charFunctions.h"

#define num_bufsiz 16
#define num_precision 9

//magic number for determining when to round numbers,
//e.g. at the ~0.5 mark.
#define num_round_magic 0.499999958629814528210388857587539


namespace z
{
    namespace core
    {

        //"buffer" is assumed to be 2*num_bufsiz + 7 characters long
        //returns number of characters in resultant string
        template<typename CHAR>
        static int num_to_cstring(const zFloat& number, CHAR* buffer)
        {
            zFloat_cast dbl_cst = {.value = number};

            bool use_scientific = (dbl_cst.part.expUnBias() > 50) ||
                                  (dbl_cst.part.expUnBias() < (-50));

            int buffer_pos = 0;

            zFloat fpart;
            long ipart;
            long exponent = 0;

            if (use_scientific)
            {
                while (dbl_cst.value >= 10)
                {
                    dbl_cst.value /= 10;
                    exponent++;
                }
            }

            ipart = dbl_cst.value;
            fpart = dbl_cst.value - ipart;

                //std::cout << fpart << ":" << ipart << std::endl;

                CHAR inv_ibuf[num_bufsiz];
                CHAR inv_fbuf[num_bufsiz];
                CHAR inv_xbuf[4];

                int ibufsiz = 0;
                int fbufsiz = 0;
                int xbufsiz = 0;


                if (ipart < 0)
                {
                    buffer[buffer_pos] = (CHAR)45;
                    buffer_pos++;

                    ipart = -ipart;
                }
                else if (fpart < 0)
                {
                    buffer[buffer_pos] = (CHAR)45;
                    buffer_pos++;

                    fpart = -fpart;
                }


                if (ipart == 0)
                {
                    inv_ibuf[0] = (CHAR)48;
                    ibufsiz++;
                }


                while (ipart != 0)
                {
                    inv_ibuf[ibufsiz] = (CHAR)((ipart % 10) + 48);
                    ipart /= 10;

                    ibufsiz++;
                }


                while ((fpart != 0) && (fbufsiz < num_precision))
                {
                    fpart *= 10;

                    int frac_char = fpart;

                    inv_fbuf[fbufsiz] = (CHAR)(frac_char + 48);

                    fbufsiz++;
                    fpart -= frac_char;
                }

                while (exponent != 0)
                {
                    inv_xbuf[xbufsiz] = (CHAR)((exponent % 10) + 48);
                    exponent /= 10;

                    xbufsiz++;
                }



                if (fpart >= num_round_magic)
                {
                    int i = fbufsiz - 1;

                    inv_fbuf[i]++;

                    while ((i > 0) && (inv_fbuf[i] > (CHAR)57))
                    {
                        fbufsiz--;
                        inv_fbuf[i-1]++;

                        i--;
                    }

                    if (inv_fbuf[0] > (CHAR)57)
                    {
                        fbufsiz--;
                        inv_ibuf[0]++;

                        i=0;

                        while ((i < ibufsiz) && (inv_ibuf[i] > (CHAR)57))
                        {
                            inv_ibuf[i] = (CHAR)48;

                            if (i+1 == ibufsiz)
                            {
                                if (ibufsiz < num_bufsiz)
                                {
                                    inv_ibuf[ibufsiz] = (CHAR)49;

                                    ibufsiz++;
                                }
                            }
                            else
                            {
                                inv_ibuf[i+1]++;
                            }

                            i++;
                        }
                    }
                }

                for (int i=ibufsiz-1; i>=0; i--)
                {
                    buffer[buffer_pos] = inv_ibuf[i];
                    buffer_pos++;
                }

                if (fbufsiz)
                {
                    buffer[buffer_pos] = (CHAR)46; // '.'
                    buffer_pos++;

                    int frac_len = 0;

                    for (int f=0; f<fbufsiz; f++)
                    {
                        buffer[buffer_pos + f] = inv_fbuf[f];

                        if (inv_fbuf[f] > 48)
                            frac_len = f + 1;
                    }

                    buffer_pos += frac_len;
                }


                if (xbufsiz)
                {
                    buffer[buffer_pos] = (CHAR)69; //'E'
                    buffer_pos++;

                    for (int i=xbufsiz-1; i>=0; i--)
                    {
                        buffer[buffer_pos] = inv_xbuf[i];
                        buffer_pos++;
                    }
                }

                //buffer is null-terminated
                buffer[buffer_pos] = (CHAR)0;

                if (buffer[buffer_pos-1] == (CHAR)46) //remove terminal decimals
                {
                    //don't allow -0
                    if ((buffer[0] == (CHAR)45) &&
                        (buffer[1] == (CHAR)48))
                    {

                        buffer[0] = (CHAR)48; //'0'
                        buffer[1] = (CHAR)0;

                        buffer_pos -= 2;
                    }
                    else
                    {
                        buffer[buffer_pos-1] = (CHAR)0;

                        buffer_pos--;
                    }
                }



                return buffer_pos;
        }




        template <typename CHAR>
        class string
        {
        private:
            int array_length;
            CHAR* string_array;


            //internal function to append a character array
            void append_string(const CHAR* buffer, int bufsiz);

            //internal function to append a single character
            void append_char(CHAR singleChar);

            //internal function to check if the specified sub-string is at the specified position
            bool found_sub_string_at(int pos, const CHAR* substr, int substr_len) const;

            //internal function to replace the substring in the given index range(inclusive) with the given string
            void replace_sub_string_at_with(int beg_index,
                                            int end_index,
                                            const CHAR* sub_str,
                                            int substr_len);

            //internal function to check string equality
            //returns -1 if this string is less than another,
            //        +1 if it is greater,
            //     and 0 if they are the same.
            int lessthan_equal_greater(const CHAR* other, int str_size) const;


            //internal function to remove a section of text from the string (start+end inclusive)
            void remove_portion(int beg_index, int end_index);


        public:
            //empty constructor
            string()
            {
                array_length = 1;
                string_array = new CHAR[1];
                string_array[0] = null;
            }


            //cstring constructor
            template <typename CHAR_2>
            string(const CHAR_2* buffer)
            {
                if (buffer)
                {
                    array_length = 0;

                    while (buffer[array_length] != null)
                        array_length++;

                    array_length++;

                    string_array = new CHAR[array_length];

                    if (sizeof(CHAR_2) <= sizeof(CHAR))
                    {
                        for (int i=0; i<array_length; i++)
                            string_array[i] = buffer[i];

                    }
                    else
                    {
                        int buf_i = 0;

                        for (int i=0; i<array_length; i++)
                        {
                            if (buffer[buf_i] < 128)
                                string_array[i] = buffer[buf_i];
                            else
                            {
                                string_array[i] = (CHAR)'?';

                                if ((buffer[buf_i] >= 0xD800) &&
                                    (buffer[buf_i] <= 0xD8FF))
                                    buf_i++;
                            }

                            buf_i++;
                        }
                    }
                }
                else
                    string();
            }


            //copy constructor
            template <typename CHAR_2>
            string(const string<CHAR_2>& other)
            {
                array_length = other.length()+1;

                string_array = new CHAR[array_length];

                if (sizeof(CHAR_2) <= sizeof(CHAR))
                {
                    for (int i=0; i<array_length-1; i++)
                        string_array[i] = other.str()[i];

                }
                else
                {
                    int buf_i = 0;

                    for (int i=0; i<array_length-1; i++)
                    {
                        if (other.str()[buf_i] < 128)
                            string_array[i] = other.str()[buf_i];
                        else
                        {
                            string_array[i] = (CHAR)'?';

                            if ((other.str()[buf_i] >= 0xD800) &&
                                (other.str()[buf_i] <= 0xD8FF))
                            buf_i++;
                        }

                        buf_i++;
                    }
                }
            }


            //char constructors
            string(const char& character)
            {
                array_length = 2;
                string_array = new CHAR[2];

                string_array[0] = character;

                string_array[1] = null;
            }

            string(const wchar_t& character)
            {
                array_length = 2;
                string_array = new CHAR[2];

                if ((sizeof(CHAR) > sizeof(wchar_t)) &&
                    (character >= 128))
                    string_array[0] = (CHAR)'?';
                else
                    string_array[0] = character;

                string_array[1] = null;
            }


            //numeric constructor
            ///converts a real number to a string
            template<
                typename T, //numeric type
                typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
            >
            string(const T& number)
            {
                //buffer assumed to be AT LEAST (2*num_bufsiz + 7) characters long!
                CHAR buffer[num_bufsiz + num_bufsiz + 7];

                array_length = num_to_cstring((zFloat)number, buffer) + 1;

                string_array = new CHAR[array_length];

                for (int i=0; i<array_length; i++)
                    string_array[i] = buffer[i];
            }


            //complex constructor
            ///converts a complex number to a string
            template<
                typename T, //numeric type
                typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type
            >
            string(const std::complex<T>& number)
            {
                if (number.imag() == 0)
                {
                    //buffer assumed to be AT LEAST (2*num_bufsiz + 7) characters long!
                    CHAR buffer[num_bufsiz + num_bufsiz + 7];

                    array_length = num_to_cstring((zFloat)number.real(), buffer) + 1;

                    string_array = new CHAR[array_length];

                    for (int i=0; i<array_length; i++)
                        string_array[i] = buffer[i];
                }
                else if (number.real() == 0)
                {
                    //buffer assumed to be AT LEAST (2*num_bufsiz + 7) characters long!
                    CHAR buffer[num_bufsiz + num_bufsiz + 8];

                    array_length = num_to_cstring((zFloat)number.imag(), buffer) + 2;
                    //append 'i', since imaginary value.
                    buffer[array_length-2] = (CHAR)105;
                    buffer[array_length-1] = (CHAR)0;

                    string_array = new CHAR[array_length];

                    for (int i=0; i<array_length; i++)
                        string_array[i] = buffer[i];
                }
                else
                {
                    //each buffer assumed to be AT LEAST (2*num_bufsiz + 7) characters long!
                    CHAR real_buffer[num_bufsiz + num_bufsiz + 8];
                    CHAR imag_buffer[num_bufsiz + num_bufsiz + 8];

                    int r_array_len = num_to_cstring((zFloat)number.real(), real_buffer) + 1;

                    //append '+'(if positive imaginary part),
                    //since imaginary value comes next.
                    if (number.imag() >= 0)
                        real_buffer[r_array_len-1] = (CHAR)43;
                    else
                        r_array_len--;


                    int i_array_len = num_to_cstring((zFloat)number.imag(), imag_buffer) + 2;
                    //append 'i', since imaginary value.
                    imag_buffer[i_array_len-2] = (CHAR)105;
                    imag_buffer[i_array_len-1] = (CHAR)0;

                    array_length = r_array_len + i_array_len;
                    string_array = new CHAR[array_length];

                    for (int i=0; i<r_array_len; i++)
                        string_array[i] = real_buffer[i];

                    for (int i=0; i<i_array_len; i++)
                        string_array[i+r_array_len] = imag_buffer[i];
                }
            }


            //destructor
            ~string()
            {
                if (string_array)
                    delete[] string_array;
            }


            //get number of characters in the string
            int size() const
            {
                int len = (int)(array_length)-1;

                if (len < 0)
                    len = 0;

                return len;
            }

            inline int length() const
            { return size(); }




            //get string array pointer
            inline const CHAR* str() const {return string_array;}




            //equality operator
            inline bool operator==(const string& other) const
            {
                return (lessthan_equal_greater(other.string_array,
                                               other.array_length) == 0);
            }


            //inequality operator
            inline bool operator!=(const string& other) const
            {
                return (lessthan_equal_greater(other.string_array,
                                               other.array_length) != 0);
            }


            //greater than operator
            inline bool operator>(const string& other) const
            {
                return (lessthan_equal_greater(other.string_array,
                                               other.array_length) == 1);
            }

            //greater than / equal to operator
            inline bool operator>=(const string& other) const
            {
                return (lessthan_equal_greater(other.string_array,
                                               other.array_length) != -1);
            }


            //less than operator
            inline bool operator<(const string& other) const
            {
                return (lessthan_equal_greater(other.string_array,
                                               other.array_length) == -1);
            }


            //less than / equal to operator
            inline bool operator<=(const string& other) const
            {
                return (lessthan_equal_greater(other.string_array,
                                               other.array_length) != -1);
            }


            //operator for concatenation of string
            inline const string& operator+=(const string& other)
            {
                append_string(other.string_array, other.array_length);

                return *this;
            }

            //addition operator
            inline const string operator+(const string& other) const
            {
                string output = *this;
                output += other;

                return output;
            }


            //function to get character at given index.
            //if the index is valid, returns character at that index.
            //otherwise, returns 0, since no character in the string will be 0.
            inline CHAR at(int index) const
            {
                if ((index < 0) ||
                    (index >= (int)array_length-1))
                    return null;
                else
                    return string_array[index];
            }

            //const index operator
            //Identical functionality to this->at(index).
            inline CHAR operator[](int index) const
            { return at(index); }


            //non-const versions of the previous two functions
            CHAR& at(int index)
            {
                if ((index < 0) ||
                    (index >= (int)array_length-1))
                    return string_array[array_length-1];
                else
                    return string_array[index];
            }

            inline CHAR& operator[](int index)
            { return at(index); }


            //function to find the position of the first occurrence of the given sub-string
            //returns -1 if it was not found
            int find(const string& sub_string) const;

            //function to find the position of the first occurrence of the given sub-string
            //after the given index.
            //returns -1 if it was not found
            int findAfter(const string& sub_string, int n) const;

            //function to find the position of the LAST occurrence of the given sub-string
            //returns -1 if it was not found
            int findLast(const string& sub_string) const;

            //function to find the position of the nth occurrence of the given sub-string
            //n starts at 0
            //returns -1 if it was not found
            int find(const string& sub_string, int n) const;


            //function to replace a sub-string (indexes start_pos to end_pos, inclusive) with some new string
            //no return, as this will always succeed when given correct parameters
            inline void replace(int start_pos, int end_pos, const string& new_string)
            {
                replace_sub_string_at_with(start_pos,
                                           end_pos,
                                           new_string.string_array,
                                           new_string.array_length);
            }


            //function to replace the first occurrence of the first given sub-string with the second one.
            //returns false if the sub-string wasn't in the original string, true otherwise.
            bool replace(const string& sub_string, const string& new_string);


            //function to replace the nth occurrence of the first given sub-string with the second one.
            //n starts at 0
            //returns false if the sub-string wasn't in the original string, true otherwise.
            bool replace(const string& sub_string, int n, const string& new_string);


            //function to return a substring given two positions
            const string substr(int start_index, int end_index) const;


            //function to return the number of times the given string appears in this string
            int count(const string& sub_string) const;


            //function to remove the portion of the string within the specified indexes (inclusive)
            inline void remove(int start_index, int end_index)
            { remove_portion(start_index, end_index); }


            //returns true if the specified sub-string was found at the specified position
            bool foundAt(const string& sub_string, int position) const;

            //returns true if the specified sub-string was found
            //AND ENDS AT the specified position
            bool foundEndAt(const string& sub_string, int position) const;

            //returns true if specified text was found at the beginning of the string
            inline bool beginsWith(const string& sub_string) const
            { return foundAt(sub_string, 0); }

            //returns true if specified text was found at the end of the string
            inline bool endsWith(const string& sub_string) const
            { return foundEndAt(sub_string, array_length-2); }

            //returns true if specified text was found at the beginning of the string,
            //after an allowed padding char (e.g. leading spaces)
            bool beginsWith(const string& sub_string, CHAR pad_char) const;

            //Clears the string
            void clear();

            template <typename CHAR_2>
            const string& operator=(const string<CHAR_2>& other)
            {
                delete[] string_array;

                array_length = other.length()+1;

                string_array = new CHAR[array_length];

                if (sizeof(CHAR_2) <= sizeof(CHAR))
                {
                    for (int i=0; i<array_length-1; i++)
                        string_array[i] = other.str()[i];

                }
                else
                {
                    int buf_i = 0;

                    for (int i=0; i<array_length-1; i++)
                    {
                        if (other.str()[buf_i] < 128)
                            string_array[i] = other.str()[buf_i];
                        else
                        {
                            string_array[i] = (CHAR)'?';

                            if ((other.str()[buf_i] >= 0xD800) &&
                                (other.str()[buf_i] <= 0xD8FF))
                            buf_i++;
                        }

                        buf_i++;
                    }
                }

                return *this;
            }


            inline const string<char> narrow() const
            { return string<char>(*this); }

            inline const string<wchar_t> wide() const
            { return string<wchar_t>(*this); }


            const string<CHAR> upper() const;

            const string<CHAR> lower() const;

            //const string<CHAR> camel();

            zFloat value(int base = 10) const;

            std::complex<zFloat> complexValue(int base = 10) const;
        };


        ///template for string evaluation functions
        // both ignore spaces, and returns 0.0 if non-numerical characters are present
        template <typename CHAR>
        zFloat string<CHAR>::value(int base) const
        {

            zFloat value = 0.0;

            int start = 0;
            bool isNegative = false;

            if (string_array[0] == (CHAR)45) //'-' character
            {
                start = 1;
                isNegative = true;
            }

            bool pastDecimal = false;
            bool pastExponent = false;
            zFloat fracMult = 1.0;
            int exponent = 0;
            bool expNegative = false;

            int length = array_length - 1;

            for (int i=start; i<length; i++)
            {
                //in the rare case that we encounter a null character
                if (string_array[i] == (CHAR)0)
                    break;

                //only one decimal point is allowed.
                //any more, and the string is invalid
                if (string_array[i] == (CHAR)46) //'.' character
                {
                    if (pastDecimal)
                    {
                        return 0.0;
                    }

                    pastDecimal = true;
                }

                //if a character is not part of a valid number,
                //the string evaluates to 0.0 and return.
                else if (isNumeric(string_array[i], base))
                {
                    if (pastExponent)
                    {
                        exponent *= base;
                        exponent += numeralValue(string_array[i]);
                    }
                    else if (pastDecimal)
                    {
                        fracMult /= (zFloat)base;
                        value += fracMult * (zFloat)numeralValue(string_array[i]); //actual value from character
                    }
                    else
                    {
                        value *= (zFloat)base;
                        value += (zFloat)numeralValue(string_array[i]); //actual value from character
                    }
                }
                else if (string_array[i] == (CHAR)69)
                {
                    pastExponent = true;
                }
                else if (pastExponent && (string_array[i] == (CHAR)45))
                {
                    expNegative = true;
                }
                else if (string_array[i] != (CHAR)46)
                {
                    return 0.0;
                }
            }


            if (exponent)
            {
                long valMult = 1;

                for(int i=0; i<exponent; i++)
                    valMult *= base;

                if (expNegative)
                    value *= (1 / (zFloat)valMult);
                else
                    value *= valMult;
            }

            //never output "-0"
            if (value == -0)
                return 0;

            if (isNegative)
            {
                return -value;
            }
            else
            {
                return value;
            }
        }


        template <typename CHAR>
        std::complex<zFloat> string<CHAR>::complexValue(int base) const
        {
            if (endsWith("i"))
            {
                zFloat real(0), imag;

                int realEnd = findLast("+");

                imag = substr(realEnd+1, length()-2).value(base);

                if (realEnd >= 0)
                {
                    real = substr(0, realEnd-1).value(base);
                }
                else
                {
                    realEnd = findLast("-");

                    if (realEnd >= 0)
                    {
                        real = substr(0, realEnd-1).value(base);
                        imag = -(substr(realEnd+1, length()-2).value(base));
                    }
                }

                return std::complex<zFloat> (real, imag);
            }
            else
            {
                return std::complex<zFloat> (value(base), 0);
            }

        }


        template <typename CHAR>
        const string<CHAR> string<CHAR>::upper() const
        {
            string<CHAR> result (*this);

            for (int i=0; i<result.length(); i++)
                if ((string_array[i] >= (CHAR)97) &&
                    (string_array[i] <= (CHAR)122))
                {
                    result.string_array[i] = string_array[i] - (CHAR)97 + (CHAR)65;
                }

            return result;
        }

        template <typename CHAR>
        const string<CHAR> string<CHAR>::lower() const
        {
            string<CHAR> result (*this);

            for (int i=0; i<result.length(); i++)
                if ((string_array[i] >= (CHAR)65) &&
                    (string_array[i] <= (CHAR)90))
                {
                    result.string_array[i] = string_array[i] - (CHAR)65 + (CHAR)97;
                }

            return result;
        }

        template <typename CHAR>
        void string<CHAR>::append_string(const CHAR* buffer, int bufsiz)
        {
            if (buffer)
            {
                //shift current data
                CHAR* temp = string_array;
                int tempsiz = array_length;

                //prep for new data
                array_length = tempsiz + bufsiz - 1;
                string_array = new CHAR[array_length];


                //copy old data
                for (int i=0; i<tempsiz-1; i++)
                    string_array[i] = temp[i];

                //copy new data
                for (int j=tempsiz-1; j<array_length; j++)
                    string_array[j] = buffer[j - tempsiz + 1];

                delete[] temp;
            }
        }

        template <typename CHAR>
        void string<CHAR>::append_char(CHAR singleChar)
        {
            //shift current data
            CHAR* temp = string_array;
            int tempsiz = array_length;

            if (!string_array || !array_length)
                tempsiz++;

            //prep for new data
            array_length = tempsiz + 1;
            string_array = new CHAR[array_length];

            //copy old data
            for (int i=0; i<tempsiz-1; i++)
                string_array[i] = temp[i];

            //append new data
            string_array[array_length-2] = singleChar;
            string_array[array_length-1] = null;

            delete[] temp;
        }

        template <typename CHAR>
        bool string<CHAR>::found_sub_string_at(int pos, const CHAR* substr, int substr_len) const
        {
            for (int i=0 ; i<substr_len-1; i++)
            {
                //if the string ends before all of the substring was found
                if (pos+i >= array_length-1)
                    return false;

                //if the exact substring was not found here
                if (substr[i] != string_array[i+pos])
                    return false;
            }

            return true;
        }

        template <typename CHAR>
        void string<CHAR>::replace_sub_string_at_with(int beg_index,
                                                      int end_index,
                                                      const CHAR* sub_str,
                                                      int substr_len)
        {
            if (string_array && array_length)
            {
                //make sure start index is within bounds
                if (beg_index < 0)
                    beg_index = 0;
                else if (beg_index >= (int)array_length-1)
                    return;

                //make sure end index is within bounds
                if (end_index < 0)
                    return;
                else if (end_index >= (int)array_length-1)
                    end_index = (int)array_length-2;

                //make sure start index is not after end index
                if (beg_index > end_index)
                    return;


                //shift the string data
                CHAR* temp = string_array;
                int tempsiz = array_length;

                //prep data
                array_length = array_length + substr_len - (end_index - beg_index);
                string_array = new CHAR[array_length];

                int index = 0;
                //copy original data until the start position
                for (int i=0; i<beg_index; i++)
                {
                    string_array[index] = temp[i];
                    index++;
                }

                //copy the new substring into place
                for (int i=0; i<substr_len-1; i++)
                {
                    string_array[index] = sub_str[i];
                    index++;
                }

                //copy original data from the end position
                for (int i=end_index+1; i<tempsiz; i++)
                {
                    string_array[index] = temp[i];
                    index++;
                }

                string_array[array_length-1] = null;

                delete[] temp;


            }
        }

        template <typename CHAR>
        int string<CHAR>::lessthan_equal_greater(const CHAR* other,
                                                 int str_size) const
        {
            int i = 0;

            while ((i<str_size-1) && (i<array_length-1))
            {
                if (string_array[i] < other[i])
                    return -1;
                else if (string_array[i] > other[i])
                    return 1;
                else if ((string_array[i] == null) && (other[i] == null))
                    return 0;

                i++;
            }

            if (array_length < str_size)
                return -1;
            else if (array_length > str_size)
                return 1;
            else
                return 0;
        }

        template <typename CHAR>
        void string<CHAR>::remove_portion(int beg_index, int end_index)
        {
            //make sure start index is within bounds
            if (beg_index < 0)
                beg_index = 0;
            else if (beg_index > (int)array_length-2)
                beg_index = (int)array_length-2;

            //make sure end index is within bounds
            if (end_index < 0)
                end_index = 0;
            else if (end_index > (int)array_length-2)
                end_index = (int)array_length-2;

            //make sure start index is not after end index
            if (beg_index > end_index)
                return;


            //shift data
            int tempsiz = array_length;
            CHAR* temp = string_array;

            //prep new string
            int section = (end_index - beg_index + 1);
            array_length = tempsiz - section;
            string_array = new CHAR[array_length];

            //copy part of old string before removed section
            for (int i=0; i<beg_index; i++)
                string_array[i] = temp[i];

            //copy part of old string after removed section
            for (int i=end_index+1; i<(int)tempsiz; i++)
                string_array[i - section] = temp[i];

            delete[] temp;
        }



        template <typename CHAR>
        int string<CHAR>::find(const string<CHAR>& sub_string) const
            {
                for (int i=0; i<array_length-1; i++)
                {
                    if (found_sub_string_at(i, sub_string.string_array,
                                            sub_string.array_length))
                        return (int)i;
                }

                return -1;
            }

        template <typename CHAR>
        int string<CHAR>::find(const string<CHAR>& sub_string, int n) const
        {
            if (n < 0)
                return -1;

            int amount = -1;

            for (int i=0; i<array_length-1; i++)
            {
                if (found_sub_string_at(i, sub_string.string_array,
                                        sub_string.array_length))
                    amount++;

                if (amount >= n)
                    return (int)i;
            }

            return -1;
        }

        template <typename CHAR>
        int string<CHAR>::findAfter(const string<CHAR>& sub_string, int n) const
        {
            for (int i=n; i<array_length-1; i++)
            {
                if (found_sub_string_at(i, sub_string.string_array, sub_string.array_length))
                    return (int)i;
            }

            return -1;
        }

        template <typename CHAR>
        int string<CHAR>::findLast(const string<CHAR>& sub_string) const
        {
            for (int i=array_length-1-sub_string.length(); i>=0; i--)
            {
                if (found_sub_string_at(i, sub_string.string_array, sub_string.array_length))
                    return (int)i;
            }

            return -1;
        }

        template <typename CHAR>
        bool string<CHAR>::replace(const string<CHAR>& sub_string,
                                   const string<CHAR>& new_string)
        {
            int found = find(sub_string);

            if (found > -1)
            {
                replace_sub_string_at_with(found,
                                           found + sub_string.array_length - 2,
                                           new_string.string_array,
                                           new_string.array_length - 1);

                return true;
            }

            return false;
        }

        template <typename CHAR>
        bool string<CHAR>::replace(const string<CHAR>& sub_string, int n,
                                   const string<CHAR>& new_string)
        {
            int found = find(sub_string, n);

            if (found > -1)
            {
                replace_sub_string_at_with(found,
                                           found + sub_string.array_length - 2,
                                           new_string.string_array,
                                           new_string.array_length - 1);

                return true;
            }

            return false;
        }

        template <typename CHAR>
        const string<CHAR> string<CHAR>::substr(int start_index, int end_index) const
        {
            //make sure start is in bounds
            if (start_index < 0)
                start_index = 0;
            else if (start_index > (int)array_length-2)
                return string<CHAR>();

            //make sure end is in bounds
            if (end_index < 0)
                return string<CHAR>();
            else if (end_index > (int)array_length-2)
                end_index = (int)array_length-2;

            //start must come before end
            if (start_index > end_index)
                return string<CHAR>();


            //accumulate data
            int len = end_index-start_index + 2;
            CHAR temp[len];
            for (int i=0; i<len-1; i++)
                temp[i] = string_array[i + start_index];
            temp[len - 1] = null;

            //assign data
            string<CHAR> sub_string (temp);

            return sub_string;
        }

        template <typename CHAR>
        int string<CHAR>::count(const string<CHAR>& sub_string) const
        {
            int amount = 0;

            for (int i=0; i<array_length-1; i++)
            {
                if (found_sub_string_at(i, sub_string.string_array, sub_string.array_length))
                    amount++;
            }

            return amount;
        }

        template <typename CHAR>
        bool string<CHAR>::foundAt(const string<CHAR>& sub_string, int position) const
        {
            if ((position < 0) || (position >= (int)array_length-1))
                return false;

            for (int i=0; i<sub_string.array_length-1; i++)
            {
                if (sub_string[i] != string_array[i + position])
                    return false;
            }

            return true;
        }

        template <typename CHAR>
        bool string<CHAR>::foundEndAt(const string<CHAR>& sub_string, int position) const
        {
            if ((position < 0) || (position >= (int)array_length-1))
                return false;

            for (int i=sub_string.array_length-2; i>=0; i--)
            {
                if (sub_string[i] != string_array[position])
                    return false;

                position--;
            }

            return true;
        }

        template <typename CHAR>
        bool string<CHAR>::beginsWith(const string<CHAR>& sub_string, CHAR pad_char) const
        {
            int i = 0;

            while ((string_array[i] == pad_char) && (i < (int)array_length-1))
            {
                if (foundAt(sub_string, i))
                    return true;

                i++;
            }

            return foundAt(sub_string, i);
        }

        template <typename CHAR>
        void string<CHAR>::clear()
        {
            delete[] string_array;
            //clear_data();

            array_length = 1;
            string_array = new CHAR[1];

            string_array[0] = null;
        }
    }
}


#undef num_bufsiz
#undef num_precision

#undef num_round_magic

#undef null

#endif // STRING_H_INCLUDED
